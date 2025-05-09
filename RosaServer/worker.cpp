#include "worker.h"

#include <thread>

#include "api.h"

// runThread uses a while true loop; this is for how many milliseconds it sleeps
// per iteration.
const long long THREAD_LOOP_SLEEP_TIME = 100;

Worker::Worker(std::string fileName) {
	std::thread thread(&Worker::runThread, this, fileName);
	thread.detach();
}

Worker::~Worker() {
	std::lock_guard<std::mutex> guard(destructionMutex);
	stop();
}

void Worker::runThread(std::string fileName) {
	std::atomic_bool* _stopped = &stopped;

	sol::state state;
	defineThreadSafeAPIs(&state);

	state["sendMessage"] = [this](std::string message) {
		this->l_sendMessage(message);
	};

	state["receiveMessage"] = [this](sol::this_state s) {
		return this->l_receiveMessage(s);
	};

	destructionMutex.lock();

	state["sleep"] = [this, &_stopped](unsigned int ms) -> bool {
		// Allow this to be deconstructed while sleeping
		this->destructionMutex.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));

		if (*_stopped) return true;

		this->destructionMutex.lock();
		return false;
	};

	{
		sol::load_result load = state.load_file(fileName);
		if (noLuaCallError(&load)) {
			sol::protected_function_result res = load();
			noLuaCallError(&res);
		}
	}

	if (!*_stopped) {
		destructionMutex.unlock();
	}

	while (!*_stopped) {
		std::this_thread::sleep_for(
		    std::chrono::milliseconds(THREAD_LOOP_SLEEP_TIME));
	}
}

void Worker::l_sendMessage(std::string message) {
	std::lock_guard<std::mutex> guard(receiveMessageQueueMutex);
	receiveMessageQueue.push(message);
	if (receiveMessageQueue.size() > 2047) receiveMessageQueue.pop();
}

sol::object Worker::l_receiveMessage(sol::this_state s) {
	sol::state_view state(s);

	sendMessageQueueMutex.lock();
	if (sendMessageQueue.empty()) {
		sendMessageQueueMutex.unlock();
		return sol::make_object(state, sol::nil);
	}

	auto message = sendMessageQueue.front();
	sendMessageQueue.pop();
	sendMessageQueueMutex.unlock();
	return sol::make_object(state, message);
}

void Worker::stop() {
	if (!stopped) {
		stopped = true;
	}
}

void Worker::sendMessage(std::string message) {
	if (stopped) return;

	std::lock_guard<std::mutex> guard(sendMessageQueueMutex);
	sendMessageQueue.push(message);
	if (sendMessageQueue.size() > 2047) sendMessageQueue.pop();
}

sol::object Worker::receiveMessage(sol::this_state s) {
	sol::state_view state(s);

	receiveMessageQueueMutex.lock();
	if (receiveMessageQueue.empty()) {
		receiveMessageQueueMutex.unlock();
		return sol::make_object(state, sol::nil);
	}

	auto message = receiveMessageQueue.front();
	receiveMessageQueue.pop();
	receiveMessageQueueMutex.unlock();
	return sol::make_object(state, message);
}

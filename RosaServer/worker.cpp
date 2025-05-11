#include "worker.h"

#include <mutex>
#include <thread>

#include "api.h"

// runThread uses a while true loop; this is for how many milliseconds it sleeps
// per iteration.
const long long THREAD_LOOP_SLEEP_TIME = 1000;

Worker::Worker(std::string fileName) {
	workerThread = std::thread(&Worker::runThread, this, fileName);
}

Worker::~Worker() {
	stop();
	workerThread.join();
}

void Worker::runThread(std::string fileName) {
	sol::state state;
	defineThreadSafeAPIs(&state);

	state["sendMessage"] = [this](std::string message) {
		this->l_sendMessage(message);
	};

	state["receiveMessage"] = [this](sol::this_state s) {
		return this->l_receiveMessage(s);
	};

	state["sleep"] = [this](unsigned int ms) -> bool {
		{
			std::unique_lock<std::mutex> lock(destructionMutex);
			stopCondition.wait_for(lock, std::chrono::milliseconds(ms));
		}

		if (this->stopped) return true;
		return false;
	};

	{
		sol::load_result load = state.load_file(fileName);
		if (noLuaCallError(&load)) {
			sol::protected_function_result res = load();
			noLuaCallError(&res);
		}
	}

	while (!stopped) {
		std::unique_lock<std::mutex> lock(destructionMutex);
		stopCondition.wait_for(lock,
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
	{
		std::lock_guard<std::mutex> guard(destructionMutex);
		stopped = true;
	}
	stopCondition.notify_all();
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

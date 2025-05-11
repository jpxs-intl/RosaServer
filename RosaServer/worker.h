#pragma once
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>

#include "sol/sol.hpp"

class Worker {
	std::atomic_bool stopped;
	std::mutex destructionMutex;
	std::condition_variable stopCondition;

	std::queue<std::string> sendMessageQueue;
	std::mutex sendMessageQueueMutex;

	std::queue<std::string> receiveMessageQueue;
	std::mutex receiveMessageQueueMutex;

	void runThread(std::string fileName);
	void l_sendMessage(std::string message);
	sol::object l_receiveMessage(sol::this_state s);

 public:
	Worker(std::string fileName);
	~Worker();
	void stop();
	void sendMessage(std::string message);
	sol::object receiveMessage(sol::this_state s);
};

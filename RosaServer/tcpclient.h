#pragma once

#include <memory>
#include <string>
#include <vector>

#include "sol/sol.hpp"

class TCPClient {
	int socketDescriptor;

 public:
	TCPClient(std::string_view address, std::string_view port);
	~TCPClient();

	void close();
	bool isOpen() const { return socketDescriptor != -1; }

	ssize_t send(std::string_view data) const;
	sol::object receive(size_t size, sol::this_state state);
};

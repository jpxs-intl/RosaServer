#include "tcpclient.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <unistd.h>

#include <cstring>
#include <stdexcept>

static constexpr const char* errorNotOpen = "Socket is not open";

static inline void throwSafe() {
	char error[256];
	throw std::runtime_error(strerror_r(errno, error, sizeof(error)));
}

ssize_t TCPClient::send(std::string_view data) const {
	if (socketDescriptor == -1) {
		throw std::runtime_error(errorNotOpen);
	}

	if (data.empty()) {
		throw std::runtime_error("Data is empty");
	}

	auto bytesWritten = write(socketDescriptor, data.data(), data.size());
	if (bytesWritten == -1) {
		if (errno == EAGAIN) {
			return 0;
		}
		throwSafe();
	}

	return bytesWritten;
}

sol::object TCPClient::receive(size_t size, sol::this_state s) {
	if (socketDescriptor == -1) {
		throw std::runtime_error(errorNotOpen);
	}

	sol::state_view lua(s);

	constexpr auto maxToRecv = sizeof(receiveBuffer);
	auto bytesRead =
	    read(socketDescriptor, receiveBuffer, std::min(size, maxToRecv));
	if (bytesRead == -1) {
		if (errno == EAGAIN) {
			return sol::make_object(lua, sol::nil);
		}
		throwSafe();
	}

	if (bytesRead == 0) {
		close();
	}

	std::string data(receiveBuffer, bytesRead);
	return sol::make_object(lua, data);
}

TCPClient::TCPClient(std::string_view address, std::string_view port) {
	addrinfo hintInfo{};
	hintInfo.ai_family = AF_UNSPEC;
	hintInfo.ai_socktype = SOCK_STREAM;
	hintInfo.ai_flags = 0;
	hintInfo.ai_protocol = 0;

	addrinfo* resultAddress;
	auto resultErr =
	    getaddrinfo(address.data(), port.data(), &hintInfo, &resultAddress);
	if (resultErr != 0) {
		throw std::runtime_error(gai_strerror(resultErr));
	}

	addrinfo* addrIter;
	for (addrIter = resultAddress; addrIter != nullptr;
	     addrIter = resultAddress->ai_next) {
		auto family = addrIter->ai_family;
		if (family == AF_UNSPEC) {
			family = AF_INET;
		}

		socketDescriptor = socket(family, SOCK_STREAM | SOCK_NONBLOCK, 0);
		if (socketDescriptor == -1) continue;

		if (connect(socketDescriptor, addrIter->ai_addr, addrIter->ai_addrlen) == 0)
			break;
		if (errno == EINPROGRESS) break;

		::close(socketDescriptor);
	}

	if (addrIter == nullptr) throw std::runtime_error("Failed to connect!");
	freeaddrinfo(resultAddress);
}

void TCPClient::close() {
	if (socketDescriptor == -1) {
		throw std::runtime_error(errorNotOpen);
	}

	::close(socketDescriptor);
	socketDescriptor = -1;
}

TCPClient::~TCPClient() {
	if (socketDescriptor != -1) {
		close();
	}
}

#include <unistd.h>

#include <chrono>
#include <thread>

#include "sol/sol.hpp"

static constexpr int CODE_INVALID_USAGE = 1;
static constexpr int CODE_FILE_INVALID = 2;
static constexpr int CODE_FILE_RUNTIME_ERROR = 3;

static constexpr const char* ERR_WRITING_MESSAGE =
    "Couldn't write full message to pipe";

static int fdFromParent;
static int fdToParent;

static double l_os_realClock() {
	auto now = std::chrono::steady_clock::now();
	auto ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
	auto epoch = ms.time_since_epoch();
	auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
	return value.count() / 1000.;
}

static sol::object l_receiveMessage(sol::this_state s) {
	sol::state_view lua(s);

	unsigned int length;

	auto bytesRead = read(fdFromParent, &length, sizeof(length));
	if (bytesRead == -1) {
		if (errno != EAGAIN) {
			throw std::runtime_error(strerror(errno));
		}
	} else if (bytesRead == sizeof(length)) {
		std::string message(length, ' ');
		bytesRead = read(fdFromParent, message.data(), length);
		if (bytesRead == -1) {
			if (errno != EAGAIN) {
				throw std::runtime_error(strerror(errno));
			}
		} else if (bytesRead == length) {
			return sol::make_object(lua, message);
		}
	}

	return sol::make_object(lua, sol::nil);
}

static void l_sendMessage(std::string message) {
	unsigned int length = static_cast<unsigned int>(message.length());

	auto bytesWritten = write(fdToParent, &length, sizeof(length));
	if (bytesWritten == -1) {
		throw std::runtime_error(strerror(errno));
	} else if (bytesWritten != sizeof(length)) {
		throw std::runtime_error(ERR_WRITING_MESSAGE);
	} else {
		bytesWritten = write(fdToParent, message.data(), length);
		if (bytesWritten == -1) {
			throw std::runtime_error(strerror(errno));
		} else if (bytesWritten != length) {
			throw std::runtime_error(ERR_WRITING_MESSAGE);
		}
	}
}

// https://github.com/moonjit/moonjit/blob/master/doc/c_api.md#luajit_setmodel-idx-luajit_mode_wrapcfuncflag
static int wrapExceptions(lua_State* L, lua_CFunction f) {
	try {
		return f(L);
	} catch (const char* s) {
		lua_pushstring(L, s);
	} catch (std::exception& e) {
		lua_pushstring(L, e.what());
	} catch (...) {
		lua_pushliteral(L, "caught (...)");
	}
	return lua_error(L);
}

int main(int argc, const char* argv[]) {
	if (argc < 4) return CODE_INVALID_USAGE;

	fdFromParent = atoi(argv[1]);
	fdToParent = atoi(argv[2]);
	const char* fileName = argv[3];

	sol::state lua;

	lua_pushlightuserdata(lua, (void*)wrapExceptions);
	luaJIT_setmode(lua, -1, LUAJIT_MODE_WRAPCFUNC | LUAJIT_MODE_ON);
	lua_pop(lua, 1);

	lua.open_libraries(sol::lib::base);
	lua.open_libraries(sol::lib::package);
	lua.open_libraries(sol::lib::coroutine);
	lua.open_libraries(sol::lib::string);
	lua.open_libraries(sol::lib::os);
	lua.open_libraries(sol::lib::math);
	lua.open_libraries(sol::lib::table);
	lua.open_libraries(sol::lib::debug);
	lua.open_libraries(sol::lib::bit32);
	lua.open_libraries(sol::lib::io);
	lua.open_libraries(sol::lib::ffi);
	lua.open_libraries(sol::lib::jit);

	lua["os"]["realClock"] = l_os_realClock;

	lua["receiveMessage"] = l_receiveMessage;
	lua["sendMessage"] = l_sendMessage;

	lua["sleep"] = [](unsigned int ms) {
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	};

	sol::load_result load = lua.load_file(fileName);
	if (load.valid()) {
		sol::protected_function_result res = load();
		if (!res.valid()) {
			return CODE_FILE_RUNTIME_ERROR;
		}
	} else {
		return CODE_FILE_INVALID;
	}

	return 0;
}

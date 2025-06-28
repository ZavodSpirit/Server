#pragma once

#include <ctime>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <iostream>
#include <fstream>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#define WM_SOCKET WM_USER

namespace model {
	class Model final {
	public:
		Model() noexcept;
		Model(const Model&) = delete;
		Model(Model&&) = delete;

		~Model() noexcept;

		void Listen(HWND) noexcept;
		void Disconnect(size_t) noexcept;

		std::vector<std::wstring> getUsers() const noexcept;
		std::vector<tm> getConnTime() const noexcept;

		tm getActiveTime(int) const noexcept;

		void getPrintScreen(int) const noexcept;

		Model& operator=(const Model&) = delete;
		Model& operator=(Model&&) = delete;
	private:
		bool status = { false };
		SOCKET sListen = {};
		sockaddr_in servInfo = {};
		std::vector<SOCKET> sConnections = {};
		std::vector<sockaddr_in> connInfo = {};
		std::vector<std::wstring> users = {};
		std::vector<tm> connTime = {};
	};
}

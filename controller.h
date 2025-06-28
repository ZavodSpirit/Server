#pragma once

#include <ctime>
#include <exception>
#include <mutex>
#include <thread>
#include <string>
#include <vector>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include "model.h"
#include "view.h"

namespace controller {
	void StartModel(HWND) noexcept;
}

namespace controller {
	LRESULT CALLBACK Controller(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
}

#pragma once

#include <ctime>
#include <exception>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include "controller.h"
#include "model.h"
#include "view.h"
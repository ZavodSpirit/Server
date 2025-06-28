#pragma once

#include <ctime>
#include <exception>
#include <string>
#include <vector>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#define ON_MENU_REFRESH 1
#define ON_MENU_USER_FIRST 10
#define ON_BUTTON_ACTIVETIME 2
#define ON_BUTTON_PRINTSCREEN 3

#define SWC_NAME L"MainServerWindowClass"
#define APP_NAME L"Мониторинг рабочей активности"
#define MSW_SIZE_X 1500
#define MSW_SIZE_Y 750

namespace view {
	void InitializeMainClass(WNDCLASS&) noexcept;
	HWND CreateMainWindow();
}

namespace view {
	class View final {
	public:
		View() noexcept;
		View(const View&) = delete;
		View(View&&) = delete;

		~View() noexcept;

		void CreateMenus(HWND, const std::vector<std::wstring>&) const noexcept;

		void CreateListOfUsers(HWND) noexcept;
		void PrintListOfUsers(const std::vector<std::wstring>&) const noexcept;

		void CreateUserProcessorWidgets(HWND) noexcept;
		void PrintUser(std::wstring, tm) const noexcept;
		void PrintActiveTime(tm) const noexcept;

		View& operator=(View&) = delete;
		View& operator=(View&&) = delete;
	private:
		HWND hListOfUsers = { 0 };
		HWND hUser = { 0 };
		HWND hActiveTime = { 0 };
		HWND hEntryTime = { 0 };
	};
}
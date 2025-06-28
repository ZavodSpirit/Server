#include "view.h"

namespace view {
	void InitializeMainClass(WNDCLASS& SWC) noexcept {
		SWC.hCursor = LoadCursor(NULL, IDC_ARROW);
		SWC.hIcon = LoadIcon(NULL, IDI_QUESTION);
		SWC.lpszClassName = SWC_NAME;
		SWC.hbrBackground = (HBRUSH)COLOR_WINDOW;
	}

	HWND CreateMainWindow() {
		HWND hWnd = CreateWindow(SWC_NAME, APP_NAME, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, MSW_SIZE_X, MSW_SIZE_Y, NULL, NULL, NULL, NULL);
		if(hWnd == NULL)
			throw std::exception("Error");
		return hWnd;
	}
}

namespace view {
	View::View() noexcept {
	}

	View::~View() noexcept {
	}

	void View::CreateMenus(HWND hWnd, const std::vector<std::wstring>& users) const noexcept {
		HMENU RootMenu = CreateMenu();
		HMENU SubMenu = CreateMenu();
		UINT_PTR command = ON_MENU_USER_FIRST;
		for (auto it = users.begin(), end = users.end(); it != end; ++it, ++command) {
			AppendMenu(SubMenu, MF_STRING, command, it->c_str());
		}
		AppendMenu(SubMenu, MF_SEPARATOR, NULL, NULL);
		AppendMenu(SubMenu, MF_STRING, ON_MENU_REFRESH, L"Обновить");
		AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu, L"Пользователи");
		SetMenu(hWnd, RootMenu);
	}

	void View::CreateListOfUsers(HWND hWnd) noexcept {
		CreateWindowW(L"static", L"Список всех подключенний", WS_VISIBLE | WS_CHILD | ES_CENTER, 5, 5, (MSW_SIZE_X - 10) * 2 / 3, 20, hWnd, NULL, NULL, NULL);
		hListOfUsers = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE, 5, 30, (MSW_SIZE_X - 10) * 2 / 3, MSW_SIZE_Y - 95, hWnd, NULL, NULL, NULL);
	}

	void View::PrintListOfUsers(const std::vector<std::wstring>& users) const noexcept {
		std::wstring buffer = {};
		for (auto it = users.begin(), end = users.end(); it != end; ++it) {
			buffer += *it;
			buffer += L"\r\n";
		}
		SetWindowTextW(hListOfUsers, buffer.c_str());
	}

	void View::CreateUserProcessorWidgets(HWND hWnd) noexcept {
		CreateWindowW(L"static", L"Подключение:", WS_VISIBLE | WS_CHILD, (MSW_SIZE_X - 10) * 2 / 3 + 10, 5, (MSW_SIZE_X - 10) / 3 - 20, 20, hWnd, NULL, NULL, NULL);
		hUser = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_HSCROLL, (MSW_SIZE_X - 10) * 2 / 3 + 10, 30, (MSW_SIZE_X - 10) / 3 - 20, 40, hWnd, NULL, NULL, NULL);
		CreateWindowW(L"button", L"Последнее время активности", WS_VISIBLE | WS_CHILD | ES_CENTER, (MSW_SIZE_X - 10) * 2 / 3 + 10, 80, 250, 30, hWnd, (HMENU)ON_BUTTON_ACTIVETIME, NULL, NULL);
		CreateWindowW(L"static", L"Последнее время активности:", WS_VISIBLE | WS_CHILD | ES_CENTER, (MSW_SIZE_X - 10) * 2 / 3 + 10, 150, 150, 30, hWnd, NULL, NULL, NULL);
		hActiveTime = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | ES_CENTER, (MSW_SIZE_X - 10) * 2 / 3 + 10 + 200, 150, 250, 30, hWnd, NULL, NULL, NULL);
		CreateWindowW(L"static", L"Последнее время входа:", WS_VISIBLE | WS_CHILD | ES_CENTER, (MSW_SIZE_X - 10) * 2 / 3 + 10, 200, 150, 30, hWnd, NULL, NULL, NULL);
		hEntryTime = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | ES_CENTER, (MSW_SIZE_X - 10) * 2 / 3 + 10 + 200, 200, 250, 30, hWnd, NULL, NULL, NULL);
		CreateWindowW(L"button", L"Скрин экрана", WS_VISIBLE | WS_CHILD | ES_CENTER, (MSW_SIZE_X - 10) * 2 / 3 + 10, 300, 150, 30, hWnd, (HMENU)ON_BUTTON_PRINTSCREEN, NULL, NULL);
	}

	void View::PrintUser(std::wstring name, tm entryTime) const noexcept {
		SetWindowTextW(hUser, name.c_str());
		std::wstring buffer = std::to_wstring(entryTime.tm_year%100) + L" y. " + std::to_wstring(entryTime.tm_mon) + L" m. " +
			std::to_wstring(entryTime.tm_mday) + L" d. " + std::to_wstring(entryTime.tm_hour) + L" h. " + std::to_wstring(entryTime.tm_min)
			+ L" min. " + std::to_wstring(entryTime.tm_sec) + L" s.";
		SetWindowTextW(hEntryTime, buffer.c_str());
	}

	void View::PrintActiveTime(tm activeTime) const noexcept {
		std::wstring buffer = std::to_wstring(activeTime.tm_year % 100) + L" y. " + std::to_wstring(activeTime.tm_mon) + L" m. " +
			std::to_wstring(activeTime.tm_mday) + L" d. " + std::to_wstring(activeTime.tm_hour) + L" h. " + std::to_wstring(activeTime.tm_min)
			+ L" min. " + std::to_wstring(activeTime.tm_sec) + L" s.";
		SetWindowTextW(hActiveTime, buffer.c_str());
	}
}
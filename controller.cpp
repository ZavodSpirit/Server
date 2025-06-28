#include "controller.h"

static model::Model ServerModel = {};
static view::View ServerView = {};

static std::vector<std::wstring> users = {};
static std::vector<tm> connTime = {};
static int index = { -1 };

static std::thread Listen = {};

namespace controller {
	void StartModel(HWND hWnd) noexcept {
		Listen = std::thread([hWnd]() {ServerModel.Listen(hWnd);});
		Listen.detach();
	}
}

namespace controller {
	LRESULT CALLBACK Controller(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
		if (msg >= WM_SOCKET) {
			ServerModel.Disconnect(msg - WM_USER);
		}
		switch (msg) {
		case WM_COMMAND:
			if ((wp >= ON_MENU_USER_FIRST) and (wp < users.size() + ON_MENU_USER_FIRST)) {
				index = wp - ON_MENU_USER_FIRST;
				ServerView.PrintUser(users.at(index), connTime.at(index));
				break;
			}
			switch (wp) {
			case ON_MENU_REFRESH:
				users = ServerModel.getUsers();
				connTime = ServerModel.getConnTime();
				ServerView.CreateMenus(hWnd, users);
				ServerView.PrintListOfUsers(users);
				ServerView.PrintUser(L"", {});
				ServerView.PrintActiveTime({});
				index = -1;
				break;
			case ON_BUTTON_ACTIVETIME:
				ServerView.PrintActiveTime(ServerModel.getActiveTime(index));
				break;
			case ON_BUTTON_PRINTSCREEN:
				ServerModel.getPrintScreen(index);
				break;
			}
			break;
		case WM_CREATE:
			ServerView.CreateMenus(hWnd, {});
			ServerView.CreateListOfUsers(hWnd);
			ServerView.CreateUserProcessorWidgets(hWnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default: return DefWindowProc(hWnd, msg, wp, lp);
		}
	}
}
#include "pch.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
	WNDCLASS ServerMainClass = { 0 };
	view::InitializeMainClass(ServerMainClass);
	ServerMainClass.hInstance = hInst;
	ServerMainClass.lpfnWndProc = controller::Controller;
	if (!RegisterClassW(&ServerMainClass)) return -1;
	MSG SoftwareMainMessage = { 0 };
	HWND hWnd = {};
	try { hWnd = view::CreateMainWindow(); }
	catch (...) { return -1; }
	controller::StartModel(hWnd);
	while (GetMessage(&SoftwareMainMessage, NULL, NULL, NULL)) {
		TranslateMessage(&SoftwareMainMessage);
		DispatchMessage(&SoftwareMainMessage);
	}
}
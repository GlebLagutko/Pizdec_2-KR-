#pragma once

#include "stdafx.h"

class WinApiModule
{
	const WCHAR* _windowClassName;
	const WCHAR* _windowTitle;
	HINSTANCE _hInstance;
	WNDPROC _wndProc;

public:
	WinApiModule(const WCHAR* windowClassName, const WCHAR* windowTitle, HINSTANCE hInstance, WNDPROC wndProc)
		: _windowClassName(windowClassName), _windowTitle(windowTitle), _hInstance(hInstance), _wndProc(wndProc) {  }

	int Run(int nCmdShow)
	{
		RegisterWindowClass(_hInstance);

		if (!InitializeInstance(_hInstance, nCmdShow))
		{
			return false;
		}

		MSG msg;
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return (int)msg.wParam;
	}

private:
	ATOM RegisterWindowClass(HINSTANCE hInstance)
	{
		auto windowClass = WNDCLASSEXW();

		windowClass.cbSize = sizeof(WNDCLASSEX);

		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = _wndProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = hInstance;
		windowClass.hIcon = nullptr;
		windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		windowClass.lpszMenuName = nullptr;
		windowClass.lpszClassName = _windowClassName;
		windowClass.hIconSm = nullptr;

		return RegisterClassExW(&windowClass);
	}

	bool InitializeInstance(HINSTANCE hInstance, int nCmdShow)
	{
		auto hWnd = CreateWindowW(
			_windowClassName,
			_windowTitle,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0,
			CW_USEDEFAULT, 0,
			nullptr,
			nullptr,
			hInstance,
			nullptr);

		if (!hWnd)
		{
			return false;
		}

		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);

		return true;
	}
};

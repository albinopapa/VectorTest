#pragma once
#include <Windows.h>
class Window
{
public:
	Window();
	Window(HINSTANCE Instance, int CmdShow, UINT Width, UINT Height);
	~Window();

	BOOL HandleMessageLoop();
	UINT Width()const;
	UINT Height()const;
	HWND Handle();
private:
	UINT width, height;
	HWND hWnd;
	HINSTANCE hInst;
	LPCWSTR clName;
};


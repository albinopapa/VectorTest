#pragma once
#include <Windows.h>
#include <string>
class Window
{
public:
	Window();
	Window(HINSTANCE Instance, int CmdShow, UINT Width, UINT Height);
	~Window();

	HRESULT Create(const RECT &WinDim);
	void Show();
	HRESULT Register(const WNDCLASSEX &WC);
	HRESULT Unregister();

	void SetTitle(const std::wstring &Title);
	BOOL HandleMessageLoop();
	UINT Width()const;
	UINT Height()const;
	HWND Handle();
private:
	UINT width, height;
	HWND hWnd;
	HINSTANCE hInst;
	std::wstring clName;
	std::wstring winTitle;

	UINT winStyle, exWinStyle;
	ATOM atom;
};


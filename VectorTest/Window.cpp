#include "Window.h"
#include "System.h"

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	System *pSystem = reinterpret_cast<System *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	LRESULT res = 0;

	if (pSystem)
	{
		res = pSystem->MsgProc(msg, wParam, lParam);
	}
	else
	{
		res = DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return res;
}

Window::Window()
	:
	width(0),
	height(0),
	hWnd(nullptr),
	hInst(nullptr),
	clName(nullptr)
{
}

Window::Window(HINSTANCE Instance, int CmdShow, UINT Width, UINT Height)
{
	clName = L"Chili DirectX Framework Window";
	width = Width;
	height = Height;

	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0, 0,
		Instance, NULL, NULL, NULL, NULL,
		clName, NULL };

	RegisterClassEx(&wc);

	RECT wr;
	wr.left = 0;
	wr.right = width + wr.left;
	wr.top = 0;
	wr.bottom = height + wr.top;
	AdjustWindowRect(&wr, WS_POPUP, FALSE);

	hWnd = CreateWindowW(clName, L"Vector Test",
		WS_POPUP, wr.left, wr.top, width, height,
		NULL, NULL, wc.hInstance, NULL);

	ShowWindow(hWnd, CmdShow);
}

Window::~Window()
{
	UnregisterClass(clName, hInst);
}

BOOL Window::HandleMessageLoop()
{
	MSG msg{};

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.message == WM_QUIT;
}

UINT Window::Width()const
{
	return width;
}

UINT Window::Height()const
{
	return height;
}

HWND Window::Handle()
{
	return hWnd;
}
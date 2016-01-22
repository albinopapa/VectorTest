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
	winTitle = L"VectorTest";
	width = Width;
	height = Height;
	winStyle = WS_POPUP;
	exWinStyle = 0;
	hInst = Instance;

	WNDCLASSEX wc{};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = MsgProc;
	wc.hInstance = hInst;
	wc.lpszClassName = clName.data();

	Register(wc);

	RECT wr;
	wr.left = 0;
	wr.right = width + wr.left;
	wr.top = 0;
	wr.bottom = height + wr.top;

	Create(wr);	
	Show();
}

Window::~Window()
{
	Unregister();
}

HRESULT Window::Create(const RECT &WinDim)
{
	hWnd = CreateWindowEx(exWinStyle, clName.data(), winTitle.data(),
		winStyle, WinDim.left, WinDim.top, width, height, nullptr,
		nullptr, hInst, nullptr);

	return hWnd != nullptr ? S_OK : E_FAIL;
}
void Window::Show()
{
	ShowWindow(hWnd, SW_SHOWDEFAULT);
}
HRESULT Window::Register(const WNDCLASSEX &WC)
{
	atom = RegisterClassEx(&WC);
	if (atom != 0)
	{
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}
HRESULT Window::Unregister()
{
	return UnregisterClass(MAKEINTATOM(atom), hInst) ? S_OK : E_FAIL;
}
void Window::SetTitle(const std::wstring &Title)
{
	SetWindowText(hWnd, Title.data());
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
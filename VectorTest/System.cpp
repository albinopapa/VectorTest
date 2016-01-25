#include "System.h"
#include <vector>
#include <list>

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
	int exitCode = 0;
	UINT sWidth = 0, sHeight = 0;
	sWidth = GetSystemMetrics(SM_CXSCREEN);
	sHeight = GetSystemMetrics(SM_CYSCREEN);

	auto window = std::make_shared<Window>(Window(hInst, SW_SHOWDEFAULT, sWidth, sHeight));
	std::unique_ptr<System> system;
	if (window)
	{
		system.reset(new System(window));
	}
	if (system)
	{
		exitCode = system->Run();
	}

	std::list<System> vSys;
	
	return exitCode;
}

System::System(std::shared_ptr<Window> &spWin)
	:
	window(spWin),
	kServ(new KeyboardServer()),
	mServ(new MouseServer()),
	game(new Game(spWin, kServ, mServ))
{
	SetWindowLongPtr(window->Handle(), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

LRESULT System::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

		// ************ KEYBOARD MESSAGES ************ //
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
		}
		else
		{
			kServ->OnKeyPressed((UINT)wParam);
		}
		break;
	case WM_KEYUP:
		kServ->OnKeyReleased((UINT)wParam);
		break;
	case WM_CHAR:
		kServ->OnChar((UINT)wParam);
		break;
		// ************ END KEYBOARD MESSAGES ************ //

		// ************ MOUSE MESSAGES ************ //
	case WM_MOUSEMOVE:
	{
		UINT x = (short)LOWORD(lParam);
		UINT y = (short)HIWORD(lParam);
		if (x > 0 && x < window->Width() && y > 0 && y < window->Height())
		{
			mServ->OnMouseMove(x, y);
			if (!mServ->IsInWindow())
			{
				SetCapture(window->Handle());
				mServ->OnMouseEnter();
			}
		}
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				x = max(0, x);
				x = min(window->Width() - 1, x);
				y = max(0, y);
				y = min(window->Height() - 1, y);
				mServ->OnMouseMove(x, y);
			}
			else
			{
				ReleaseCapture();
				mServ->OnMouseLeave();
				mServ->OnLeftReleased();
				mServ->OnRightReleased();
			}
		}
	}
	break;
	case WM_LBUTTONDOWN:
		mServ->OnLeftPressed();
		break;
	case WM_RBUTTONDOWN:
		mServ->OnRightPressed();
		break;
	case WM_LBUTTONUP:
		mServ->OnLeftReleased();
		break;
	case WM_RBUTTONUP:
		mServ->OnRightReleased();
		break;
		// ************ END MOUSE MESSAGES ************ //
	}

	return DefWindowProc(window->Handle(), msg, wParam, lParam);
}

int System::Run()
{
	BOOL done = FALSE;
	while (!done)
	{
		done = window->HandleMessageLoop();
		game->Go();
	}

	return 0;
}

System::~System()
{
}

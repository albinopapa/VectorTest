#pragma once

#include <memory>
#include <Windows.h>
#include "Window.h"
#include "Game.h"
class System
{
public:
	System(std::shared_ptr<Window> &spWin);
	~System();

	LRESULT MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	int Run();


private:
	System(const System &rSys);
	System &operator=(const System &rSys);

	std::shared_ptr<KeyboardServer> kServ;
	std::shared_ptr<MouseServer> mServ;
	std::shared_ptr<Window> window;
	std::unique_ptr<Game> game;
};


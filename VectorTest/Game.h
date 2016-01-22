#pragma once

#include <memory>
#include <math.h>
#include <immintrin.h>
#include "D3DGraphics.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Window.h"
#include "Timer.h"
#include "Vec2f.h"
#include "Vector.h"
#include "AlignedPtr.h"

class Game
{
public:
	Game(std::shared_ptr<Window> &rWin, std::shared_ptr<KeyboardServer>& kServer, std::shared_ptr<MouseServer> &mServ);
	~Game();
	void Go();

private:
	void UpdateTime();
	void ComposeFrame();

private:
	std::shared_ptr<D3DGraphics> gfx;// DirectX Framework Variable
	std::shared_ptr<KeyboardClient> kbd;// Access to keyboard variable
	std::shared_ptr<MouseClient> mouse;
	std::shared_ptr<Window> win;
	Font fixedSys;

	Timer timer;
	float dt;
	char fps[16]{};

	AlignedPtr<D3DCOLOR> pSurf;
};
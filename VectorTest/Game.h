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
#include "Vec2SSE.h"
// WIP
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
	void UpdateFrame();

private:
	std::shared_ptr<D3DGraphics> gfx;// DirectX Framework Variable
	std::shared_ptr<KeyboardClient> kbd;// Access to keyboard variable
	std::shared_ptr<MouseClient> mouse;
	std::shared_ptr<Window> win;
	Font fixedSys;

	Timer timer;
	float dt;
	char fps[16]{};

	int radius;
	D3DCOLOR* pSurf;

	float g;
	int numBalls;
	Vector2 *pos;
	Vector2 *vel;
	Vector2 *acc;
};
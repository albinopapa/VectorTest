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
#include "Vec3SSE.h"
#include "Vec4SSE.h"

// WIP
#include "AlignedPtr.h"
#include "CpuID.h"


class Game
{
public:
	Game(std::shared_ptr<Window> &rWin, std::shared_ptr<KeyboardServer>& kServer, std::shared_ptr<MouseServer> &mServ);
	~Game();
	void Go();

private:
	void InitBalls();
	void CreateSurface();
	void UpdateTime();
	void ComposeFrame();
	void UpdateFrameSSE();
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
	std::unique_ptr<Vector2[]> pos;
	std::unique_ptr<Vector2[]> vel;
	std::unique_ptr<Vector2[]> acc;

	std::unique_ptr<Vec2f[]> vPos;
	std::unique_ptr<Vec2f[]> vVel;
	std::unique_ptr<Vec2f[]> vAcc;

	CpuID cpuid;
};
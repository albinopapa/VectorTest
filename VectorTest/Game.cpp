#include "Game.h"
#include "Bitmap.h"
#include <ctime>
#include <sstream>
#include <string>
#include <ppl.h>

using namespace SSE_Utils::Float4_Utils;

Game::Game(std::shared_ptr<Window> &rWin, std::shared_ptr<KeyboardServer>& kServer,
	std::shared_ptr<MouseServer> &mServ)
	: 
	gfx(new D3DGraphics(rWin)),
	kbd(new KeyboardClient(*kServer.get())),
	mouse(new MouseClient(*mServ.get())),
	win(rWin)
{
	srand((UINT)time(NULL));
	LoadFont(&fixedSys, "Fonts\\Fixedsys16x28.bmp", 16, 28, 32);
	skelly = Skeleton({ (float)win->Width() / 2, (float)win->Height() / 2, 0.0f, 0.0f });
	

	//CreateSurface();
	//InitBalls();

}

Game::~Game()
{
}

void Game::Go()
{
	UpdateTime();
	//UpdateFrameSSE();
	static float f = 0.016f;
	f += 0.000016f;
	f = fmodf(f, DirectX::XM_2PI);
	float fm = f - DirectX::XM_PI;
	skelly.Update(f);
	gfx->BeginFrame();
	ComposeFrame();
	gfx->EndFrame();
}

void Game::InitBalls()
{
	numBalls = 2000;
	ball.reset(new Ball[numBalls]);

	std::default_random_engine rnd;

	for (int i = 0; i < numBalls; ++i)
	{
		ball[i] = Ball(rnd, i, win, pSurf);
	}
}

void Game::CreateSurface()
{
	radius = 8;
	int diam = radius * 2;
	float invRad = 1.0f / (float)radius;

	pSurf = new D3DCOLOR[diam * diam];
	int index = 0;
	for (int y = 0; y < diam; ++y)
	{
		int iy = y - radius;
		for (int x = 0; x < diam; ++x)
		{
			int ix = x - radius;
			int sqrDist = (ix * ix) + (iy * iy);
			index = x + (y * diam);

			UINT alpha = 0;
			D3DCOLOR c = D3DCOLOR_ARGB(alpha, 255, alpha, 0);

			if (sqrDist <= radius * radius)
			{
				float dist = sqrtf((float)sqrDist);
				UINT iDist = (UINT)((dist * invRad * 255.0f) + 0.5f);
				alpha = 255 - iDist;
				c = D3DCOLOR_ARGB(alpha, 0, alpha, 255);
			}

			pSurf[index] = c;
		}
	}
}

void Game::UpdateFrameSSE()
{
	for (int i = 0; i < numBalls; ++i)		
	{
		ball[i].Update(ball.get(), numBalls, dt);
	}
}

void Game::UpdateTime()
{
	timer.StopWatch();
	const float mili = timer.GetTimeMilli();
	dt = mili * 0.01f;

	itoa(Lint(1000.0f / mili), fps, 10);
	wchar_t wfps[16]{};
	MultiByteToWideChar(0, 0, fps, 16, wfps, 16);
	SetWindowText(win->Handle(), wfps);

	timer.StartWatch();
}

void Game::ComposeFrame()
{
	std::stringstream ss;
	ss << "Frames per second: " << fps;
	std::string text = ss.str();
	gfx->DrawString(text, 0, 0, fixedSys, D3DCOLOR_XRGB(0, 255, 0));

	/*for (int i = 0; i < numBalls; ++i)
	{
		ball[i].Draw(*gfx.get());
	}*/

	skelly.Draw(*gfx.get());

}

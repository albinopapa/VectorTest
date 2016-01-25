#include "Game.h"
#include "Bitmap.h"
#include <ctime>
#include <sstream>
#include <string>
#include <random>

typedef __m128 Float4;

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

	//enum Feature_Ecx
	//{
	//	// ECX register
	//	SSE3 = 0x00000001,
	//	pclmulqdq = 1 << 1,
	//	dtes64 = 1 << 2,
	//	monitor = 1 << 3,
	//	ds_cpl = 1 << 4,
	//	Virtual_Machine_Instructions = 1 << 5,
	//	Safe_Mode_Extensions = 1 << 6,
	//	Enhanced_Speed_Step = 1 << 7,
	//	Thermal_Monitor = 1 << 8,
	//	SSSE3 = 1 << 9,
	//	L1_Context_ID = 1 << 10,
	//	Silicon_Debug_Interface = 1 << 11,
	//	FMA3 = 1 << 12,
	//	CMPXCHG16B = 1 << 13,
	//	xtpr = 1 << 14,
	//	pdcm = 1 << 15,
	//	pcid = 1 << 17,
	//	dca = 1 << 18,
	//	SSE4_1 = 1 << 19,
	//	SSE4_2 = 1 << 20,
	//	x2apic = 1 << 21,
	//	movbe = 1 << 22,
	//	popcnt = 1 << 23,
	//	tsc_deadline = 1 << 24,
	//	aes = 1 << 25,
	//	xsave = 1 << 26,
	//	osxsave = 1 << 27,
	//	AVX = 1 << 28,
	//	f16c = 1 << 29,
	//	rdrnd = 1 << 30,
	//	hypervisor = 1 << 31
	//};
	//enum Feature_Edx
	//{
	//	// EDX register
	//	fpu = 1,
	//	vme = 1 << 1,
	//	de = 1 << 2,
	//	pse = 1 << 3,
	//	tsc = 1 << 4,
	//	msr = 1 << 5,
	//	pae = 1 << 6,
	//	mce = 1 << 7,
	//	cx8 = 1 << 8,
	//	apic = 1 << 9,
	//	sep = 1 << 11,
	//	mtrr = 1 << 12,
	//	pge = 1 << 13,
	//	mca = 1 << 14,
	//	cmov = 1 << 15,
	//	pat = 1 << 16,
	//	pse_36 = 1 << 17,
	//	psn = 1 << 18,
	//	clfsh = 1 << 19,
	//	ds = 1 << 21,
	//	acpi = 1 << 22,
	//	MMX = 1 << 23,
	//	fxsr = 1 << 24,
	//	SSE = 1 << 25,
	//	SSE2 = 1 << 26,
	//	ss = 1 << 27,
	//	Hyper_threading = 1 << 28,
	//	tm = 1 << 29,
	//	ia64 = 1 << 30,
	//	pbe = 1 << 31

	//};
	//struct Reg
	//{
	//	union
	//	{
	//		int iReg[4];
	//		struct
	//		{
	//			int eax, ebx, ecx, edx;
	//		};
	//	};
	//};
	//Reg temp;
	//__cpuid(temp.iReg, 1);

	//Feature_Ecx ecx; 
	//Feature_Edx edx;

	//ecx = (Feature_Ecx)temp.ecx;
	//edx = (Feature_Edx)temp.edx;

	//pSurf = AlignedPtr<D3DCOLOR>(64);
	numBalls = 2000;
	pos = new Vec2f[numBalls];
	vel = new Vec2f[numBalls];
	acc = new Vec2f[numBalls];

	std::default_random_engine rnd;

	for (int i = 0; i < numBalls; ++i)
	{
		std::uniform_real_distribution<float> xRand(0.0f, (float)win->Width());
		std::uniform_real_distribution<float> yRand(0.0f, (float)win->Height());

		pos[i] = Vec2f(xRand(rnd), yRand(rnd));
		vel[i] = Vec2f();
		acc[i] = Vec2f();
	}


	// Create surface
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
			D3DCOLOR c = D3DCOLOR_ARGB(alpha, 255, 255, 255);

			if (sqrDist <= radius * radius)
			{
				float dist = sqrtf((float)sqrDist);
				UINT iDist = (UINT)((dist * invRad * 255.0f) + 0.5f);
				alpha = 255 - iDist;
				c = D3DCOLOR_ARGB(alpha, 255, 255, 255);
			}

			pSurf[index] = c;
		}
	}
	// Done creating surface

	Vec2SSE a(1.0f, 2.0f), b(3.0f);
	Vec2SSE c(5.0f, 10.0f);

	c.v = MirrorXY(c.v);
	int at = 0;

}

Game::~Game()
{
}

void Game::Go()
{	
	gfx->BeginFrame();
	UpdateTime();
	ComposeFrame();
	gfx->EndFrame();
}

void Game::UpdateFrame()
{
	for (int i = 0; i < numBalls; i += 2)
	{
		Vector pi(pos[i].x, pos[i].y, pos[i + 1].x, pos[i + 1].y);
		Vector vi(vel[i].x, vel[i].y, vel[i + 1].x, vel[i + 1].y);
		Vector ai(acc[i].x, acc[i].y, acc[i + 1].x, acc[i + 1].y);
		Vec2f temp[2];

		for (int j = i + 1; j < numBalls - 1; j += 2)
		{
			Vector pj(pos[j].x, pos[j].y, pos[j + 1].x, pos[j + 1].y);
			Vector vj(vel[j].x, vel[j].y, vel[j + 1].x, vel[j + 1].y);
			Vector aj(acc[j].x, acc[j].y, acc[j + 1].x, acc[j + 1].y);

			Vector delta(pj - pi);
			Vector deltaSqr(delta.MultiDot2(delta));
			Vector normal(delta.MultiNormalize2());
			Vector force((Vector(5.0f) / deltaSqr).Min(Vector(0.06f)));
			Vector accel(normal * force);

			ai += accel;
			aj -= accel;
			_mm_storeu_ps((float*)temp, aj.vector);
			acc[j] = temp[0];
			acc[j + 1] = temp[1];
		}

		_mm_storeu_ps((float*)temp, ai.vector);

		acc[i] = temp[0];
		acc[i + 1] = temp[1];

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

	int diam = radius * 2;

	for (int i = 0; i < numBalls; ++i)
	{
		int px = (int)pos[i].x + 0.5f;
		int py = (int)pos[i].y + 0.5f;

		gfx->DrawSurfaceAlpha(px, py, diam, diam, pSurf);
	}
}

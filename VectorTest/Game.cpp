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

	//enum Intel_Feature_Ecx
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

	//enum Intel_Feature_Edx
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

	//Intel_Feature_Ecx ecx; 
	//Intel_Feature_Edx edx;

	//ecx = (Intel_Feature_Ecx)temp.ecx;
	//edx = (Intel_Feature_Edx)temp.edx;

	//pSurf = AlignedPtr<D3DCOLOR>(64);
	CreateSurface();
	InitBalls();

	Vec4SSE a(1.0f, 0.0f, 0.0f, 1.0f);
	Vec4SSE b(0.0f, 1.0f, 0.0f, 1.0f);
	Vec4SSE c = a.Cross(b);

	bool bSSE = cpuid.SupportsSSE();
	bool bSSE2 = cpuid.SupportsSSE2();
	bool bSSE3 = cpuid.SupportsSSE3();
	bool bSSSE3 = cpuid.SupposrtsSSSE3();
	bool bSSE4a = cpuid.SupportsSSE4a();
	bool bSSE41 = cpuid.SupportsSSE41();
	bool bSSE42 = cpuid.SupportsSSE42();
	bool bFMA3 = cpuid.SupportsFMA3();
	bool bFMA4 = cpuid.SupportsFMA4();
	bool bAVX = cpuid.SupportsAVX();


	int ia = 0;
}

Game::~Game()
{
}

void Game::Go()
{
	gfx->BeginFrame();
	UpdateTime();
	UpdateFrameSSE();
	ComposeFrame();
	gfx->EndFrame();
}

void Game::InitBalls()
{
	numBalls = 2000;
	pos.reset(new Vector2[numBalls]);
	vel.reset(new Vector2[numBalls]);
	acc.reset(new Vector2[numBalls]);

	vPos.reset(new Vec2f[numBalls]);
	vVel.reset(new Vec2f[numBalls]);
	vAcc.reset(new Vec2f[numBalls]);

	std::default_random_engine rnd;

	for (int i = 0; i < numBalls; ++i)
	{
		std::uniform_real_distribution<float> xRand(0.0f, 200.0f);
		std::uniform_real_distribution<float> yRand(0.0f, 200.0f);
		float theta = (float)i / 180.0f * 3.14159f;
		float rx = xRand(rnd) * cos(theta);
		float ry = yRand(rnd) * sin(theta);
		float x = win->Width() / 6;
		float y = win->Height() / 2;
		if (i % 2 == 0)
		{
			pos[i].x = x + rx;
			pos[i].y = y + ry;
			vel[i].x = 1.0f * cos(theta);
			vel[i].y = 1.0f * sin(theta);
		}
		else
		{
			pos[i].x = (x * 5.0f) + rx;
			pos[i].y = y + ry;
			vel[i].x = -1.0f * cos(theta);
			vel[i].y = -1.0f * sin(theta);
		}
		acc[i].x = acc[i].y = 0.0f;

		vPos[i] = Vec2f(yRand(rnd) + (win->Width() / 2), yRand(rnd) + (win->Height() / 2));
		vVel[i] = vAcc[i] = Vec2f();
	}

	g = 1.0f;
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

void Game::UpdateFrame()
{
	for (int i = 0; i < numBalls; ++i)
	{		
		for (int j = i + 1; j < numBalls; ++j)
		{			
			Vec2f delta(vPos[j] - vPos[i]);
			float deltaSqr(delta.LengthSquared());
			Vec2f normal(delta.Normalize());
			float force(min((g / deltaSqr), 0.06f));
			Vec2f accel(normal * force);

			vAcc[i] += accel;
			vAcc[j] -= accel;
		}

		vVel[i] += vAcc[i];
		vPos[i] += vVel[i];
		vAcc[i] = Vec2f();
	}
}

void Game::UpdateFrameSSE()
{	
	Vec2SSE z(ZeroPS);

	for (int i = 0; i < numBalls; ++i)
	{
		Vec2SSE pi(pos[i]);
		Vec2SSE vi(vel[i]);
		Vec2SSE ai(acc[i]);

		for (int j = i + 1; j < numBalls; ++j)
		{
			Vec2SSE pj(pos[j]);
			Vec2SSE vj(vel[j]);
			Vec2SSE aj(acc[j]);

			Vec2SSE delta(pj - pi);
			Vec2SSE deltaSqr(delta.LengthSquare());
			Vec2SSE dist(SSE_Utils::Float4_Utils::RecipSqrRoot(deltaSqr.v));
			Vec2SSE normal(delta * dist);
			Vec2SSE force(SSE_Utils::Float4_Utils::Min((g / deltaSqr).v, Vec2SSE(0.06f).v));
			Vec2SSE accel(normal * force);

			ai += accel;
			aj -= accel;
			acc[j] = aj.StoreFloat();
		}

		vi += ai;
		pi += vi;

		vel[i] = vi.StoreFloat();
		pos[i] = pi.StoreFloat();		
		acc[i] = z.StoreFloat();
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

		if (px < 0 || px + diam >= win->Width() ||
			py < 0 || py + diam >= win->Height())
		{
			continue;
		}
		gfx->DrawSurfaceAlpha(px, py, diam, diam, pSurf);		
	}

	/*for (int i = 0; i < numBalls; ++i)
	{
	int px = (int)vPos[i].x + 0.5f;
	int py = (int)vPos[i].y + 0.5f;

	if (px < 0 || px + diam >= win->Width() ||
	py < 0 || py + diam >= win->Height())
	{
	continue;
	}
	gfx->DrawSurfaceAlpha(px, py, diam, diam, pSurf);
	}*/

}

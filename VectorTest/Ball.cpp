#include "Ball.h"

using namespace SSE_Utils::Float4_Utils;

Ball::Ball()
{
}

Ball::Ball(std::default_random_engine &Rnd, int Index, std::shared_ptr<Window> &Win,
	D3DCOLOR *Surface)
	:
	win(Win),
	myIndex(Index),
	pSurf(Surface),
	diam(16),
	g(4.0f),
	maxAccel(0.06f)
{
	std::uniform_real_distribution<float> xRand(100.0f, 300.0f);
	std::uniform_real_distribution<float> yRand(100.0f, 300.0f);
	
	float theta = (float)myIndex / 180.0f * 3.14159f;
	float rx = xRand(Rnd) * cos(theta);
	float ry = yRand(Rnd) * sin(theta);
	float x = win->Width() / 6;
	float y = win->Height() / 2;

	float initVel = 0.250f;
	if (myIndex % 2 == 0)
	{
		pos = { x + rx, y + ry };
		vel = { initVel };
	}
	else
	{
		pos = { x * 5.0f + rx, y + ry };
		vel = { -initVel };
	}
	acc = { ZeroPS };
}

void Ball::Update(Ball *BallJ, UINT NumBalls, float dt)
{
	Vec2SSE z(ZeroPS);
	Vec2SSE vMin(0.06f);

	Vec2SSE vDt((dt) * g);
	for (int j = myIndex + 1; j < NumBalls; ++j)
	{
		Vec2SSE delta = BallJ[j].pos - pos;
		Vec2SSE deltaSqr = delta.LengthSquare();
		Vec2SSE dist = RecipSqrRoot(deltaSqr.v);
		Vec2SSE normal = delta * dist;
		Vec2SSE invSqrDist = Recip(deltaSqr.v);
		Vec2SSE force = Min(invSqrDist.v * vDt.v, vMin.v);
		Vec2SSE accel(normal * force);
		acc += accel;
		BallJ[j].acc -= accel;
	}

	vel += acc;
	pos += vel;
	acc = { ZeroPS };

}

void Ball::Draw(D3DGraphics &gfx)
{
	int px = (int)pos.X() + 0.5f;
	int py = (int)pos.Y() + 0.5f;

	if (px < 0 || px + diam >= win->Width() ||
		py < 0 || py + diam >= win->Height())
	{
		return;
	}
	gfx.DrawSurfaceAlphaUtil(px, py, diam, diam, pSurf);
}

Ball::~Ball()
{
}

#pragma once
#include <random>
#include "Vec2SSE.h"
#include "D3DGraphics.h"

class Ball
{
public:
	Ball();
	Ball(std::default_random_engine &Rnd, int Index, std::shared_ptr<Window> &Win, 
		D3DCOLOR *Surface);
	~Ball();

	void Update(Ball *BallJ, UINT NumBalls, float dt);
	void Draw(D3DGraphics &gfx);
private:
	Vec2SSE pos, vel, acc;
	float g, maxAccel;
	int myIndex, diam;
	D3DCOLOR *pSurf;
	std::shared_ptr<Window> win;
};


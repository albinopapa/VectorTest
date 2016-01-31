#pragma once
#include "Animation.h"

class Animate
{
public:
	Animate();
	~Animate();

	void Update(Animation &Anim, const float TimeStep);
};


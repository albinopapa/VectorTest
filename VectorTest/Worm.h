#pragma once
#include "Slink.h"
class Worm
{
public:
	Worm();
	~Worm();

	void Update(const float TimeStep);
private:
	Slink slink;
	Skeleton rig;
	Vec4 pos;
};


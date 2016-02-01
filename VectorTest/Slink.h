#pragma once
#include "Animation.h"
#include <vector>

class Slink :public Animation
{
public:
	Slink();
	~Slink();

	
	void Update(Skeleton &Skelly, const float TimeStep)override;

private:
	unsigned int frame;
	float rate;
	std::vector<Quaternion> joints;
};


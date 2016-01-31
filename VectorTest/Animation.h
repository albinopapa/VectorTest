#pragma once
#include "Skeleton.h"

class Animation
{
public:
	Animation();
	virtual~Animation();

	
	virtual void Update(Skeleton &Skelly, const float TimeStep)abstract;

};


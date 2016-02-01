#include "Slink.h"


Slink::Slink()
{
	for (int i = 0; i < 6; ++i)
	{
		joints.emplace_back(Quaternion());
		
	}
}

void Slink::Update(Skeleton &Skelly, const float TimeStep)
{
	Skelly.Bend(1, -0.2f);
	Skelly.Bend(2, -0.4f);
	Skelly.Bend(3, -0.2f);
	Skelly.Bend(4, 0.0f);
	Skelly.Bend(5, 0.2f);
}

Slink::~Slink()
{
}

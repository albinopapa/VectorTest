#include "Worm.h"


Worm::Worm()
{
	unsigned int bid = 0;
	Vec4 offset(16.0f, 0.0f, 0.0f, 0.0f);
	pos = { 640.0f, 400.0f, 0.0f, 0.0f };
	rig = Skeleton(pos);
	const float angle = 0.0f;

	Vec4 segment = pos + offset;
	rig.AddBone(segment, ++bid, angle);

	segment += offset;
	rig.AddBone(segment, ++bid, angle);

	segment += offset;
	rig.AddBone(segment, ++bid, angle);
}

void Worm::Update(const float TimeStep)
{
	if (TimeStep == 0.67f)
	{
		slink.Update(rig, TimeStep);
	}
}

Worm::~Worm()
{
}

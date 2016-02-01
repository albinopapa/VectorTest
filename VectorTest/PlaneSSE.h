#pragma once
#include "Matrix4x4SSE.h"

class PlaneSSE
{
public:
	PlaneSSE();
	PlaneSSE(const Vec4SSE &V);

	Vec4SSE Dot(const Vec4SSE &V);
	Vec4SSE DotCoord(const Vec4SSE &V);
	Vec4SSE DotNormal(const Vec4SSE &V);
	Vec4SSE Normalize();
	Vec4SSE FastNormalize();

	// Builders
	PlaneSSE &FromPointNormal(const Vec4SSE &Point, const Vec4SSE &Normal);
	PlaneSSE &FromPoints(const Vec4SSE &P0, const Vec4SSE &P1, const Vec4SSE &P2);
	PlaneSSE &Transform(const Matrix4x4SSE &M);
	Vec4SSE IntersectsLine(const Vec4SSE &LineStart, const Vec4SSE &LineEnd);
	void IntersectsPlane(const PlaneSSE &OtherPlane, Vec4SSE &LineStart, Vec4SSE &LineEnd);

	bool operator==(const PlaneSSE &P);
	bool IsInfinite();
	bool IsNaN();
	bool IsNearEqual(const PlaneSSE &P, const Vec4SSE &Epsilon);
	bool operator!=(const PlaneSSE &P);


	Vec4SSE coEfficients;
};


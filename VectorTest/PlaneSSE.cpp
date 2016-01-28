#include "PlaneSSE.h"

using namespace DirectX;
PlaneSSE::PlaneSSE()
{
}

PlaneSSE::PlaneSSE(const Vec4SSE &V)
	:
	coEfficients(V.v)
{}

Vec4SSE PlaneSSE::Dot(const Vec4SSE &V)
{
	return{ XMPlaneDot(coEfficients.v, V.v) };
}

Vec4SSE PlaneSSE::DotCoord(const Vec4SSE &V)
{
	return{ XMPlaneDotCoord(coEfficients.v, V.v) };
}

Vec4SSE PlaneSSE::DotNormal(const Vec4SSE &V)
{
	return{ XMPlaneDotNormal(coEfficients.v, V.v) };
}

Vec4SSE PlaneSSE::Normalize()
{
	return{ XMPlaneNormalize(coEfficients.v) };
}

Vec4SSE PlaneSSE::FastNormalize()
{
	return{ XMPlaneNormalizeEst(coEfficients.v) };
}

PlaneSSE &PlaneSSE::FromPointNormal(const Vec4SSE &Point, const Vec4SSE &Normal)
{
	coEfficients.v = XMPlaneFromPointNormal(Point.v, Normal.v);
	return *this;
}

PlaneSSE &PlaneSSE::FromPoints(const Vec4SSE &P0, const Vec4SSE &P1, const Vec4SSE &P2)
{
	coEfficients.v = XMPlaneFromPoints(P0.v, P1.v, P2.v);
	return *this;
}

PlaneSSE &PlaneSSE::Transform(const Matrix4x4SSE &M)
{
	coEfficients.v = XMPlaneTransform(coEfficients.v, M);
	return *this;
}

Vec4SSE PlaneSSE::IntersectsLine(const Vec4SSE &LineStart, const Vec4SSE &LineEnd)
{
	return{ XMPlaneIntersectLine(coEfficients.v, LineStart.v, LineEnd.v) };
}

void PlaneSSE::IntersectsPlane(const PlaneSSE &OtherPlane, Vec4SSE &LineStart, Vec4SSE &LineEnd)
{
	XMPlaneIntersectPlane(&LineStart.v, &LineEnd.v, coEfficients.v, 
		OtherPlane.coEfficients.v);
}

bool PlaneSSE::operator==(const PlaneSSE &P)
{
	return XMPlaneEqual(coEfficients.v, P.coEfficients.v);
}

bool PlaneSSE::IsInfinite()
{
	return XMPlaneIsInfinite(coEfficients.v);
}

bool PlaneSSE::IsNaN()
{
	return XMPlaneIsNaN(coEfficients.v);
}

bool PlaneSSE::IsNearEqual(const PlaneSSE &P, const Vec4SSE &Epsilon)
{
	return XMPlaneNearEqual(coEfficients.v, P.coEfficients.v, Epsilon.v);
}

bool PlaneSSE::operator!=(const PlaneSSE &P)
{
	return XMPlaneNotEqual(coEfficients.v, P.coEfficients.v);
}



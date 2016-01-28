#include "QuaternionSSE.h"
#include "Matrix4x4SSE.h"

using namespace DirectX;
namespace Utils = SSE_Utils::Float4_Utils;
QuaternionSSE::QuaternionSSE()
{
	Identity();
}

QuaternionSSE::QuaternionSSE(const Vec4SSE &V)
	:
	angle(V)
{

}

QuaternionSSE QuaternionSSE::Conjugate()
{
	QuaternionSSE t;
	t.angle.v = XMQuaternionConjugate(angle.v);
	return t;
}

QuaternionSSE QuaternionSSE::operator*(const QuaternionSSE &Q)
{
	QuaternionSSE t;
	t.angle = { XMQuaternionMultiply(angle.v, Q.angle.v) };
	return t;
}

QuaternionSSE &QuaternionSSE::operator*=(const QuaternionSSE &Q)
{
	*this = (*this) * Q;
	return *this;
}

Vec4SSE QuaternionSSE::Dot(const QuaternionSSE &Q)
{
	return{ XMQuaternionDot(angle.v, Q.angle.v) };
}

Vec4SSE QuaternionSSE::Exponential()
{
	return{ XMQuaternionExp(angle.v) };
}

Vec4SSE QuaternionSSE::Inverse()
{
	return{ XMQuaternionInverse(angle.v) };
}

Vec4SSE QuaternionSSE::InverseLength()
{
	return{ XMQuaternionReciprocalLength(angle.v) };
}

Vec4SSE QuaternionSSE::Length()
{
	return{ XMQuaternionLength(angle.v) };
}

Vec4SSE QuaternionSSE::LengthSqr()
{
	return{ XMQuaternionLengthSq(angle.v) };
}

Vec4SSE QuaternionSSE::NaturalLog()
{
	return{ XMQuaternionLn(angle.v) };
}

QuaternionSSE QuaternionSSE::Normalize()
{
	return{ { XMQuaternionNormalize(angle.v) } };
}

QuaternionSSE QuaternionSSE::FastNormalize()
{
	return{ { XMQuaternionNormalizeEst(angle.v) } };
}

QuaternionSSE QuaternionSSE::Slerp(const QuaternionSSE &Q, const float T)
{
	return{ { XMQuaternionSlerp(angle.v, Q.angle.v, T) } };
}

QuaternionSSE QuaternionSSE::SlerpQuad(const QuaternionSSE &Q0, const QuaternionSSE &Q1,
	const QuaternionSSE &Q2, const QuaternionSSE &Q3, const float T)
{
	return{ { XMQuaternionSquad(Q0.angle.v, Q1.angle.v, Q2.angle.v, Q3.angle.v, T) } };
}

void QuaternionSSE::SlerpQuadSetup(QuaternionSSE &rQ0, QuaternionSSE &rQ1, QuaternionSSE &rQ2,
	const QuaternionSSE &Q0, const QuaternionSSE &Q1,
	const QuaternionSSE &Q2, const QuaternionSSE &Q3)
{
	XMQuaternionSquadSetup(&rQ0.angle.v, &rQ1.angle.v, &rQ2.angle.v, Q0.angle.v,
		Q1.angle.v, Q2.angle.v, Q3.angle.v);
}

void QuaternionSSE::GetAxisAndAngle(Vec4SSE &Axis, float &Angle)
{
	XMQuaternionToAxisAngle(&Axis.v, &Angle, angle.v);
}

// Builders
QuaternionSSE &QuaternionSSE::BaryCentric(const QuaternionSSE &Q0, const QuaternionSSE &Q1,
	const QuaternionSSE &Q2, const float F, const float G)
{
	angle.v = XMQuaternionBaryCentric(Q0.angle.v, Q1.angle.v, Q2.angle.v, F, G);
	return (*this);
}

QuaternionSSE &QuaternionSSE::Identity()
{
	angle.v = XMQuaternionIdentity();
	return *this;
}

QuaternionSSE &QuaternionSSE::RotateAxis(const Vec4SSE &Axis, const float Angle)
{
	angle.v = XMQuaternionRotationAxis(Axis.v, Angle);
	return *this;
}

QuaternionSSE &QuaternionSSE::RotationMatrix(const Matrix4x4SSE &M)
{
	angle.v = XMQuaternionRotationMatrix(M);
	return *this;
}

QuaternionSSE &QuaternionSSE::RotationNormal(const Vec4SSE &NormalAxis, const float Angle)
{
	angle.v = XMQuaternionRotationNormal(NormalAxis.v, Angle);
	return *this;
}

QuaternionSSE &QuaternionSSE::RotationFromVector(const Vec4SSE &Angles)
{
	angle.v = XMQuaternionRotationRollPitchYawFromVector(Angles.v);
	return *this;
}

// Comparisons
bool QuaternionSSE::operator==(const QuaternionSSE &Q)
{
	return XMQuaternionEqual(angle.v, Q.angle.v);
}

bool QuaternionSSE::operator!=(const QuaternionSSE &Q)
{
	return XMQuaternionNotEqual(angle.v, Q.angle.v);
}

bool QuaternionSSE::IsIdentity()
{
	return XMQuaternionIsIdentity(angle.v);
}

bool QuaternionSSE::IsInfinite()
{
	return XMQuaternionIsInfinite(angle.v);
}

bool QuaternionSSE::IsNan()
{
	return XMQuaternionIsNaN(angle.v);
}
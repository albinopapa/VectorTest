#pragma once
#include "Vec4SSE.h"
#include <DirectXMath.h>

// Forward declaration.  Matrix4x4SSE uses QuaternionSSE also
class Matrix4x4SSE;

class QuaternionSSE
{
public:
	QuaternionSSE();
	QuaternionSSE(const Vec4SSE &V);
	
	QuaternionSSE operator*(const QuaternionSSE &Q);
	QuaternionSSE &operator*=(const QuaternionSSE &Q);

	QuaternionSSE Conjugate();
	Vec4SSE Dot(const QuaternionSSE &Q);
	Vec4SSE Exponential();
	Vec4SSE Inverse();
	Vec4SSE InverseLength();
	Vec4SSE Length();
	Vec4SSE LengthSqr();
	Vec4SSE NaturalLog();
	QuaternionSSE Normalize();
	QuaternionSSE FastNormalize();
	QuaternionSSE Slerp(const QuaternionSSE &Q, const float T);
	QuaternionSSE SlerpQuad(const QuaternionSSE &Q0, const QuaternionSSE &Q1,
		const QuaternionSSE &Q2, const QuaternionSSE &Q3, const float T);
	void SlerpQuadSetup(QuaternionSSE &rQ0, QuaternionSSE &rQ1, QuaternionSSE &rQ2,
		const QuaternionSSE &Q0, const QuaternionSSE &Q1,
		const QuaternionSSE &Q2, const QuaternionSSE &Q3);
	void GetAxisAndAngle(Vec4SSE &Axis, float &Angle);

	// Builders
	QuaternionSSE &BaryCentric(const QuaternionSSE &Q0, const QuaternionSSE &Q1,
		const QuaternionSSE &Q2, const float F, const float G);
	QuaternionSSE &Identity();
	QuaternionSSE &RotateAxis(const Vec4SSE &Axis, const float Angle);
	QuaternionSSE &RotationMatrix(const Matrix4x4SSE &M);
	QuaternionSSE &RotationNormal(const Vec4SSE &NormalAxis, const float Angle);
	QuaternionSSE &RotationFromVector(const Vec4SSE &Angles);
	
	// Comparisons
	bool operator==(const QuaternionSSE &Q);
	bool operator!=(const QuaternionSSE &Q);
	bool IsIdentity();
	bool IsInfinite();
	bool IsNan();

	Vec4SSE angle;
};


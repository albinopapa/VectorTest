#pragma once
#include <immintrin.h>

typedef __m128 Float4;


inline Float4 operator+(const Float4 &V0, const Float4 &V1)
{
	return _mm_add_ps(V0, V1);
}

inline Float4 operator-(const Float4 &V0, const Float4 &V1)
{
	return _mm_sub_ps(V0, V1);
}

inline Float4 operator*(const Float4 &V0, const float &S)
{
	return _mm_mul_ps(V0, _mm_set1_ps(S));
}

inline Float4 operator/(const Float4 &V0, const float S)
{
	return _mm_mul_ps(V0, _mm_rcp_ps(_mm_set1_ps(S)));
}

// Assumes vector is all one component (x0, x1, x2, x3)
inline Float4 Dot(const Float4 &Vx0, const Float4 &Vy0, const Float4 &Vx1, const Float4 &Vy1)
{
	Float4 rx = _mm_mul_ps(Vx0, Vx1);
	Float4 ry = _mm_mul_ps(Vy0, Vy1);
	Float4 r = _mm_add_ps(rx, ry);
	return r;
}

// Assumes vector is a vector4 (x,y,z,w)
inline Float4 Dot4(const Float4 &V0, const Float4 &V1)
{
	Float4 rx = _mm_mul_ps(V0, V1);
	Float4 ry = _mm_shuffle_ps(rx, rx, _MM_SHUFFLE(0, 1, 2, 3));
	rx = _mm_add_ps(rx, ry);
	ry = _mm_shuffle_ps(rx, rx, _MM_SHUFFLE(0, 1, 2, 3));	
	Float4 r = _mm_add_ps(rx, ry);
	return r;
}

#pragma once
#include <immintrin.h>
#include <math.h>

class Vec2f
{
public:
	Vec2f();
	Vec2f(float X, float Y);
	~Vec2f();

	Vec2f operator-(const Vec2f &V)const;
	Vec2f operator+(const Vec2f &V)const;
	float operator*(const Vec2f &V)const;
	Vec2f operator*(const float S)const;
	Vec2f operator/(const float S)const;
	Vec2f &operator-=(const Vec2f &V);
	Vec2f &operator+=(const Vec2f &V);

	float Length()const;
	Vec2f Normalize()const;

	float x, y;
};

_MM_ALIGN16 
class Vec2sse
{
public:
	Vec2sse();
	Vec2sse(float X, float Y);
	Vec2sse(const __m128 &V);
	Vec2sse(const Vec2sse &V);
	~Vec2sse();

	Vec2sse operator - (const Vec2sse &V)const;
	Vec2sse operator+(const Vec2sse &V)const;
	Vec2sse operator*(const Vec2sse &V)const;
	Vec2sse operator*(const float &S)const;
	Vec2sse operator/(const float &S)const;
	Vec2sse &operator-=(const Vec2sse &V);
	Vec2sse &operator+=(const Vec2sse &V);

	Vec2sse LenSqr()const;
	Vec2sse InvLength()const;
	Vec2sse Length()const;
	Vec2sse Normalize()const;

	__m128 v;
};

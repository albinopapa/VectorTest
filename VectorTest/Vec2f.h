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

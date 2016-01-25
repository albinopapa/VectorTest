#include "Vec2f.h"

Vec2f::Vec2f()
	:
	x(0.0f),
	y(0.0f)
{
}

Vec2f::Vec2f(float X, float Y)
	:
	x(X),
	y(Y)
{

}

Vec2f::~Vec2f()
{
}

Vec2f Vec2f::operator-(const Vec2f &V)const
{
	return{ x - V.x, y - V.y };
}

Vec2f Vec2f::operator+(const Vec2f &V)const
{
	return{ x + V.x, y + V.y };
}

float Vec2f::operator*(const Vec2f &V)const
{
	return ((x * V.x) + (y * V.y));
}

Vec2f Vec2f::operator*(float S)const
{
	return{ x * S, y * S };
}

Vec2f Vec2f::operator/(const float S)const
{
	return{ (*this) * (1.0f / S) };
}

Vec2f &Vec2f::operator-=(const Vec2f &V)
{
	(*this) = (*this) - V;
	return (*this);
}

Vec2f &Vec2f::operator+=(const Vec2f &V)
{
	(*this) = (*this) + V;
	return (*this);
}

float Vec2f::Dot(const Vec2f &V)const
{
	return((x * x) + (y * y));
}

float Vec2f::LengthSquared()const
{
	return Dot(*this);
}

float Vec2f::Length()const
{
	return sqrtf(LengthSquared());
}

Vec2f Vec2f::Normalize()const
{
	const float invDist = 1.0f / Length();
	return{ (*this) * invDist };
}


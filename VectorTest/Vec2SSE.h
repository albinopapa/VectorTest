#pragma once

#include "SSE_Utils.h"


/*
The Vec2SSE class is a 2 component vector class.  Only the low 64 bits are used
in calculations while the upper 64 bits are discarded and set to 0.0f.  Therefore,
it is not recommended to try to pack two 2 component vectors.  There is/will be a 
separate class for that.
*/

Align16
class Vec2SSE
{
public:
	Vec2SSE();
	Vec2SSE(float S);
	Vec2SSE(float X, float Y);
	Vec2SSE(const FLOAT4 &V);
	Vec2SSE(const Vector2 &V);
	Vec2SSE(const Vec2SSE &V);

	// Logical operators
	Vec2SSE _vectorcall operator&(const Vec2SSE &V)const;
	Vec2SSE _vectorcall operator|(const Vec2SSE &V)const;

	// Logical function
	// same as C = (!A) & B;
	Vec2SSE _vectorcall AndNot(const Vec2SSE &V);

	// Arithmetic operators
	Vec2SSE _vectorcall operator-()const;
	Vec2SSE _vectorcall operator+(const Vec2SSE &V)const;
	Vec2SSE _vectorcall operator-(const Vec2SSE &V)const;
	Vec2SSE _vectorcall operator*(const float S)const;
	Vec2SSE _vectorcall operator*(const Vec2SSE &V)const;
	Vec2SSE _vectorcall operator/(const float S)const;
	Vec2SSE _vectorcall operator/(const Vec2SSE &V)const;

	// Arithmetic plus assignment operators
	Vec2SSE & _vectorcall operator+=(const Vec2SSE &V);
	Vec2SSE & _vectorcall operator-=(const Vec2SSE &V);
	Vec2SSE & _vectorcall operator*=(const Vec2SSE &V);
	Vec2SSE & _vectorcall operator/=(const Vec2SSE &V);

	// return V1 + ((*this) * V0);
	Vec2SSE _vectorcall MultiplyAdd(const Vec2SSE &V0, const Vec2SSE &V1);
	Vec2SSE _vectorcall Dot(const Vec2SSE &V)const;
	Vec2SSE _vectorcall Cross(const Vec2SSE &V)const;
	Vec2SSE _vectorcall Length()const;
	Vec2SSE _vectorcall LengthSquare()const;
	Vec2SSE _vectorcall InverseLength()const;
	Vec2SSE Normalize();

	Vec2SSE SplatX()const;
	Vec2SSE SplatY()const;

	Vector2 StoreFloat()const;
	Vector2 StoreInt()const;
	Vector2 StoreIntCast();

	float X()const;
	float Y()const;


	FLOAT4 v;
};


inline Vec2SSE _vectorcall operator/(const float S, const Vec2SSE &V)
{
	return{ _mm_div_ps(SSE_Utils::Float4_Utils::LoadFloat(S), V.v) };
}
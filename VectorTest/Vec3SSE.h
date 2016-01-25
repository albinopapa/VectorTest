#pragma once
#include "SSE_Utils.h"

class Vec3SSE
{
public:
	Vec3SSE();
	Vec3SSE(float S);
	Vec3SSE(float X, float Y, float Z);
	Vec3SSE(const FLOAT4 &V);
	Vec3SSE(const Vector3 &V);
	Vec3SSE(const Vec3SSE &V);

	// Logical operators
	Vec3SSE _vectorcall operator&(const Vec3SSE &V)const;
	Vec3SSE _vectorcall operator|(const Vec3SSE &V)const;

	// Logical function
	// same as C = (!A) & B;
	Vec3SSE _vectorcall AndNot(const Vec3SSE &V);

	// Arithmetic operators
	Vec3SSE _vectorcall operator-()const;
	Vec3SSE _vectorcall operator+(const Vec3SSE &V)const;
	Vec3SSE _vectorcall operator-(const Vec3SSE &V)const;
	Vec3SSE _vectorcall operator*(const float S)const;
	Vec3SSE _vectorcall operator*(const Vec3SSE &V)const;
	Vec3SSE _vectorcall operator/(const float S)const;
	Vec3SSE _vectorcall operator/(const Vec3SSE &V)const;

	// Arithmetic plus assignment operators
	Vec3SSE & _vectorcall operator+=(const Vec3SSE &V);
	Vec3SSE & _vectorcall operator-=(const Vec3SSE &V);
	Vec3SSE & _vectorcall operator*=(const Vec3SSE &V);
	Vec3SSE & _vectorcall operator/=(const Vec3SSE &V);

	// return V1 + ((*this) * V0);
	Vec3SSE _vectorcall MultiplyAdd(const Vec3SSE &V0, const Vec3SSE &V1);
	Vec3SSE _vectorcall Dot(const Vec3SSE &V)const;
	Vec3SSE _vectorcall Cross(const Vec3SSE &V)const;
	Vec3SSE _vectorcall Length()const;
	Vec3SSE _vectorcall LengthSquare()const;
	Vec3SSE _vectorcall InverseLength()const;
	Vec3SSE Normalize();

	Vec3SSE SplatX()const;
	Vec3SSE SplatY()const;
	Vec3SSE SplatZ()const;

	Vector3 StoreFloat()const;
	Vector3 StoreInt()const;
	Vector3 StoreIntCast();

	float X()const;
	float Y()const;
	float Z()const;


	FLOAT4 v;
};


inline Vec3SSE _vectorcall operator/(const float S, const Vec3SSE &V)
{
	return{ _mm_div_ps(SSE_Utils::Float4_Utils::LoadFloat(S), V.v) };
}


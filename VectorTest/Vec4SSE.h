#pragma once
#include "SSE_Utils.h"

class Vec4SSE
{
public:
	Vec4SSE();
	Vec4SSE(float S);
	Vec4SSE(float X, float Y, float Z, float W);
	Vec4SSE(const FLOAT4 &V);
	Vec4SSE(const Vector4 &V);
	Vec4SSE(const Vec4SSE &V);

	// Logical operators
	Vec4SSE _vectorcall operator&(const Vec4SSE &V)const;
	Vec4SSE _vectorcall operator|(const Vec4SSE &V)const;

	// Logical function
	// same as C = (!A) & B;
	Vec4SSE _vectorcall AndNot(const Vec4SSE &V);

	// Arithmetic operators
	Vec4SSE _vectorcall operator-()const;
	Vec4SSE _vectorcall operator+(const Vec4SSE &V)const;
	Vec4SSE _vectorcall operator-(const Vec4SSE &V)const;
	Vec4SSE _vectorcall operator*(const float S)const;
	Vec4SSE _vectorcall operator*(const Vec4SSE &V)const;
	Vec4SSE _vectorcall operator/(const float S)const;
	Vec4SSE _vectorcall operator/(const Vec4SSE &V)const;

	// Arithmetic plus assignment operators
	Vec4SSE & _vectorcall operator+=(const Vec4SSE &V);
	Vec4SSE & _vectorcall operator-=(const Vec4SSE &V);
	Vec4SSE & _vectorcall operator*=(const Vec4SSE &V);
	Vec4SSE & _vectorcall operator/=(const Vec4SSE &V);

	// return V1 + ((*this) * V0);
	Vec4SSE _vectorcall MultiplyAdd(const Vec4SSE &V0, const Vec4SSE &V1);
	Vec4SSE _vectorcall Dot(const Vec4SSE &V)const;
	Vec4SSE _vectorcall Cross(const Vec4SSE &V)const;
	Vec4SSE _vectorcall Length()const;
	Vec4SSE _vectorcall LengthSquare()const;
	Vec4SSE _vectorcall InverseLength()const;
	Vec4SSE Normalize();

	Vec4SSE SplatX()const;
	Vec4SSE SplatY()const;
	Vec4SSE SplatZ()const;
	Vec4SSE SplatW()const;

	Vector4 StoreFloat()const;
	Vector4 StoreInt()const;
	Vector4 StoreIntCast();

	float X()const;
	float Y()const;
	float Z()const;
	float W()const;


	FLOAT4 v;
};


inline Vec4SSE _vectorcall operator/(const float S, const Vec4SSE &V)
{
	return{ _mm_div_ps(SSE_Utils::Float4_Utils::LoadFloat(S), V.v) };
}


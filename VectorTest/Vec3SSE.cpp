#include "Vec3SSE.h"

using namespace SSE_Utils::Float4_Utils;

Vec3SSE::Vec3SSE()
	:
	v(ZeroPS)
{}

Vec3SSE::Vec3SSE(float S)
	:
	v(_mm_set1_ps(S))	// Set all elements to S (S, S, S, S)
{
	// Zero out the last element as it won't be used (v, v, v, 0.0f)
	//FLOAT4 t = _mm_shuffle_ps(v, _mm_setzero_ps(), _MM_SHUFFLE(0, 0, 2, 1));
	FLOAT4 t = Shuffle<Axy | Bxy>(ZeroPS, v);
	v = Shuffle<Axy | Bxz>(v, t);
}
Vec3SSE::Vec3SSE(float X, float Y, float Z)
	:
	v(_mm_set_ps(0.0f, Z, Y, X))		// SSE registers are setup backward
{}
Vec3SSE::Vec3SSE(const FLOAT4 &V)
	:
	v(V)
{}
Vec3SSE::Vec3SSE(const Vector3 &V)
	:
	v(_mm_set_ps(0.0f, V.z, V.y, V.x))
{}
Vec3SSE::Vec3SSE(const Vec3SSE &V)
	:
	v(V.v)
{}

Vec3SSE Vec3SSE::operator&(const Vec3SSE &V)const
{
	return{ v & V.v };
}
Vec3SSE Vec3SSE::operator|(const Vec3SSE &V)const
{
	return v | V.v;
}

Vec3SSE Vec3SSE::AndNot(const Vec3SSE &V)
{
	return{ SSE_Utils::Float4_Utils::AndNot(v, V.v) };
}

Vec3SSE Vec3SSE::operator-()const
{
	return{ -v };
}
Vec3SSE Vec3SSE::operator+(const Vec3SSE &V)const
{
	return v + V.v;
}
Vec3SSE Vec3SSE::operator-(const Vec3SSE &V)const
{
	return v - V.v;
}
Vec3SSE Vec3SSE::operator*(const float S)const
{
	return v * S;
}
Vec3SSE Vec3SSE::operator*(const Vec3SSE &V)const
{
	return v * V.v;
}
Vec3SSE Vec3SSE::operator/(const float S)const
{
	return v / S;
}
Vec3SSE Vec3SSE::operator/(const Vec3SSE &V)const
{
	return v / V.v;
}
Vec3SSE & Vec3SSE::operator+=(const Vec3SSE &V)
{
	v += V.v;
	return (*this);
}
Vec3SSE & Vec3SSE::operator-=(const Vec3SSE &V)
{
	v -= V.v;
	return (*this);
}
Vec3SSE & Vec3SSE::operator*=(const Vec3SSE &V)
{
	v *= V.v;
	return (*this);
}
Vec3SSE & Vec3SSE::operator/=(const Vec3SSE &V)
{
	v /= V.v;
	v = v & LoadFloat4(xyzMask);
	return (*this);
}

Vec3SSE Vec3SSE::MultiplyAdd(const Vec3SSE &V0, const Vec3SSE &V1)
{
	return V1.v + (v * V0.v);
}
Vec3SSE Vec3SSE::Dot(const Vec3SSE &V)const
{
	/*
	The dot product of a vector3 is X0*X1 + Y0*Y1 + Z0*Z1
	In SSE we can vertically multiply:
		X0 Y0 Z0
	*	X1 Y1 Z1
	------------------
		Xr Yr Zr
	In order to add the results together we have to shuffle the elements 
	YrXrZr
	then add them together
		 Xr  Yr  Zr
	+	 Yr  Xr  Zr
	------------------
		XYr YXr ZZr
	This means X and Y have the same values but Z has been doubled, so we 
	have to shuffle in the Zr from the previous operation and add it's value
	to the XYrYXr sum and copy XYr to the Zr position so that all 3 (X, Y and Z)
	all have the same result
		 XYr  YXr   Zr
	+	  Zr   Zr  XYr
	------------------
		XYZr YXZr ZXYr
	*/
	FLOAT4 t0 = v * V.v;

	FLOAT4 t1 = Shuffle<Ayx | Bzw>(t0, t0);
	t0 += t1;
	t0 = Shuffle<Axy | Bzw>(t0, t1);
	t1 = Shuffle<Azz | Bxw>(t0, t0);
	t0 += t1;
	
	return t0;
}

Vec3SSE Vec3SSE::Cross(const Vec3SSE &V)const
{
	FLOAT4 u0 = Shuffle<Ayz | Bxw>(v);
	FLOAT4 u1 = Shuffle<Azx | Byw>(v);
	FLOAT4 v0 = Shuffle<Ayz | Bxw>(V.v);
	FLOAT4 v1 = Shuffle<Azx | Byw>(V.v);

	FLOAT4 result = (u0 * v1) - (u1 * v0);

	return result;
}

Vec3SSE Vec3SSE::Length()const
{
	return{ SqrRoot(Dot(*this).v) };
}

Vec3SSE Vec3SSE::LengthSquare()const
{
	return Dot(*this);
}

Vec3SSE Vec3SSE::InverseLength()const
{
	FLOAT4 t0 = RecipSqrRoot(LengthSquare().v);
	
	// Zero W component	
	FLOAT4 t1 = Shuffle<Axy | Bxx>(t0, ZeroPS);
	t0 = Shuffle<Axy | Bxz>(t0, t1);
	return t0;
}

Vec3SSE Vec3SSE::Normalize()
{
	return ((*this) * InverseLength());
}

Vec3SSE Vec3SSE::SplatX()const
{
	return Shuffle<XXXX>(v);
}
Vec3SSE Vec3SSE::SplatY()const
{
	return Shuffle<YYYY>(v);
}
Vec3SSE Vec3SSE::SplatZ()const
{
	return Shuffle<ZZZZ>(v);
}

Vector3 Vec3SSE::StoreFloat()const
{
	Vector3 temp;
	temp.x = X();
	temp.y = Y();
	temp.z = Z();

	return temp;
}
Vector3 Vec3SSE::StoreInt()const
{
	Vector3 temp;
	temp.b = _mm_cvt_ss2si(v);
	temp.g = _mm_cvt_ss2si(Shuffle<Ayx>(v));
	temp.r = _mm_cvt_ss2si(Shuffle<Azx>(v));
	return temp;
}
Vector3 Vec3SSE::StoreIntCast()
{
	Vector3 temp;
	Vector4 t;
	_mm_store_si128((PDQWORD)&t, _mm_castps_si128(v));
	temp.ix = t.iX;
	temp.iy = t.iY;
	temp.iz = t.iZ;
	return temp;
}

float Vec3SSE::X()const
{
	return _mm_cvtss_f32(v);
}
float Vec3SSE::Y()const
{
	Vec3SSE t = SplatY();
	return _mm_cvtss_f32(t.v);
}
float Vec3SSE::Z()const
{
	Vec3SSE t = SplatZ();
	return _mm_cvtss_f32(t.v);
}
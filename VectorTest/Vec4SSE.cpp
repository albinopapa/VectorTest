#include "Vec4SSE.h"
#include "Matrix4x4SSE.h"

using namespace SSE_Utils::Float4_Utils;

Vec4SSE::Vec4SSE()
	:
	v(ZeroPS)
{}

Vec4SSE::Vec4SSE(float S)
	:
	v(_mm_set1_ps(S))	// Set all elements to S (S, S, S, S)
{	
}
Vec4SSE::Vec4SSE(float X, float Y, float Z, float W)
	:
	v(_mm_set_ps(W, Z, Y, X))		// SSE registers are setup backward
{}
Vec4SSE::Vec4SSE(const FLOAT4 &V)
	:
	v(V)
{}
Vec4SSE::Vec4SSE(const Vector4 &V)
	:
	v(_mm_load_ps((float*)&V))
{}
Vec4SSE::Vec4SSE(const Vec4SSE &V)
	:
	v(V.v)
{}

Vec4SSE Vec4SSE::operator&(const Vec4SSE &V)const
{
	return{ v & V.v };
}
Vec4SSE Vec4SSE::operator|(const Vec4SSE &V)const
{
	return v | V.v;
}

Vec4SSE Vec4SSE::AndNot(const Vec4SSE &V)
{
	return{ SSE_Utils::Float4_Utils::AndNot(v, V.v) };
}

Vec4SSE Vec4SSE::operator-()const
{
	return{ -v };
}
Vec4SSE Vec4SSE::operator+(const Vec4SSE &V)const
{
	return v + V.v;
}
Vec4SSE Vec4SSE::operator-(const Vec4SSE &V)const
{
	return v - V.v;
}
Vec4SSE Vec4SSE::operator*(const float S)const
{
	return v * S;
}
Vec4SSE Vec4SSE::operator*(const Vec4SSE &V)const
{
	return v * V.v;
}
Vec4SSE Vec4SSE::operator*(const Matrix4x4SSE &M)const
{
	return{ DirectX::XMVector4Transform(v, M) };
}
Vec4SSE Vec4SSE::operator/(const float S)const
{
	return v / S;
}
Vec4SSE Vec4SSE::operator/(const Vec4SSE &V)const
{
	return v / V.v;
}
Vec4SSE & Vec4SSE::operator+=(const Vec4SSE &V)
{
	v += V.v;
	return (*this);
}
Vec4SSE & Vec4SSE::operator-=(const Vec4SSE &V)
{
	v -= V.v;
	return (*this);
}
Vec4SSE & Vec4SSE::operator*=(const Vec4SSE &V)
{
	v *= V.v;
	return (*this);
}
Vec4SSE & Vec4SSE::operator/=(const Vec4SSE &V)
{
	v /= V.v;
	return (*this);
}

Vec4SSE Vec4SSE::MultiplyAdd(const Vec4SSE &V0, const Vec4SSE &V1)const
{
	return V1.v + (v * V0.v);
}
Vec4SSE Vec4SSE::Dot(const Vec4SSE &V)const
{	
	FLOAT4 t0 = v * V.v;

	FLOAT4 t1 = Shuffle<Ayx | Bwz>(t0, t0);
	t0 += t1;	
	t1 = Shuffle<Azw | Bxy>(t0, t0);
	t0 += t1;

	return t0;
}

Vec4SSE Vec4SSE::Cross(const Vec4SSE &V)const
{
	FLOAT4 u0 = Shuffle<Ayz | Bwx>(v);
	FLOAT4 u1 = Shuffle<Azw | Bxy>(v);
	FLOAT4 v0 = Shuffle<Ayz | Bwx>(V.v);
	FLOAT4 v1 = Shuffle<Azw | Bxy>(V.v);

	FLOAT4 result = (u0 * v1) - (u1 * v0);
	return result;
}

Vec4SSE Vec4SSE::Length()const
{
	return{ SqrRoot(Dot(*this).v) };
}

Vec4SSE Vec4SSE::LengthSquare()const
{
	return Dot(*this);
}

Vec4SSE Vec4SSE::InverseLength()const
{
	FLOAT4 t0 = RecipSqrRoot(LengthSquare().v);
	return t0;
}

Vec4SSE Vec4SSE::Normalize()
{
	return ((*this) * InverseLength());
}

Vec4SSE Vec4SSE::SplatX()const
{
	return Shuffle<XXXX>(v);
}
Vec4SSE Vec4SSE::SplatY()const
{
	return Shuffle<YYYY>(v);
}
Vec4SSE Vec4SSE::SplatZ()const
{
	return Shuffle<ZZZZ>(v);
}
Vec4SSE Vec4SSE::SplatW()const
{
	return Shuffle<WWWW>(v);
}

Vector4 Vec4SSE::StoreFloat()const
{
	Vector4 temp;
	_mm_store_ps((float *)&temp, v);

	return temp;
}
Vector4 Vec4SSE::StoreInt()const
{
	Vector4 temp;
	_mm_store_si128((PDQWORD)&temp, _mm_cvttps_epi32(v));
	return temp;
}
Vector4 Vec4SSE::StoreIntCast()
{
	Vector4 temp;
	Vector4 t;
	_mm_store_si128((PDQWORD)&t, _mm_castps_si128(v));
	return temp;
}

float Vec4SSE::X()const
{
	return _mm_cvtss_f32(v);
}
float Vec4SSE::Y()const
{
	Vec4SSE t = SplatY();
	return _mm_cvtss_f32(t.v);
}
float Vec4SSE::Z()const
{
	Vec4SSE t = SplatZ();
	return _mm_cvtss_f32(t.v);
}
float Vec4SSE::W()const
{
	Vec4SSE t = SplatW();
	return _mm_cvtss_f32(t.v);
}
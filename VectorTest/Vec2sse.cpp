#include "Vec2SSE.h"
#include <DirectXMath.h>

using namespace SSE_Utils::Float4_Utils;

Vec2SSE::Vec2SSE()
	:
	v(ZeroPS)
{};
Vec2SSE::Vec2SSE(float S)
	:
	v(_mm_set1_ps(S))	// Set all elements to S (S, S, S, S)
{
	// Zero out the last two elements as they won't be used (v, v, 0.0f, 0.0f)
	v = _mm_shuffle_ps(v, _mm_setzero_ps(), _MM_SHUFFLE(0, 0, 2, 1));
}
Vec2SSE::Vec2SSE(float X, float Y)
	:
	v(_mm_set_ps(0.0f, 0.0f, Y, X))		// SSE registers are setup backward
{}
Vec2SSE::Vec2SSE(const FLOAT4 &V)
	:
	v(V)
{}
Vec2SSE::Vec2SSE(const Vector2 &V)
	:
	v(_mm_set_ps(0.0f, 0.0f, V.y, V.x))
{}
Vec2SSE::Vec2SSE(const Vec2SSE &V)
	:
	v(V.v)
{}

Vec2SSE Vec2SSE::operator&(const Vec2SSE &V)const
{
	return{ v & V.v };
}
Vec2SSE Vec2SSE::operator|(const Vec2SSE &V)const
{
	return v | V.v;
}

Vec2SSE Vec2SSE::AndNot(const Vec2SSE &V)
{
	return{ SSE_Utils::Float4_Utils::AndNot(v, V.v) };
}

Vec2SSE Vec2SSE::operator-()const
{
	return{ -v };
}
Vec2SSE Vec2SSE::operator+(const Vec2SSE &V)const
{
	return v + V.v;
}
Vec2SSE Vec2SSE::operator-(const Vec2SSE &V)const
{
	return v - V.v;
}
Vec2SSE Vec2SSE::operator*(const float S)const
{
	return v * S;
}
Vec2SSE Vec2SSE::operator*(const Vec2SSE &V)const
{
	return v * V.v;
}
Vec2SSE Vec2SSE::operator/(const float S)const
{
	return v / S;
}
Vec2SSE Vec2SSE::operator/(const Vec2SSE &V)const
{
	return v / V.v;
}
Vec2SSE & Vec2SSE::operator+=(const Vec2SSE &V)
{
	v += V.v;
	return (*this);
}
Vec2SSE & Vec2SSE::operator-=(const Vec2SSE &V)
{
	v -= V.v;
	return (*this);
}
Vec2SSE & Vec2SSE::operator*=(const Vec2SSE &V)
{
	v *= V.v;
	return (*this);
}
Vec2SSE & Vec2SSE::operator/=(const Vec2SSE &V)
{
	v /= V.v;
	return (*this);
}

// return V1 + ((*this) * V0);
Vec2SSE Vec2SSE::MultiplyAdd(const Vec2SSE &V0, const Vec2SSE &V1)
{
	return V1.v + (v * V0.v);
}

#include <ammintrin.h>
Vec2SSE Vec2SSE::Dot(const Vec2SSE &V)const
{
	FLOAT4 t0 = v * V.v;
	/*t0 = _mm_hadd_ps(t0, t0);
	t0 = Shuffle<Axz | Byw>(t0);*/
	FLOAT4 t1 = _mm_shuffle_ps(t0, ZeroPS, _MM_SHUFFLE(0, 0, 0, 1));
	t0 += t1;
	
	
	return t0;
}

Vec2SSE Vec2SSE::Cross(const Vec2SSE &V)const
{
	/*
	((x0 * y1) - (y0 * x1));
	*/
	FLOAT4 xy = v;
	FLOAT4 yx = _mm_shuffle_ps(V.v, _mm_setzero_ps(), _MM_SHUFFLE(0, 1, 0, 0));
	FLOAT4 t0 = xy * yx;
	FLOAT4 t1 = _mm_shuffle_ps(t0, ZeroPS, _MM_SHUFFLE(0, 0, 0, 1));
	t0 -= t1;

	return t0;
}

Vec2SSE Vec2SSE::Length()const
{
	return{ _mm_sqrt_ps(Dot(*this).v) };
}

Vec2SSE Vec2SSE::LengthSquare()const
{
	return Dot(*this);
}

Vec2SSE Vec2SSE::InverseLength()const
{
	return{ _mm_rsqrt_ps(LengthSquare().v) };
}

Vec2SSE Vec2SSE::Normalize()
{
	return ((*this) * InverseLength());
}

Vec2SSE Vec2SSE::SplatX()const
{
	return _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0));
}
Vec2SSE Vec2SSE::SplatY()const
{
	return _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1));
}

Vector2 Vec2SSE::StoreFloat()const
{
	Vector2 rhs;
	rhs.x = X();
	rhs.y = Y();

	return rhs;
}
Vector2 Vec2SSE::StoreInt()const
{
	Vector2 temp;
	FLOAT4 t = Shuffle<Ayx>(v, ZeroPS);
	temp.b = _mm_cvt_ss2si(v);	
	temp.g = _mm_cvt_ss2si(t);
		
	return temp;
}
Vector2 Vec2SSE::StoreIntCast()
{
	Vector2 temp;
	Vector4 t;
	_mm_store_si128((PDQWORD)&t, _mm_castps_si128(v));
	temp.ix = t.iX;
	temp.iy = t.iY;
	return temp;
}

float Vec2SSE::X()const
{
	return _mm_cvtss_f32(v);
}
float Vec2SSE::Y()const
{
	Vec2SSE t = SplatY();
	return _mm_cvtss_f32(t.v);
}

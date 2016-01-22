#include "Vec2f.h"

Vec2sse::Vec2sse()
	:
	v(_mm_setzero_ps())
{

}

Vec2sse::Vec2sse(float X, float Y)
	:
	v(_mm_set_ps(0.0f, 0.0f, Y, X))
{}

Vec2sse::Vec2sse(const Vec2sse &V)
	:
	v(V.v)
{

}

Vec2sse::Vec2sse(const __m128 &V)
	:
	v(V)
{}

Vec2sse::~Vec2sse()
{}

Vec2sse Vec2sse::operator-(const Vec2sse &V)const
{
	return{ _mm_sub_ps(v, V.v) };
}

Vec2sse Vec2sse::operator+(const Vec2sse &V)const
{
	return{ _mm_add_ps(v, V.v) };
}

Vec2sse Vec2sse::operator*(const Vec2sse &V)const
{
	Vec2sse t = _mm_mul_ps(v, V.v);
	Vec2sse r = _mm_shuffle_ps(t.v, t.v, _MM_SHUFFLE(0, 1, 2, 3));
	r.v = _mm_add_ps(t.v, r.v);
	return r;
}

Vec2sse Vec2sse::operator*(const float &S)const
{
	Vec2sse t(_mm_set1_ps(S));
	t.v = _mm_mul_ps(v, t.v);
	return t;
}

Vec2sse Vec2sse::operator/(const float &S)const
{
	Vec2sse t(_mm_set1_ps(S));
	t.v = _mm_rcp_ps(t.v);
	t.v = _mm_mul_ps(v, t.v);
	return t;
}

Vec2sse &Vec2sse::operator-=(const Vec2sse &V)
{
	v = _mm_sub_ps(v, V.v);
	return (*this);
}

Vec2sse &Vec2sse::operator+=(const Vec2sse &V)
{
	v = _mm_add_ps(v, V.v);
	return (*this);
}

Vec2sse Vec2sse::LenSqr()const
{
	Vec2sse t;
	t.v = _mm_mul_ps(v, v);
	__m128 r = _mm_shuffle_ps(t.v, t.v, _MM_SHUFFLE(0, 1, 2, 3));
	t.v = _mm_add_ps(r, t.v);
	return t;
}

Vec2sse Vec2sse::InvLength()const
{
	Vec2sse t;
	t.v = _mm_mul_ps(v, v);
	__m128 r = _mm_shuffle_ps(t.v, t.v, _MM_SHUFFLE(0, 1, 2, 3));
	t.v = _mm_add_ps(r, t.v);
	t.v = _mm_rsqrt_ps(t.v);
	return t;
}

Vec2sse Vec2sse::Length()const
{
	Vec2sse t;
	t.v = _mm_mul_ps(v, v);
	__m128 r = _mm_shuffle_ps(t.v, t.v, _MM_SHUFFLE(0, 1, 2, 3));
	t.v = _mm_add_ps(r, t.v);
	t.v = _mm_sqrt_ps(t.v);
	return t;
}

Vec2sse Vec2sse::Normalize()const
{
	Vec2sse t;
	t.v = _mm_mul_ps(v, v);
	__m128 r = _mm_shuffle_ps(t.v, t.v, _MM_SHUFFLE(0, 1, 2, 3));
	t.v = _mm_add_ps(r, t.v);
	t.v = _mm_rsqrt_ps(t.v);
	t.v = _mm_mul_ps(v, t.v);
	return t;
}

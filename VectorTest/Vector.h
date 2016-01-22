#pragma once
#include <Windows.h>
#include <DirectXMath.h>
#include "SSE.h"

// Used for store operations
Align16
union Vector4
{
	struct
	{
		float x, y, z, w;
	};
	struct
	{
		UINT b, g, r, a;
	};
	struct
	{
		UINT iX, iY, iZ, iW;
	};
};

Align16
class Vector
{
public:
	Vector(){};
	Vector(float S)
	{
		vector = _mm_set1_ps(S);
	}
	Vector(float X, float Y, float Z, float W = 1.0f)
	{
		vector = _mm_setr_ps(X, Y, Z, W);
	}
	Vector(const FLOAT4 &rhs)
		:
		vector(rhs)
	{}
	Vector(const DirectX::XMFLOAT3 &rhs)
	{
		vector = Vector(rhs.x, rhs.y, rhs.z, 0.0f).vector;
	}
	Vector(const DirectX::XMFLOAT4 &rhs)
	{
		vector = _mm_loadu_ps((float *)&rhs);
	}
	inline Vector _vectorcall operator&(const Vector &V )
	{
		return _mm_and_ps(vector, V.vector);
	}
	inline Vector _vectorcall operator|(const Vector &V)const
	{
		FLOAT4 result = _mm_or_ps(vector, V.vector);
		return result;
	}
	inline Vector _vectorcall operator+(const Vector &V)const
	{
		return _mm_add_ps(vector, V.vector);
	}
	inline Vector _vectorcall operator-()const
	{
		return _mm_sub_ps(_mm_setzero_ps(), vector);
	}
	inline Vector _vectorcall operator-(const Vector &V)const
	{
		return _mm_sub_ps(vector, V.vector);
	}
	inline Vector _vectorcall operator*(const float S)const
	{
		return _mm_mul_ps(_mm_set1_ps(S), vector);
	}
	inline Vector _vectorcall operator*(const Vector &V)const
	{
		return _mm_mul_ps(vector, V.vector);
	}
	inline Vector _vectorcall operator/(const float S)const
	{
		return _mm_mul_ps(vector, _mm_rcp_ps(_mm_set1_ps(S)));
	}
	inline Vector _vectorcall operator/(const Vector &V)const
	{
		return _mm_mul_ps(vector, _mm_rcp_ps(V.vector));
	}
	inline Vector & _vectorcall operator+=(const Vector &V)
	{
		vector = _mm_add_ps(vector, V.vector);
		return (*this);
	}
	inline Vector &_vectorcall operator*=(const Vector &V)
	{
		vector = _mm_mul_ps(vector, V.vector);
		return (*this);
	}
	
	inline Vector _vectorcall Add3(const Vector &V)
	{
		DQWORD wMaski = _mm_setr_epi32(xyzMask);
		FLOAT4 wMaskf = _mm_castsi128_ps(wMaski);

		FLOAT4 t0 = _mm_add_ps(vector, V.vector);
		t0 = _mm_and_ps(t0, wMaskf);

		return t0;
	}

	// return V1 + ((*this) * V0);
	inline Vector _vectorcall MultiplyAdd(const Vector &V0, const Vector &V1)
	{
		return  _mm_add_ps(V1.vector, _mm_mul_ps(vector, V0.vector));
	}
	inline Vector _vectorcall Dot2(const Vector &A)const
	{
		UINT all = -1, none = 0;

		DQWORD xyMaski = _mm_setr_epi32(xyMask);
		FLOAT4 xyMaskf = _mm_castsi128_ps(xyMaski);

		FLOAT4 t0 = _mm_mul_ps(vector, A.vector);
		t0 = _mm_and_ps(t0, xyMaskf);
		FLOAT4 t1 = _mm_shuffle_ps(t0, t0, _MM_SHUFFLE(0, 1, 2, 3));
		t0 = _mm_add_ps(t0, t1);
		t1 = _mm_shuffle_ps(t0, t0, _MM_SHUFFLE(1, 0, 0, 1));
		t0 = _mm_add_ps(t0, t1);

		return t0;
	}
	inline Vector _vectorcall Dot3(const Vector &A)const
	{		
		DQWORD xyzMaski = _mm_setr_epi32(xyzMask);
		FLOAT4 xyzMaskf = _mm_castsi128_ps(xyzMaski);

		FLOAT4 t0 = _mm_mul_ps(vector, A.vector);
		t0 = _mm_and_ps(vector, xyzMaskf);

		FLOAT4 t1 = _mm_shuffle_ps(t0, t0, _MM_SHUFFLE(0, 1, 2, 3));
		t0 = _mm_add_ps(t0, t1);
		t1 = _mm_shuffle_ps(t0, t0, _MM_SHUFFLE(1, 0, 0, 1));
		t0 = _mm_add_ps(t0, t1);

		return t0;
	}
	inline Vector _vectorcall Dot4(const Vector &A)const
	{
		FLOAT4 t0 = _mm_mul_ps(A.vector, vector);
		FLOAT4 t1 = _mm_shuffle_ps(t0, t0, _MM_SHUFFLE(0, 1, 2, 3));
		t0 = _mm_add_ps(t0, t1);
		t1 = _mm_shuffle_ps(t0, t0, _MM_SHUFFLE(1, 0, 0, 1));
		t0 = _mm_add_ps(t0, t1);

		return t0;
	}
	// Uses DirectXMath XMVector3Cross
	inline Vector _vectorcall Cross3(const Vector &rhs)const
	{
		return DirectX::XMVector3Cross(vector, rhs.vector);
	}

	// Uses recipracol square root which is a lot faster, but not as accurate
	inline Vector Normalize3()
	{
		DQWORD xyzMaski = _mm_setr_epi32(xyzMask);
		FLOAT4 xyzMaskf = _mm_castsi128_ps(xyzMaski);
		vector = _mm_and_ps(vector, xyzMaskf);

		FLOAT4 len = _mm_rsqrt_ps(Dot3(vector).vector);
		FLOAT4 v = _mm_mul_ps(vector, len);

		return v;
	}
	inline Vector Normalize4()
	{
		FLOAT4 temp = Dot4(vector).vector;
		FLOAT4 len = _mm_rsqrt_ps(temp);
		FLOAT4 v = _mm_mul_ps(vector, len);

		return v;
	}

	inline Vector SplatX()const
	{
		return _mm_shuffle_ps(vector, vector, _MM_SHUFFLE(0, 0, 0, 0));
	}
	inline Vector SplatY()const
	{		
		return _mm_shuffle_ps(vector, vector, _MM_SHUFFLE(1, 1, 1, 1));
	}
	inline Vector SplatZ()const
	{		
		return _mm_shuffle_ps(vector, vector, _MM_SHUFFLE(2, 2, 2, 2));
	}
	inline Vector SplatW()const
	{
		return _mm_shuffle_ps(vector, vector, _MM_SHUFFLE(3, 3, 3, 3));
	}

	template<typename const int A, typename const int B, typename const int C, typename const int D>
	inline Vector &Shuffle()
	{
		vector = _mm_shuffle_ps(vector, vector, _MM_SHUFFLE(D, C, B, A));
		return (*this);
	}

	inline Vector4 StoreFloat()const
	{		
		Vector4 rhs;
		_mm_store_ps((float *)&rhs, vector);
		return rhs;
	}
	inline Vector4 StoreInt()const
	{
		Vector4 temp;
		_mm_store_si128((PDQWORD)&temp, _mm_cvtps_epi32(vector));
		return temp;
	}
	inline Vector4 StoreIntCast()
	{
		Vector4 temp;
		_mm_store_si128((PDQWORD)&temp, _mm_castps_si128(vector));
		return temp;
	}

	inline float X()const
	{
		return _mm_cvtss_f32(vector);
	}
	inline float Y()const
	{
		Vector t = SplatY();
		return _mm_cvtss_f32(t.vector);
	}
	inline float Z()const
	{
		Vector t = SplatZ();
		return _mm_cvtss_f32(t.vector);
	}
	inline float W()const
	{
		Vector t = SplatW();
		return _mm_cvtss_f32(t.vector);
	}

	// Shamefully, copied from DirectXMath
	inline Vector Ceil()
	{
		using namespace DirectX;
		// To handle NAN, INF and numbers greater than 8388608, use masking
		DQWORD vTest = _mm_and_si128(_mm_castps_si128(vector), g_XMAbsMask);
		vTest = _mm_cmplt_epi32(vTest, g_XMNoFraction);

		// Truncate
		DQWORD vInt = _mm_cvttps_epi32(vector);
		FLOAT4 vResult = _mm_cvtepi32_ps(vInt);
		FLOAT4 vSmaller = _mm_cmplt_ps(vResult, vector);

		// 0 -> 0, 0xffffffff -> -1.0f
		vSmaller = _mm_cvtepi32_ps(_mm_castps_si128(vSmaller));
		vResult = _mm_sub_ps(vResult, vSmaller);

		// All numbers less than 8388608 will use the round to int
		vResult = _mm_and_ps(vResult, _mm_castsi128_ps(vTest));

		// All others, use the ORIGINAL value
		vTest = _mm_andnot_si128(vTest, _mm_castps_si128(vector));
		vResult = _mm_or_ps(vResult, _mm_castsi128_ps(vTest));

		return vResult;
	}

	// Shamefully, copied from DirectXMath
	inline Vector Floor()
	{
		using namespace DirectX;
		// To handle NAN, INF and numbers greater than 8388608, use masking
		__m128i vTest = _mm_and_si128(_mm_castps_si128(vector), g_XMAbsMask);
		vTest = _mm_cmplt_epi32(vTest, g_XMNoFraction);

		// Truncate
		__m128i vInt = _mm_cvttps_epi32(vector);
		FLOAT4 vResult = _mm_cvtepi32_ps(vInt);
		FLOAT4 vLarger = _mm_cmpgt_ps(vResult, vector);

		// 0 -> 0, 0xffffffff -> -1.0f
		vLarger = _mm_cvtepi32_ps(_mm_castps_si128(vLarger));
		vResult = _mm_add_ps(vResult, vLarger);

		// All numbers less than 8388608 will use the round to int
		vResult = _mm_and_ps(vResult, _mm_castsi128_ps(vTest));

		// All others, use the ORIGINAL value
		vTest = _mm_andnot_si128(vTest, _mm_castps_si128(vector));
		vResult = _mm_or_ps(vResult, _mm_castsi128_ps(vTest));
		return vResult;
	}

	inline Vector Abs()
	{
		FLOAT4 result = _mm_setzero_ps();
		result = _mm_sub_ps(result, vector);
		return _mm_max_ps(result, vector);
	}
	inline Vector _vectorcall Min(const Vector &rhs)
	{
		return _mm_min_ps(vector, rhs.vector);
	}
	inline Vector _vectorcall Max(const Vector &rhs)
	{
		return _mm_max_ps(vector, rhs.vector);
	}

	FLOAT4 vector;
};

namespace Math
{
	inline Vector Slope(const Vector &SubLo, const Vector &SubHi, const Vector &DivBy)
	{
		FLOAT4 t = _mm_sub_ps(SubHi.vector, SubLo.vector);
		t = _mm_mul_ps(t, _mm_rcp_ps(DivBy.vector));
		return t;
	}
	inline Vector _vectorcall Interpolate(const Vector &Start, const Vector &End, const float Step)
	{
		FLOAT4 tStep = _mm_set1_ps(Step);
		FLOAT4 t = _mm_sub_ps(End.vector, Start.vector);
		t = _mm_mul_ps(t, tStep);
		t = _mm_add_ps(t, Start.vector);

		return t;
	}
	inline Vector _vectorcall Interpolate(const Vector &Start, const Vector &End, const Vector &Step)
	{
		FLOAT4 t = _mm_sub_ps(End.vector, Start.vector);
		t = _mm_mul_ps(t, Step.vector);
		t = _mm_add_ps(t, Start.vector);

		return t;
	}
	inline void _vectorcall StreamCopy(float *Dest, FLOAT4 Src)
	{
		_mm_stream_ps(Dest, Src);
	}

}
#pragma once

#ifndef _INC_WINDOWS
#include <Windows.h>
#endif

// Include for SSE 1/2/3
#include <pmmintrin.h>

#include <memory>
#include "Math.h"

typedef __m128 FLOAT4, *PFLOAT4;
typedef __m128i DQWORD, *PDQWORD;

//***************************************************************/
//*   SSE float intrinsic functions end in ss, ps, sd and pd	*/
//*   SS = Scalar Single precision float ie. float				*/
//*   PS = Packed Single precision float ie. four floats		*/
//*   SD = Scalar Double precision float ie. double				*/
//*   PD = Packed Double precision float ie. two doubles		*/
//*																*/
//*   The same notation will be used in this file				*/
//***************************************************************/


// Aligned declarations
/*
Put Align16 before structs or classes to have them aligned
when creating objects, and before variable declarations
*/
#define Align16 _declspec(align(16))

enum ShuffConstants
{
	Axx	= 0,
	Ayx = 1,
	Azx = 2,
	Awx = 3,
	Axy = 4,
	Ayy = 5,
	Azy = 6,
	Awy = 7,
	Axz = 8,
	Ayz = 9,
	Azz = 10,
	Awz = 11,
	Axw = 12,
	Ayw = 13,
	Azw = 14,
	Aww = 15,

	Bxx = 0,
	Byx = 16,
	Bzx = 32,
	Bwx = 48,
	Bxy = 64,
	Byy = 80,
	Bzy = 96,
	Bwy = 112,
	Bxz = 128,
	Byz = 144,
	Bzz = 160,
	Bwz = 176,
	Bxw = 192,
	Byw = 208,
	Bzw = 224,
	Bww = 240
};

enum ShuffleOrder
{
	// Splat
	XXXX = Axx | Bxx,
	YYYY = Ayy | Byy,
	ZZZZ = Azz | Bzz,
	WWWW = Aww | Bww,

	// Replicate
	XYXY = Axy | Bxy,
	XZXZ = Axz | Bxz,
	XWXW = Axw | Bxw,
	YXYX = Ayx | Byx,
	YZYZ = Ayz | Byz,
	YWYW = Ayw | Byw,
	ZXZX = Azx | Bzx,
	ZYZY = Azy | Bzy,
	ZWZW = Azw | Bzw,

	// Mirror
	XYYX = Axy | Byx,
	XZZX = Axz | Bzx,
	XWWX = Axw | Bwx,
	YXXY = Ayx | Bxy,
	YZZY = Ayz | Bzy,
	YWWY = Ayw | Bwy,
	ZXXZ = Azx | Bxz,
	ZYYZ = Azy | Byz,
	ZWWZ = Azw | Bwz,
	WXXW = Awx | Bxw,
	WYYW = Awy | Byw,
	WZZW = Awz | Bzw,

	// Roll
	YZWX = Ayz | Bwx,
	ZWXY = Azw | Bxy,
	WXYZ = Awx | Byz,

	// Reverse
	WZYX = Awz | Byx
};

#define Replicate_ps _mm_set1_ps

#define xMask		0xFFFFFFFF, 0x0, 0x0, 0x0
#define yMask		0x0, 0xFFFFFFFF, 0x0, 0x0
#define zMask		0x0, 0x0, 0xFFFFFFFF, 0x0
#define wMask		0x0, 0x0, 0x0, 0xFFFFFFFF

#define xyMask		0xFFFFFFFF, 0xFFFFFFFF, 0x0, 0x0
#define xyzMask		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0

/*
Reverse
Example:
FLOAT4 res = Reverse(x,y,z,w)
res = w,z,y,x
*/
#define ReverseVector(A) (Shuffle(A, A, WZYX))

#pragma region Replicate
/*
Replicate
Example:
x, y, z, w to (x, y, x, y)
*/
#define ReplicateXY(A) (Shuffle(A, A, XYXY))
#define ReplicateXZ(A) (Shuffle(A, A, XZXZ))
#define ReplicateXW(A) (Shuffle(A, A, XWXW))

#define ReplicateYX(A) (Shuffle(A, A, YXYX))
#define ReplicateYZ(A) (Shuffle(A, A, YZYZ))
#define ReplicateYW(A) (Shuffle(A, A, YWYW))

#define ReplicateZX(A) (Shuffle(A, A, ZXZX))
#define ReplicateZY(A) (Shuffle(A, A, ZYZY))
#define ReplicateZW(A) (Shuffle(A, A, ZWZW))
#pragma endregion Replicate shuffling

#pragma region Mirror
/*
Mirror??
Example:
A = x, y, z, w
B = MirrorXY(A)
B = Ax, Ay, Ay, Ax
*/
#define MirrorXY(A) (Shuffle(A, A, XYYX))
#define MirrorXZ(A) (Shuffle(A, A, XZZX))
#define MirrorXW(A) (Shuffle(A, A, XWWX))

#define MirrorYX(A) (Shuffle(A, A, YXXY))
#define MirrorYZ(A) (Shuffle(A, A, YZZY))
#define MirrorYW(A) (Shuffle(A, A, YWWY))

#define MirrorZX(A) (Shuffle(A, A, ZXXZ))
#define MirrorZY(A) (Shuffle(A, A, ZYYZ))
#define MirrorZW(A) (Shuffle(A, A, ZWWZ))

#define MirrorWX(A) (Shuffle(A, A, WXXW))
#define MirrorWY(A) (Shuffle(A, A, WYYW))
#define MirrorWZ(A) (Shuffle(A, A, WZZW))
#pragma endregion Mirror shuffling

/*
Repeat
float a = 0.0f
FLOAT4 A = Repeat(a);
A = 0.0f, 0.0f, 0.0f, 0.0f
*/
#define Repeat _mm_set1_ps

/*
SetFloat4
Creates a FLOAT4 using 4 floats
SetFloat4(f0, f1, f2, f3).  SetFloat4 reverses the values so that
they appear in the order they were set
*/
#define SetFloat4(f0, f1, f2, f3) (_mm_setr_ps(f0, f1, f2, f3))

/*
MoveHiLo
Example:
A = x, y, z, w
B = x, y, z, w
res = Bz, Bw, Az, Aw;
*/
#define MoveHiLo _mm_movehl_ps
/*
MoveLoHi
Example:
A = x, y, z, w
B = x, y, z, w
res = Ax, Ay, Bx, By
*/
#define MoveLoHi _mm_movelh_ps

/*
Add
A + B
*/
#define Add _mm_add_ps

/*
Sub
A - B
*/
#define Sub _mm_sub_ps

/*
Mul
A * B
*/
#define Mul _mm_mul_ps

/*
Div
A * recip(B)
It's faster to take the reciprocal of a number and multiply 
than to divide
*/
#define Div _mm_div_ps

/*
Recip
1.0f / A
*/
#define Recip _mm_rcp_ps

/*
Sqrt
square root
*/
#define Sqrt _mm_sqrt_ps

/*
RecipSqrt
1.0 / sqrt(A)
*/
#define RecipSqrt _mm_rsqrt_ps

/*
AndNot
Inverts the bits in A and (and's) them with B
A = 0011, B = 1010
C = AndNot(A, B)  (1100 & 1010)
C = 1000
*/
#define AndNot _mm_andnot_ps

/*
And
Logical and ( & )
*/
#define And _mm_and_ps

/*
Or
Logical or ( | )
*/
#define Or _mm_or_ps

#define gt _mm_cmpgt_ps
#define lt _mm_cmplt_ps
#define equals _mm_cmpeq_ps
#define noteq _mm_cmpneq_ps
#define lte _mm_cmple_ps
#define gte _mm_cmpge_ps

/*
Roll(FLOAT4, ShuffleOrder) or Roll(FLOAT4, ShuffleConstants)
Shuffles the elements to mimic rolling
A = 1.0f, 2.0f, 3.0f, 4.0f
Roll(A, ShuffleOrder::WXYZ) 
A = 4.0f, 1.0f, 2.0f, 3.0f
Roll(A, ShuffleConstants::Ayz + ShuffleConstants::Bwx)
A = 1.0f, 2.0f, 3.0f, 4.0f
*/
#define Roll(A, B) (Shuffle((A), (A), (B)))

/*
RollR
Shifts all elements to the right, and puting the far right element
on the left.
X, Y, Z, W becomes W, X, Y, Z
*/
#define RollR(A) (_mm_shuffle_ps(A, A, Awx + Byz))

/*
RollL
Shifts all elements to the left and putting the far left element
on the right
X, Y, Z, W becomes Y, Z, W, X
*/
#define RollL(A) (_mm_shuffle_ps(A, A, Ayz + Bwx))

/*
Zerops returns a FLOAT4 with all elements set to 0.0f
A = Zerops
A = 0.0f, 0.0f, 0.0f, 0.0f
*/
#define Zerops _mm_setzero_ps()

/*
Oneps sets all elements to 1.0f
A = Oneps
A = 1.0f, 1.0f, 1.0f, 1.0f
*/
#define Oneps _mm_set1_ps(1.0f)

/*
ConvertToFloat4
Converts DQWORD (__m128i) to FLOAT4 (__m128)
Converts the four ints to four floats 
1, 2, 10, 20 becomes 1.0f, 2.0f, 10.0f, 20.0f
*/
#define CvtToFloat4 _mm_cvtepi32_ps

/*
ConvertToDQWord
Converts FLOAT4 (__m128) to DQWORD (__m128i)
Converts the four floats to four ints, rounding to nearest whole number
1.1f, 2.0092f, 10.9f, 2.0f becomes 1, 2, 11, 2
*/
#define CvtToDQWord _mm_cvtps_epi32

/*
CvtTrunc
Converts FLOAT4 to DQWORD by first truncating or removing everything 
to the right of the decimal
*/
#define CvtTrunc _mm_cvttps_epi32

/*
CastToFloat4
Casts the binary value from DQWORD to FLOAT4, no conversion or rounding
is done.
*/
#define CastToFloat4 _mm_castsi128_ps

/*
CastToDQWord
Casts the binary value from FLOAT4 to DQWORD, no conversion is done.
*/
#define CastToDQWord _mm_castps_si128

/*
Streamps
Bypasses the cpu cache.  Use when you want to copy large amounts of data 
from one location to another and aren't going to be processing it.

*/
#define Streamps _mm_stream_ps

#define Minps _mm_min_ps
#define Maxps _mm_max_ps


Align16 
struct Matrix
{
	FLOAT4 r[4];
};

namespace float_sse
{
	inline FLOAT4 _vectorcall operator+(const FLOAT4 &A, const FLOAT4 &B)
	{
		return Add(A, B);
	}
	inline FLOAT4 _vectorcall operator+=(FLOAT4 &A, const FLOAT4 &B)
	{
		A = Add(A, B);
		return A;
	}
	inline FLOAT4 _vectorcall operator-(const FLOAT4 &A, const FLOAT4 &B)
	{
		return Sub(A, B);
	}
	inline FLOAT4 _vectorcall operator*(const FLOAT4 &A, const FLOAT4 &B)
	{
		return Mul(A, B);
	}
	inline FLOAT4 _vectorcall operator*(const FLOAT4 &A, const float &B)
	{
		return A * Repeat(B);
	}
	inline FLOAT4 _vectorcall operator/(const FLOAT4 &A, const FLOAT4 &B)
	{
		return A * Recip(B);
	}
	inline FLOAT4 _vectorcall operator-(const FLOAT4 &A)
	{
		return Sub(Zerops, A);
	}

	// Logical operators
	inline FLOAT4 _vectorcall operator&(const FLOAT4 &A, const FLOAT4 &B)
	{
		return And(A, B);
	}
	inline FLOAT4 _vectorcall operator|(const FLOAT4 &A, const FLOAT4 &B)
	{
		return Or(A, B);
	}
	inline FLOAT4 _vectorcall operator~(const FLOAT4 &A)
	{
		return AndNot(A, A);
	}

	// Compare operators
	inline FLOAT4 _vectorcall operator>(const FLOAT4 &A, const FLOAT4 &B)
	{
		
		return gt(A, B);
	}
	inline FLOAT4 _vectorcall operator<(const FLOAT4 &A, const FLOAT4 &B)
	{
		return lt(A, B);
	}
	/*inline FLOAT4 _vectorcall operator==(const FLOAT4 &A, const FLOAT4 &B)
	{
		return eq(A, B);
	}*/
	inline FLOAT4 _vectorcall operator>=(const FLOAT4 &A, const FLOAT4 &B)
	{
		return gte(A, B);
	}
	inline FLOAT4 _vectorcall operator<=(const FLOAT4 &A, const FLOAT4 &B)
	{
		return lte(A, B);
	}
	inline FLOAT4 _vectorcall operator!=(const FLOAT4 &A, const FLOAT4 &B)
	{
		return noteq(A, B);
	}

	// Vector operations
	inline FLOAT4 _vectorcall DotProduct(const FLOAT4 &A, const FLOAT4 &B)
	{
		FLOAT4 t0 = A * B;
		FLOAT4 t5 = _mm_hadd_ps(t0, t0);
		t5 = _mm_hadd_ps(t5, t5);
		return t5;
	}
	inline FLOAT4 _vectorcall LengthSqr(const FLOAT4 &A)
	{
		return DotProduct(A, A);
	}
	inline FLOAT4 _vectorcall Length(const FLOAT4 &A)
	{
		return Sqrt(LengthSqr(A));
	}
	inline FLOAT4 _vectorcall RecipLength(const FLOAT4 &A)
	{
		return RecipSqrt(LengthSqr(A));
	}
	inline FLOAT4 _vectorcall Normalize(const FLOAT4 &A)
	{
		return A * RecipLength(A);
	}
	inline FLOAT4 _vectorcall Interpolate(const FLOAT4 &A, const FLOAT4 &B, const FLOAT4 &Step)
	{
		FLOAT4 dist = (B - A);
		FLOAT4 rcpDist = Length(dist);
		FLOAT4 stepDist = Step / rcpDist;
		FLOAT4 deltaDist = dist * stepDist;
		FLOAT4 dif = A + deltaDist;
		return dif;
	}
	inline FLOAT4 _vectorcall Ceil(const FLOAT4 &A)
	{
		DQWORD b = _mm_cvttps_epi32(A);
		FLOAT4 B = _mm_cvtepi32_ps(b);
		FLOAT4 fMask = B < A;
		DQWORD iMask = _mm_castps_si128(fMask);
		return B - _mm_cvtepi32_ps(iMask);
	}
	inline FLOAT4 _vectorcall Floor(const FLOAT4 &A)
	{
		DQWORD b = _mm_cvttps_epi32(A);
		FLOAT4 B = _mm_cvtepi32_ps(b);
		FLOAT4 fMask = B > A;
		DQWORD iMask = _mm_castps_si128(fMask);
		fMask = _mm_cvtepi32_ps(iMask);
		return B + fMask;
	}

	// Matrix operations
	/*inline void _vectorcall Transpose(const Matrix &M, Matrix &Out)
	{
		FLOAT4 t0, t1;

		t0 = Shuffle(M.r[0], M.r[1], XYXY);
		t1 = Shuffle(M.r[2], M.r[3], XYXY);
		Out.r[0] = Shuffle(t0, t1, XZXZ);
		Out.r[1] = Shuffle(t0, t1, YWYW);

		t0 = Shuffle(M.r[0], M.r[1], ZWZW);
		t1 = Shuffle(M.r[2], M.r[3], ZWZW);
		Out.r[2] = Shuffle(t0, t1, XZXZ);
		Out.r[3] = Shuffle(t0, t1, YWYW);

		int a = 0;
	}*/
	inline Matrix _vectorcall operator+(const Matrix &M0, const Matrix &M1)
	{
		Matrix res;
		for (int i = 0; i < 4; ++i)
		{
			res.r[i] = M0.r[i] + M1.r[i];
		}

		return res;
	}
	inline Matrix _vectorcall operator-(const Matrix &M0, const Matrix &M1)
	{
		Matrix res;
		for (int i = 0; i < 4; ++i)
		{
			res.r[i] = M0.r[i] - M1.r[i];
		}

		return res;
	}
	/*inline FLOAT4 _vectorcall operator*(const FLOAT4 &V, const Matrix &M)
	{		
		Matrix tm;
		Transpose(M, tm);		

		FLOAT4 X = V * tm.r[0];
		FLOAT4 Y = V * tm.r[1];
		FLOAT4 Z = V * tm.r[2];
		FLOAT4 W = V * tm.r[3];
		FLOAT4 tY = _mm_hadd_ps(X, Y);
		FLOAT4 tW = _mm_hadd_ps(Z, W);
		FLOAT4 t4 = _mm_hadd_ps(tY, tW);
			
		return t4;
	}*/
	/*inline Matrix _vectorcall operator*(const Matrix &M0, const Matrix &M1)
	{
		Matrix res;
		
		for (int i = 0; i < 4; ++i)
		{
			res.r[i] = M0.r[i] * M1;
		}

		return res;
	}*/
	
	/*inline Matrix _vectorcall Identity()
	{
		Matrix res;
		res.r[0] = MaskX(Oneps);
		res.r[1] = RollR(res.r[0]);
		res.r[2] = RollR(res.r[1]);
		res.r[3] = RollR(res.r[2]);

		return res;
	}*/
	/*inline Matrix _vectorcall RotateX(const float theta)
	{
		float angle = ConverToRadians(theta);
		float sinTheta = sin(angle);
		float cosTheta = cos(angle);

		Matrix res = Identity();
		res.r[1] = SetFloat4(0.0f, cosTheta, sinTheta, 0.0f);
		res.r[2] = SetFloat4(0.0f, -sinTheta, cosTheta, 0.0f);
		
		return res;
	}*/
	/*inline Matrix _vectorcall RotateY(const float theta)
	{
		float angle = (3.14159f * (1.0f / 180.0f)) * theta;
		float sinTheta = sin(angle);
		float cosTheta = cos(angle);

		Matrix res = Identity();
		res.r[0] = SetFloat4(cosTheta, 0.0f, -sinTheta, 0.0f);
		res.r[2] = SetFloat4(sinTheta, 0.0f, cosTheta, 0.0f);

		return res;
	}*/
	/*inline Matrix _vectorcall RotateZ(const float theta)
	{
		float angle = (3.14159f * (1.0f / 180.0f)) * theta;
		float sinTheta = sin(angle);
		float cosTheta = cos(angle);

		Matrix res = Identity();
		res.r[0] = SetFloat4(cosTheta, sinTheta, 0.0f, 0.0f);
		res.r[1] = SetFloat4(-sinTheta, cosTheta, 0.0f, 0.0f);

		return res;
	}*/
	/*inline Matrix _vectorcall Rotate(const FLOAT4 Theta)
	{
		float *ft = (float *)&Theta;
		return RotateZ(ft[2]) * RotateX(ft[0]) * RotateY(ft[1]);
	}*/
	/*inline Matrix _vectorcall Translate(const FLOAT4 Trans)
	{
		Matrix res = Identity();
		res.r[3] = Trans;
		return res;
	}*/
	/*inline Matrix _vectorcall Scale(const FLOAT4 scale)
	{
		Matrix res = Identity();
		for (int i = 0; i < 4; ++i)
		{
			res.r[i] = res.r[i] * scale;
		}

		return res;
	}*/
}



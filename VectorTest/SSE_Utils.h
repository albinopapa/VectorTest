#pragma once

#ifndef _INC_WINDOWS
#include <Windows.h>
#endif

// Include for SSE 1/2/3
#include <intrin.h>
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
//*   The same notation will be used in this file like ZeroPS	*/
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

#define xMask		0xFFFFFFFF, 0x0, 0x0, 0x0
#define yMask		0x0, 0xFFFFFFFF, 0x0, 0x0
#define zMask		0x0, 0x0, 0xFFFFFFFF, 0x0
#define wMask		0x0, 0x0, 0x0, 0xFFFFFFFF

#define xyMask		0xFFFFFFFF, 0xFFFFFFFF, 0x0, 0x0
#define xyzMask		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0

#define SetFloat4(f0, f1, f2, f3) (_mm_set_ps(f3, f2, f1, f0))

#define ZeroPS _mm_setzero_ps()
#define OnePS _mm_set1_ps(1.0f)
#define HalfPS _mm_set1_ps(0.5f);

#define ConvertToFloat4(A) _mm_cvtepi32_ps((A))
#define ConvertToDQWORD(A) _mm_cvtps_epi32((A))
#define TruncateConvertToDQWORD(A) _mm_cvttps_epi32((A))
#define CastToFloat4(A) _mm_castsi128_ps((A))
#define CastToDQWORD(A) _mm_castps_si128((A))

#define StreamPS _mm_stream_ps

// SSE Integer Utils

enum ShiftCount128
{
	// ARGBARGBARGBARGB
	SL_RGBA_RGBA_RGBA_RGB0 = 15,
	SL_GBAR_GBAR_GBAR_GB00 = 14,
	SL_BARG_BARG_BARG_B000 = 13,
	SL_ARGB_ARGB_ARGB_0000 = 12,
	SL_RGBA_RGBA_RGB0_0000 = 11,
	SL_GBAR_GBAR_GB00_0000 = 10,
	SL_BARG_BARG_B000_0000 = 9,
	SL_ARGB_ARGB_0000_0000 = 8,
	SL_RGBA_RGB0_0000_0000 = 7,
	SL_GBAR_GB00_0000_0000 = 6,
	SL_BARG_B000_0000_0000 = 5,
	SL_ARGB_0000_0000_0000 = 4,
	SL_RGB0_0000_0000_0000 = 3,
	SL_GB00_0000_0000_0000 = 2,
	SL_B000_0000_0000_0000 = 1,

	SR_0ARG_BARG_BARG_B0RG = 15,
	SR_00AR_GBAR_GBAR_GB0R = 14,
	SR_000A_RGBA_RGBA_RGB0 = 13,
	SR_0000_ARGB_ARGB_ARGB = 12,
	SR_0000_0ARG_BARG_B0RG = 11,
	SR_0000_00AR_GBAR_GB0R = 10,
	SR_0000_000A_RGBA_RGB0 = 9,
	SR_0000_0000_ARGB_ARGB = 8,
	SR_0000_0000_0ARG_B0RG = 7,
	SR_0000_0000_00AR_GB0R = 6,
	SR_0000_0000_000A_RGB0 = 5,
	SR_0000_0000_0000_ARGB = 4,
	SR_0000_0000_0000_0ARG = 3,
	SR_0000_0000_0000_00AR = 2,
	SR_0000_0000_0000_000A = 1
};

enum Shuffle_Words_or_DWords
{
	// Roll
	RGBA = Ayz | Bwx,
	GBAR = Azw | Bxy,
	BARG = Awx | Byz,

	// Reverse
	BGRA = Awz | Byx,

	// Replicate
	AAAA = Aww | Bww
};

#define ShiftLeft_128 _mm_slli_si128
#define ShiftRight_128 _mm_srli_si128

#define ShuffleHi_Word _mm_shufflehi_epi16
#define ShuffleLo_Word _mm_shufflelo_epi16

#define Zero128 _mm_setzero_si128()
#define Replicate32 _mm_set1_epi32
#define Replicate16 _mm_set1_epi16

// Roll DWORDs 
#define RollR32_1(A) (_mm_shuffle_epi32(A, BARG))
#define RollR32_2(A) (_mm_shuffle_epi32(A, GBAR))
#define RollR32_3(A) (_mm_shuffle_epi32(A, RGBA))

#define RollL32_1(A) RollR32_3(A)
#define RollL32_2(A) RollR32_2(A)
#define RollL32_3(A) RollR32_1(A)

// Shift DWORDs
#define ShiftL32_1(A) (_mm_slli_si128(A, SL_ARGB_ARGB_ARGB_0000))
#define ShiftL32_2(A) (_mm_slli_si128(A, SL_ARGB_ARGB_0000_0000))
#define ShiftL32_3(A) (_mm_slli_si128(A, SL_ARGB_0000_0000_0000))

#define ShiftR32_1(A) (_mm_srli_si128(A, SR_0000_ARGB_ARGB_ARGB))
#define ShiftR32_2(A) (_mm_srli_si128(A, SR_0000_0000_ARGB_ARGB))
#define ShiftR32_3(A) (_mm_srli_si128(A, SR_0000_0000_0000_ARGB))

#define LoadU_32(A)(_mm_loadu_si128((DQWORD*)A))
#define LoadA_32(A)(_mm_load_si128((DQWORD*)A))
#define StoreU_32(A, B)(_mm_storeu_si128((DQWORD*)A, B))
#define StoreA_32(A, B)(_mm_store_si128((DQWORD*)A, B))

#define Set32(A, B, C, D)(_mm_set_epi32((int)A, (int)B, (int)C, (int)D))

// Replicate alpha channel
#define ReplicateAlpha(A) (ShuffleHi_Word(A, AAAA))
// End of SSE Integer Utils


// Used for store operations

union Vector2
{
	struct
	{
		float x, y;
	};
	struct
	{
		UINT b, g;
	};
	struct
	{
		UINT ix, iy;
	};
};

union Vector3
{
	struct
	{
		float x, y, z;
	};
	struct
	{
		UINT b, g, r;
	};
	struct
	{
		UINT ix, iy, iz;
	};
};

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

namespace SSE_Utils
{
	namespace Float4_Utils
	{
		// Load operations
		// To load Vector2/3/4 use the SSE vector classes		
		// Loads A into X position, sets the rest to 0.0f
		inline FLOAT4 _vectorcall LoadFloat(const float A)
		{
			return _mm_set_ss(A);
		}
		inline FLOAT4 _vectorcall LoadFloat2(const float A, const float B)
		{
			return _mm_set_ps(0.0f, 0.0f, B, A);
		}
		inline FLOAT4 _vectorcall LoadFloat3(const float A, const float B, const float C)
		{
			return _mm_set_ps(0.0f, C, B, A);
		}
		inline FLOAT4 _vectorcall LoadFloat4(const float A, const float B, const float C, const float D)
		{
			return _mm_set_ps(D, C, B, A);
		}
		

		// Logical operators
		inline FLOAT4 _vectorcall operator&(const FLOAT4 &A, const FLOAT4 &B)
		{
			return _mm_and_ps(A, B);
		}
		inline FLOAT4 _vectorcall operator|(const FLOAT4 &A, const FLOAT4 &B)
		{
			return _mm_or_ps(A, B);
		}
		inline FLOAT4 _vectorcall AndNot(const FLOAT4 &A, const FLOAT4 &B)
		{
			return _mm_andnot_ps(A, B);
		}

		// Arithmetic operator overloads
		inline FLOAT4 _vectorcall operator-(const FLOAT4 &A)
		{
			return _mm_sub_ps(_mm_setzero_ps(), A);
		}
		inline FLOAT4 _vectorcall operator+(const FLOAT4 &A, const FLOAT4 &B)
		{
			return _mm_add_ps(A, B);
		}
		inline FLOAT4 _vectorcall operator-(const FLOAT4 &A, const FLOAT4 &B)
		{
			return _mm_sub_ps(A, B);
		}
		inline FLOAT4 _vectorcall operator*(const FLOAT4 &A, const float B)
		{
			return _mm_mul_ps(A, _mm_set_ps1(B));
		}
		inline FLOAT4 _vectorcall operator/(const FLOAT4 &A, const float B)
		{
			FLOAT4 temp = _mm_set_ps1(B);
			return _mm_div_ps(A, temp);
		}

		// Vector multiply and divide
		inline FLOAT4 _vectorcall operator*(const FLOAT4 &A, const FLOAT4 &B)
		{
			return _mm_mul_ps(A, B);
		}
		inline FLOAT4 _vectorcall operator/(const FLOAT4 &A, const FLOAT4 &B)
		{
			return _mm_div_ps(A, B);
		}

		// Assignment plus arithmetic
		inline FLOAT4 &_vectorcall operator+=(FLOAT4 &A, const FLOAT4 &B)
		{
			A = _mm_add_ps(A, B);
			return A;
		}
		inline FLOAT4 &_vectorcall operator-=(FLOAT4 &A, const FLOAT4 &B)
		{
			A = _mm_sub_ps(A, B);
			return A;
		}
		inline FLOAT4 &_vectorcall operator*=(FLOAT4 &A, const FLOAT4 &B)
		{
			A = _mm_mul_ps(A, B);
			return A;
		}
		inline FLOAT4 &_vectorcall operator/=(FLOAT4 &A, const FLOAT4 &B)
		{
			FLOAT4 temp = _mm_rcp_ps(B);
			A = _mm_mul_ps(A, temp);
			return A;
		}

		// Compare operators
		inline FLOAT4 _vectorcall operator>(const FLOAT4 &A, const FLOAT4 &B)
		{
			return _mm_cmpgt_ps(A, B);
		}
		inline FLOAT4 _vectorcall operator<(const FLOAT4 &A, const FLOAT4 &B)
		{
			return _mm_cmplt_ps(A, B);
		}
		inline FLOAT4 _vectorcall operator==(const FLOAT4 &A, const FLOAT4 &B)
		{
			return _mm_cmpeq_ps(A, B);
		}
		inline FLOAT4 _vectorcall operator>=(const FLOAT4 &A, const FLOAT4 &B)
		{
			return _mm_cmpge_ps(A, B);
		}
		inline FLOAT4 _vectorcall operator<=(const FLOAT4 &A, const FLOAT4 &B)
		{
			return _mm_cmple_ps(A, B);
		}
		inline FLOAT4 _vectorcall operator!=(const FLOAT4 &A, const FLOAT4 &B)
		{
			return _mm_cmpneq_ps(A, B);
		}

		// Math functions
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
		inline FLOAT4 _vectorcall Min(const FLOAT4 &A, const FLOAT4 &B)
		{
			return _mm_min_ps(A, B);
		}
		inline FLOAT4 _vectorcall Max(const FLOAT4 &A, const FLOAT4 &B)
		{
			return _mm_max_ps(A, B);
		}
		inline FLOAT4 _vectorcall Abs(const FLOAT4 &A)
		{
			return Max(ZeroPS - A, A);
		}
		inline FLOAT4 _vectorcall RecipSqrRoot(const FLOAT4 &A)
		{
			return _mm_rsqrt_ps(A);
		}
		inline FLOAT4 _vectorcall SqrRoot(const FLOAT4 &A)
		{
			return _mm_sqrt_ps(A);
		}
		inline FLOAT4 _vectorcall Recip(const FLOAT4 &A)
		{
			return _mm_rcp_ps(A);
		}
#pragma region Shuffling
		// Shuffle overloads
		template<const unsigned int A, const unsigned int B,
			const unsigned int C, const unsigned int D>
			inline FLOAT4 _vectorcall Shuffle(const FLOAT4 &V0, const FLOAT4 &V1)
		{
			return _mm_shuffle_ps(V0, V1, _MM_SHUFFLE(D, C, B, A));
		}

		template<const unsigned int A, const unsigned int B,
			const unsigned int C, const unsigned int D>
			inline FLOAT4 _vectorcall Shuffle(const FLOAT4 &V)
		{
			return Shuffle<A, B, C, D>(V, V);
		}

		template<unsigned int A>
		inline FLOAT4 _vectorcall Shuffle(const FLOAT4 &V)
		{
			return _mm_shuffle_ps(V, V, A);
		}

		template<unsigned int A>
		inline FLOAT4 _vectorcall Shuffle(const FLOAT4 &V0, const FLOAT4 &V1)
		{
			return _mm_shuffle_ps(V0, V1, A);
		}

		// Component mirroring functions
		inline FLOAT4 _vectorcall MirrorXY(const FLOAT4 &V)
		{
			return Shuffle<XYYX>(V);
		}
		inline FLOAT4 _vectorcall MirrorXZ(const FLOAT4 &V)
		{
			return Shuffle<XZZX>(V);
		}
		inline FLOAT4 _vectorcall MirrorXW(const FLOAT4 &V)
		{
			Shuffle<XWWX>(V);
		}

		inline FLOAT4 _vectorcall MirrorYX(const FLOAT4 &V)
		{
			Shuffle<YXXY>(V);
		}
		inline FLOAT4 _vectorcall MirrorYZ(const FLOAT4 &V)
		{
			Shuffle<YZZY>(V);
		}
		inline FLOAT4 _vectorcall MirrorYW(const FLOAT4 &V)
		{
			Shuffle<YWWY>(V);
		}

		inline FLOAT4 _vectorcall MirrorZX(const FLOAT4 &V)
		{
			return Shuffle<ZXXZ>(V);
		}
		inline FLOAT4 _vectorcall MirrorZY(const FLOAT4 &V)
		{
			return Shuffle<ZYYZ>(V);
		}
		inline FLOAT4 _vectorcall MirrorZW(const FLOAT4 &V)
		{
			return Shuffle<ZWWZ>(V);
		}

		inline FLOAT4 _vectorcall MirrorWX(const FLOAT4 &V)
		{
			return Shuffle<WXXW>(V);
		}
		inline FLOAT4 _vectorcall MirrorWY(const FLOAT4 &V)
		{
			return Shuffle<WYYW>(V);
		}
		inline FLOAT4 _vectorcall MirrorWZ(const FLOAT4 &V)
		{
			return Shuffle<WZZW>(V);
		}

		// Replicate component functions
		inline FLOAT4 _vectorcall ReplicateXY(const FLOAT4 &V)
		{
			return Shuffle<XYXY>(V);
		}
		inline FLOAT4 _vectorcall ReplicateXZ(const FLOAT4 &V)
		{
			return Shuffle<XZXZ>(V);
		}
		inline FLOAT4 _vectorcall ReplicateXW(const FLOAT4 &V)
		{
			return Shuffle<XWXW>(V);
		}

		inline FLOAT4 _vectorcall ReplicateYX(const FLOAT4 &V)
		{
			return Shuffle<YXYX>(V);
		}
		inline FLOAT4 _vectorcall ReplicateYZ(const FLOAT4 &V)
		{
			return Shuffle<YZYZ>(V);
		}
		inline FLOAT4 _vectorcall ReplicateYW(const FLOAT4 &V)
		{
			return Shuffle<YWYW>(V);
		}

		inline FLOAT4 _vectorcall ReplicateZX(const FLOAT4 &V)
		{
			return Shuffle<ZXZX>(V);
		}
		inline FLOAT4 _vectorcall ReplicateZY(const FLOAT4 &V)
		{
			return Shuffle<ZYZY>(V);
		}
		inline FLOAT4 _vectorcall ReplicateZW(const FLOAT4 &V)
		{
			return Shuffle<ZWZW>(V);
		}

		inline FLOAT4 _vectorcall ReverseVector(const FLOAT4 &V)
		{
			return Shuffle<WZYX>(V);
		}

		// Same as ReplicateZW if V0 and V1 are the same
		inline FLOAT4 _vectorcall MoveHiLo(const FLOAT4 &V0, const FLOAT4 &V1)
		{
			return _mm_movehl_ps(V0, V1);
		}
		// Same as ReplicateXY if V0 and V1 are the same
		inline FLOAT4 _vectorcall MoveLoHi(const FLOAT4 &V0, const FLOAT4 &V1)
		{
			return _mm_movelh_ps(V0, V1);
		}
		inline FLOAT4 _vectorcall RollR(const FLOAT4 &V)
		{
			return Shuffle<WXYZ>(V);
		}
		inline FLOAT4 _vectorcall RollL(const FLOAT4 &V)
		{
			return Shuffle<YZWX>(V);
		}
#pragma endregion
	}
	namespace Dqword_Utils
	{
		inline DQWORD ExtractAlpha(const DQWORD &Color)
		{
			DQWORD alpha = ShuffleHi_Word(Color, AAAA);
			alpha = ShuffleLo_Word(alpha, AAAA);
			return alpha;
		}
		inline void UnpackColor(DQWORD &HiColor, DQWORD &LoColor, const DQWORD SrcColor)
		{
			HiColor = _mm_unpackhi_epi8(SrcColor, Zero128);
			LoColor = _mm_unpacklo_epi8(SrcColor, Zero128);
		}
		inline DQWORD AlphaBlend(const DQWORD &Color0, const DQWORD &Color1)
		{
			DQWORD alphaMask = Replicate16(255);

			DQWORD hiColorSrc, loColorSrc, hiColorDst, loColorDst;
			UnpackColor(hiColorSrc, loColorSrc, Color0);
			UnpackColor(hiColorDst, loColorDst, Color1);

			DQWORD srcAlphaHi = ExtractAlpha(hiColorSrc);
			DQWORD srcAlphaLo = ExtractAlpha(loColorSrc);
			DQWORD dstAlphaHi = _mm_sub_epi16(alphaMask, srcAlphaHi);
			DQWORD dstAlphaLo = _mm_sub_epi16(alphaMask, srcAlphaLo);

			
			DQWORD hiResultSrc = _mm_mullo_epi16(srcAlphaHi, hiColorSrc);
			DQWORD loResultSrc = _mm_mullo_epi16(srcAlphaLo, loColorSrc);
			DQWORD hiResultDst = _mm_mullo_epi16(dstAlphaHi, hiColorDst);
			DQWORD loResultDst = _mm_mullo_epi16(dstAlphaLo, loColorDst);
			
			DQWORD hiResult = _mm_add_epi16(hiResultSrc, hiResultDst);
			DQWORD loResult = _mm_add_epi16(loResultSrc, loResultDst);

			hiResult = _mm_srli_epi16(hiResult, 8);
			loResult = _mm_srli_epi16(loResult, 8);

			DQWORD result = _mm_packus_epi16(loResult, hiResult);

			return result;
		}
		inline DQWORD ColorKeyBlend(const DQWORD &Color0, const DQWORD &Color1, unsigned int ColorKey)
		{
			DQWORD key = Replicate16(ColorKey);

			DQWORD mask = _mm_cmpeq_epi32(Color0, key);
			DQWORD srcColor = _mm_and_si128(mask, Color0);
			DQWORD dstColor = _mm_andnot_si128(mask, Color1);
			DQWORD color = _mm_or_si128(srcColor, dstColor);

			return color;
		}
	}
}



#pragma once
#include "SSE_Utils.h"



class SSE
{
public:
	SSE()
		:
		A(_mm_setzero_si128())
	{
		
	}

	SSE(const DQWORD &Val)
		:
		A(Val)
	{

	}
	SSE(int A, int R, int G, int B)
		:
		A(_mm_set_epi32(B, G, R, A))
	{}
	SSE(int LoA, int LoR, int LoG, int LoB,
		int HiA, int HiR, int HiG, int HiB)
		:
		A(_mm_set_epi16(LoB, LoG, LoR, LoA, HiB, HiG, HiR, HiA))
	{}
	///// operations on the bytes of DQWORD

	// Arithmetic
	inline SSE _vectorcall AddBytes(const SSE &B)const
	{
		return _mm_add_epi8(A, B.A);
	}
	inline SSE _vectorcall SubtractBytes(const SSE &B)const
	{
		return _mm_sub_epi8(A, B.A);
	}
	inline SSE _vectorcall AddSignedSaturateBytes(const SSE &B)const
	{
		return _mm_adds_epi8(A, B.A);
	}
	inline SSE _vectorcall AddUnsignedSaturateBytes(const SSE &B)const
	{
		return _mm_adds_epu8(A, B.A);
	}
	inline SSE _vectorcall SubSignedSaturateBytes(const SSE &B)const
	{
		return _mm_subs_epi8(A, B.A);
	}
	inline SSE _vectorcall SubUnsignedSaturateBytes(const SSE &B)const
	{
		return _mm_subs_epu8(A, B.A);
	}
	inline SSE _vectorcall AverageBytes(const SSE &B)const
	{
		return _mm_avg_epu8(A, B.A);
	}
	inline SSE _vectorcall MaxBytes(const SSE &B)const
	{
		return _mm_max_epu8(A, B.A);
	}
	inline SSE _vectorcall MinBytes(const SSE &B)const
	{
		return _mm_min_epu8(A, B.A);
	}
	inline SSE _vectorcall SubAddBytes(const SSE &B)const
	{
		return _mm_sad_epu8(A, B.A);
	}

	// Comparison operators
	inline SSE _vectorcall IsEqualBytes(const SSE &B)const
	{
		return _mm_cmpeq_epi8(A, B.A);
	}
	inline SSE _vectorcall IsGreaterThanBytes(const SSE &B)const
	{
		return _mm_cmpgt_epi8(A, B.A);
	}
	inline SSE _vectorcall IsLessThanBytes(const SSE &B)const
	{
		return _mm_cmplt_epi8(A, B.A);
	}

	// Misc. operators
	inline SSE _vectorcall UnpackHiBytes(const SSE &B)const
	{
		return _mm_unpackhi_epi8(A, B.A);
	}
	inline SSE _vectorcall UnpackHiBytes()const
	{
		return _mm_unpackhi_epi8(A, _mm_setzero_si128());
	}
	inline SSE _vectorcall UnpackLoBytes(const SSE &B)const
	{
		return _mm_unpacklo_epi8(A, B.A);
	}
	inline SSE _vectorcall UnpackLoBytes()const
	{
		return _mm_unpacklo_epi8(A, _mm_setzero_si128());
	}
	///// End of Byte operations

	///// Word operations

	// Arithmetic operations on the words of DQWORD
	inline SSE _vectorcall AddWords(const SSE &B)const
	{
		return _mm_add_epi16(A, B.A);
	}
	inline SSE _vectorcall SubtractWords(const SSE &B)const
	{
		return _mm_sub_epi16(A, B.A);
	}
	inline SSE _vectorcall AddSignedSaturateWords(const SSE &B)const
	{
		return _mm_adds_epi16(A, B.A);
	}
	inline SSE _vectorcall AddUnsignedSaturateWords(const SSE &B)const
	{
		return _mm_adds_epu16(A, B.A);
	}
	inline SSE _vectorcall SubSignedSaturateWords(const SSE &B)const
	{
		return _mm_subs_epi16(A, B.A);
	}
	inline SSE _vectorcall SubUnsignedSaturateWords(const SSE &B)const
	{
		return _mm_subs_epu16(A, B.A);
	}
	inline SSE _vectorcall AverageWords(const SSE &B)const
	{
		return _mm_avg_epu16(A, B.A);
	}
	inline SSE _vectorcall MultiplyAddWords(const SSE &B)const
	{
		return _mm_madd_epi16(A, B.A);
	}
	inline SSE _vectorcall MaxWords(const SSE &B)const
	{
		return _mm_max_epi16(A, B.A);
	}
	inline SSE _vectorcall MinWords(const SSE &B)const
	{
		return _mm_min_epi16(A, B.A);
	}
	inline SSE _vectorcall MulSignedHiWords(const SSE &B)const
	{
		return _mm_mulhi_epi16(A, B.A);
	}
	inline SSE _vectorcall MulUnsignedHiWords(const SSE &B)const
	{
		return _mm_mulhi_epu16(A, B.A);
	}
	inline SSE _vectorcall MultiplyLoWords(const SSE &B)const
	{
		return _mm_mullo_epi16(A, B.A);
	}
	inline SSE _vectorcall SumOfAbsoluteDifferences(const SSE &B)const
	{
		return _mm_sad_epu8(A, B.A);
	}

	// Bit shifting
	inline SSE _vectorcall ShiftLeftIWords(int Count)const
	{
		return _mm_slli_epi16(A, Count);
	}
	inline SSE _vectorcall ShiftLeftWords(const SSE &Count)const
	{
		return _mm_sll_epi16(A, Count.A);
	}
	inline SSE _vectorcall ShiftRightSignedIWords(const int Count)const
	{
		return _mm_srai_epi16(A, Count);
	}
	inline SSE _vectorcall ShiftRightSignedWords(const SSE &Count)const
	{
		return _mm_sra_epi16(A, Count.A);
	}
	inline SSE _vectorcall ShiftRightIWords(const int Count)const
	{
		return _mm_srli_epi16(A, Count);
	}
	inline SSE _vectorcall ShiftRightWords(const SSE &Count)const
	{
		return _mm_srl_epi16(A, Count.A);
	}

	// Comparison operators
	inline SSE _vectorcall IsEqualToWords(const SSE &B)const
	{
		return _mm_cmpeq_epi16(A, B.A);
	}
	inline SSE _vectorcall IsGreaterThanWords(const SSE &B)const
	{
		return _mm_cmpgt_epi16(A, B.A);
	}
	inline SSE _vectorcall IsLessThanWords(const SSE &B)const
	{
		return _mm_cmplt_epi16(A, B.A);
	}

	// Misc. 
	inline SSE _vectorcall PackSignedSaturateWords(const SSE &B)const
	{
		return _mm_packs_epi16(A, B.A);
	}
	inline SSE _vectorcall PackUnsignedSaturateWords(const SSE &B)const
	{
		return _mm_packus_epi16(A, B.A);
	}
	inline SSE _vectorcall UnpackHiWords(const SSE &B)const
	{
		return _mm_unpackhi_epi16(A, B.A);
	}
	inline SSE _vectorcall UnpackLoWords(const SSE &B)const
	{
		return _mm_unpacklo_epi16(A, B.A);
	}
	inline SSE _vectorcall UnpackHiWords()const
	{
		return _mm_unpackhi_epi16(A, _mm_setzero_si128());
	}
	inline SSE _vectorcall UnpackLoWords()const
	{
		return _mm_unpacklo_epi16(A, _mm_setzero_si128());
	}
	///// End of Word operations

	///// DWord operations

	// Arithmetic operations on the words of DQWORD
	inline SSE _vectorcall AddDwords(const SSE &B)const
	{
		return _mm_add_epi32(A, B.A);
	}
	inline SSE _vectorcall SubtractDwords(const SSE &B)const
	{
		return _mm_sub_epi32(A, B.A);
	}
	inline SSE _vectorcall MultiplyDwords(const SSE &B)const
	{
		return _mm_mul_epu32(A, B.A);
	}

	// Bit shifting
	inline SSE _vectorcall ShiftLeftIDwords(const int Count)const
	{
		return _mm_slli_epi32(A, Count);
	}
	inline SSE _vectorcall ShiftLeftDwords(const SSE &Count)const
	{
		return _mm_sll_epi32(A, Count.A);
	}
	inline SSE _vectorcall ShiftRightSignedIDwords(int Count)const
	{
		return _mm_srai_epi32(A, Count);
	}
	inline SSE _vectorcall ShiftRightSignedDwords(const SSE &Count)const
	{
		return _mm_sra_epi32(A, Count.A);
	}
	inline SSE _vectorcall ShiftRightIDwords(const int Count)const
	{
		return _mm_srli_epi32(A, Count);
	}
	inline SSE _vectorcall ShiftRightDwords(const SSE &Count)const
	{
		return _mm_srl_epi32(A, Count.A);
	}

	// Comparison operators
	inline SSE _vectorcall IsEqualToDwords(const SSE &B)const
	{
		return _mm_cmpeq_epi32(A, B.A);
	}
	inline SSE _vectorcall IsGreaterThanDwords(const SSE &B)const
	{
		return _mm_cmpgt_epi32(A, B.A);
	}
	inline SSE _vectorcall IsLessThanDwords(const SSE &B)const
	{
		return _mm_cmplt_epi32(A, B.A);
	}

	///// End of dWord operations

	///// QWord operations
	///// Arithmetic operations
	inline SSE _vectorcall AddQwords(const SSE &B)const
	{
		return _mm_add_epi64(A, B.A);
	}
	inline SSE _vectorcall SubtractQwords(const SSE &B)const
	{
		return _mm_sub_epi64(A, B.A);
	}

	// Bit shifting
	inline SSE _vectorcall ShiftLeftIQwords(const int Count)const
	{
		return _mm_slli_epi64(A, Count);
	}
	inline SSE _vectorcall ShiftLeftQwords(const SSE &Count)const
	{
		return _mm_sll_epi64(A, Count.A);
	}
	inline SSE _vectorcall ShiftRightIQwords(const int Count)const
	{
		return _mm_srli_epi64(A, Count);
	}
	inline SSE _vectorcall ShiftRightQwords(const SSE &Count)const
	{
		return _mm_srl_epi64(A, Count.A);
	}
	///// End of qWord operations

	///// Common logical operators
	inline SSE _vectorcall operator&(const SSE &B)
	{
		return _mm_and_si128(A, B.A);
	}
	inline SSE _vectorcall IntAndNot(const SSE &B)const
	{
		return _mm_andnot_si128(A, B.A);
	}
	inline SSE _vectorcall operator|(const SSE &B)
	{
		return _mm_or_si128(A, B.A);
	}
	inline SSE _vectorcall IntOr(const SSE &B)const
	{
		return _mm_or_si128(A, B.A);
	}
	inline SSE _vectorcall IntXor(const SSE &B)const
	{
		return _mm_xor_si128(A, B.A);
	}


	DQWORD A;
};


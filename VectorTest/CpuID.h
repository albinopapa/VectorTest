#pragma once
#include <string>

enum Brand:unsigned
{
	Intel	= 0x00000000, 
	Amd		= 0x80000000
};
enum Common_ecx
{
	SSE3 = 1,
	SSSE3 = 1 << 9,
	FMA3 = 1 << 12,
	SSE4_1 = 1 << 19,
	SSE4_2 = 1 << 20,
	AVX = 1 << 28,
};
enum Common_edx
{
	SSE = 1 << 25,
	SSE2 = 1 << 26,
};
enum AmdEx_ecx
{
	SSE4a = 1 << 6,
	FMA4 = 1 << 16
};
enum AmdEx_edx
{

};
class CpuID
{
public:

	CpuID()
	{
		SetBrand();
	}

	~CpuID()
	{
	}

	void SetBrand()
	{
		int code[4]{};
		__cpuid(code, 0);

		char str[13]{};
		std::memcpy(&str[0], &code[1], 4);
		std::memcpy(&str[4], &code[3], 4);
		std::memcpy(&str[8], &code[2], 4);
		brandString.assign(str);
		auto res = brandString.find("AMD");
		if (res == 0)
		{
			brand = Brand::Intel;
		}
		else
		{
			brand = Brand::Amd;
		}
	}
	void SetFeatures()
	{
		int code[4]{};
		__cpuid(code, 1);

		eax = code[0];
		ebx = code[1];
		ecx = code[2];
		edx = code[3];
	}
	std::string brandString;
	Brand brand;
	int eax, ebx, ecx, edx;

};


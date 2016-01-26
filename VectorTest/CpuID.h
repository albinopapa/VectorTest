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
	SSE1 = 1 << 25,
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
		SetFeatures();
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

		__cpuid(code, brand + 1);
		xEax = code[0];
		xEbx = code[1];
		xEcx = code[2];
		xEdx = code[3];

	}
	bool SupportsSSE()
	{
		int t = (edx & SSE1);
		return t >> 25;
	}
	bool SupportsSSE2()
	{
		return (edx & SSE2) >> 26;
	}
	bool SupportsSSE3()
	{
		return (ecx & SSE3);
	}
	bool SupposrtsSSSE3()
	{
		return (ecx & SSSE3) >> 9;
	}
	bool SupportsSSE4a()
	{

		bool isAmd = brand == Amd;
		return (isAmd && ((xEcx & SSE4a) >> 6));
	}
	bool SupportsSSE41()
	{
		return (ecx & SSE4_1) >> 19;
	}
	bool SupportsSSE42()
	{
		return (ecx & SSE4_2) >> 20;
	}
	bool SupportsFMA3()
	{
		int t = (xEcx & FMA3);
		return t >> 12;
	}
	bool SupportsFMA4()
	{
		bool isAmd = brand == Amd;
		return (isAmd && ((xEcx & FMA4) >> 16));
	}
	bool SupportsAVX()
	{
		return (ecx & AVX) >> 28;
	}
	
private:
	std::string brandString;
	Brand brand;

	int eax, ebx, ecx, edx, xEax, xEbx, xEcx, xEdx;

};


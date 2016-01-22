#include "D3DGraphics.h"
#include "Bitmap.h"

D3DGraphics::D3DGraphics(std::shared_ptr<Window> &rWin)
	: pBackBuffer(NULL),
	win(rWin)
{
	pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, rWin->Handle(),
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE, &d3dpp, pDevice.GetAddressOf());

	pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	pSysBuffer.reset(new D3DCOLOR[rWin->Width() * rWin->Height()]);
}

D3DGraphics::~D3DGraphics()
{
}

void D3DGraphics::PutPixel(int x, int y, D3DCOLOR C)
{
	pSysBuffer[x + (y * win->Width())] = C;	
}

void D3DGraphics::BeginFrame()
{
	FillMemory(pSysBuffer.get(), win->Width() * win->Height() * sizeof(D3DCOLOR), 0x0);
	pBackBuffer->LockRect(&rect, NULL, NULL);
}

void D3DGraphics::EndFrame()
{
	pBackBuffer->UnlockRect();

	const UINT width = win->Width();
	const UINT height = win->Height();

	const UINT vidStride = rect.Pitch >> 2;
	D3DCOLOR *pVidBuffer = (D3DCOLOR *)rect.pBits;

	for (UINT y = 0; y < height; ++y)
	{
		UINT vidIndex = y * vidStride;
		UINT sysIndex = y * width;
		CopyMemory(&pVidBuffer[vidIndex], &pSysBuffer[sysIndex], sizeof(D3DCOLOR) * width);
	}

	pDevice->Present(NULL, NULL, NULL, NULL);
}

void LoadFont(Font* font, const char* filename,
	int charWidth, int charHeight, int nCharsPerRow)
{
	LoadBmp(filename, font->surface);
	font->charHeight = charHeight;
	font->charWidth = charWidth;
	font->nCharsPerRow = nCharsPerRow;
}

void D3DGraphics::DrawChar(char c, int xoff, int yoff, Font& font, D3DCOLOR color)
{
	const int sheetIndex = c - ' ';
	const int sCol = sheetIndex % font.nCharsPerRow;
	const int sRow = sheetIndex / font.nCharsPerRow;
	const int xStart = sCol * font.charWidth;
	const int yStart = sRow * font.charHeight;
	const int xEnd = xStart + font.charWidth;
	const int yEnd = yStart + font.charHeight;
	const int surfWidth = font.charWidth * font.nCharsPerRow;

	for (int y = yStart; y < yEnd; y++)
	{
		for (int x = xStart; x < xEnd; x++)
		{			
			if (font.surface[x + y * surfWidth] == D3DCOLOR_XRGB(0, 0, 0))
			{
				PutPixel(x + xoff - xStart, y + yoff - yStart, color);
			}
		}
	}
}

void D3DGraphics::DrawString(const std::string &Text, int X, int Y, Font &font, D3DCOLOR Color)
{
	int offset = font.charWidth;
	int i = 0;
	for (auto &c : Text)
	{
		DrawChar(c, (i * offset) + X, Y, font, Color);
		++i;
	}
}

void D3DGraphics::DrawSurface(int xoff, int yoff, int width, int height, D3DCOLOR key, const D3DCOLOR* surf)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			D3DCOLOR c = surf[x + y * width];

			if (c != key)
			{
				PutPixel(x + xoff, y + yoff, c);
			}
		}
	}
}

void D3DGraphics::DrawSurfaceAlpha(int X, int Y, int Width, int Height, const D3DCOLOR * Surface)
{
	LONGLONG xStart = max(-X, 0);
	LONGLONG xEnd = min(Width, win->Width() - Width);
	LONGLONG yStart = max(-Y, 0);
	LONGLONG yEnd = min(Height, win->Height() - Height);

	for (LONGLONG y = yStart; y < yEnd; ++y)
	{
		LONGLONG surfaceRowOffset = y * Width;
		LONGLONG backBufferRowOffset = (y + Y) * win->Width();
		for (LONGLONG x = xStart; x < xEnd; x += 4)
		{
			LONGLONG surfaceIndex = x + surfaceRowOffset;
			LONGLONG backBufferIndex = (x + X) + backBufferRowOffset;
			PDQWORD mSurface = (PDQWORD)&Surface[surfaceIndex];
			PDQWORD mBackbuffer = (PDQWORD)&pSysBuffer[backBufferIndex];

			DQWORD srcLo = SSE(mSurface[surfaceIndex]).UnpackLoBytes().A;
			DQWORD srcHi = SSE(mSurface[surfaceIndex]).UnpackHiBytes().A;

			DQWORD sAHi = ReplicateAlpha(srcHi);
			sAHi = ShuffleLo_Word(sAHi, AAAA);
			DQWORD sALo = ReplicateAlpha(srcLo);
			sALo = ShuffleLo_Word(sALo, AAAA);

			DQWORD dAHi = SSE(_mm_set1_epi16(255)).SubtractWords(sAHi).A;
			DQWORD dALo = SSE(_mm_set1_epi16(255)).SubtractWords(sALo).A;

			DQWORD rsHi = SSE(sAHi).MultiplyLoWords(srcHi).A;
			DQWORD rsLo = SSE(sALo).MultiplyLoWords(srcLo).A;

			DQWORD rdHi = SSE(dAHi).MultiplyLoWords(mBackbuffer[backBufferIndex]).A;
			DQWORD rdLo = SSE(dALo).MultiplyLoWords(mBackbuffer[backBufferIndex]).A;

			DQWORD rHi = SSE(rsHi).AddWords(rdHi).A;
			DQWORD rLo = SSE(rsLo).AddWords(rdLo).A;

			rHi = SSE(rHi).ShiftRightIWords(8).A;
			rLo = SSE(rLo).ShiftRightIWords(8).A;

			DQWORD r = SSE(rHi).PackUnsignedSaturateWords(rLo).A;
			_mm_storeu_si128(mBackbuffer, r);
		}
	}
}
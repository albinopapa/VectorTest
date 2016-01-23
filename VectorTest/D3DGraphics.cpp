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
	int xStart = max(-X, 0);
	int xEnd = min(Width, win->Width() - X - 1);
	int yStart = max(-Y, 0);
	int yEnd = min(Height, win->Height() - Y - 1);

	SSE alphaMask(_mm_set1_epi16(255));

	for (int y = yStart; y < yEnd; ++y)
	{
		int surfaceRowOffset = y * Width;
		int backBufferRowOffset = (y + Y) * win->Width();
		for (int x = xStart; x < xEnd; x += 4)
		{
			int surfaceIndex = x + surfaceRowOffset;
			int backBufferIndex = (x + X) + backBufferRowOffset;
			PDQWORD mSurfPixel = (PDQWORD)(&Surface[surfaceIndex]);
			PDQWORD mBackPixel = (PDQWORD)(&(pSysBuffer.get()[backBufferIndex]));

			DQWORD mSrcColor = *mSurfPixel;
			DQWORD mDstColor = *mBackPixel;

			// Unpack source pixels
			SSE srcLo(SSE(mSrcColor).UnpackLoBytes());
			SSE srcHi(SSE(mSrcColor).UnpackHiBytes());

			// Unpack destination pixels
			SSE dstLo(SSE(mDstColor).UnpackLoBytes());
			SSE dstHi(SSE(mDstColor).UnpackHiBytes());

			// Extract source alpha
			SSE sAHi(ShuffleHi_Word(srcHi.A, AAAA));
			sAHi.A = ShuffleLo_Word(sAHi.A, AAAA);
			SSE sALo(ShuffleHi_Word(srcLo.A, AAAA));
			sALo.A = ShuffleLo_Word(sALo.A, AAAA);

			// Subtract source alpha from 255 to get dst alpha
			SSE dAHi(alphaMask.SubtractWords(sAHi));
			SSE dALo(alphaMask.SubtractWords(sALo));

			// Multiply unpacked source pixels by source alpha
			SSE rsHi(sAHi.MultiplyLoWords(srcHi));
			SSE rsLo(sALo.MultiplyLoWords(srcLo));

			// Multiply unpacked dst pixels by dst alpha
			SSE rdHi(dAHi.MultiplyLoWords(dstHi));
			SSE rdLo(dALo.MultiplyLoWords(dstLo));

			// Add results of src and dst multiplies with each other
			SSE rHi(rsHi.AddWords(rdHi));
			SSE rLo(rsLo.AddWords(rdLo));

			// Shift right by 8
			rHi = rHi.ShiftRightIWords(8);
			rLo = rLo.ShiftRightIWords(8);

			// Pack pixels and store
			SSE r(rLo.PackUnsignedSaturateWords(rHi));
			_mm_storeu_si128(mBackPixel, r.A);
		}
	}
}
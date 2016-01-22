#pragma once

// C++ library includes
#include <memory>
#include <string>

// Win API library includes
#include <d3d9.h>
#include <wrl.h>
#include <DirectXMath.h>
// Aplication includes
#include "SSE.h"
#include "Window.h"

// Application typedefs
#if defined _M_X64
typedef LONGLONG Lint;
#elif
typedef LONG Lint ;
#endif // x64


struct Font {
	int charWidth;
	int charHeight;
	int nCharsPerRow;
	std::unique_ptr<D3DCOLOR[]> surface;
};

void LoadFont(Font* font, const char* filename, int charWidth, int charHeight, 
	int nCharsPerRow);

class D3DGraphics
{
public:
	D3DGraphics(std::shared_ptr<Window> &rWin);
	~D3DGraphics();
	void PutPixel(int x, int y, int r, int g, int b);
	void PutPixel(int x, int y, D3DCOLOR c);
	void BeginFrame();
	void EndFrame();
	void DrawChar(char c, int x, int y, Font& font, D3DCOLOR color);
	void DrawString(const std::string &Text, int X, int Y, Font &font, D3DCOLOR Color);
	void DrawSurface(int x, int y, int width, int height, D3DCOLOR key, const D3DCOLOR* surf);
	void DrawSurfaceAlpha(int X, int Y, int Width, int Height, const D3DCOLOR * Surface);


private:
	Microsoft::WRL::ComPtr<IDirect3D9>			pDirect3D;
	Microsoft::WRL::ComPtr<IDirect3DDevice9>	pDevice;
	Microsoft::WRL::ComPtr<IDirect3DSurface9>	pBackBuffer;
	std::shared_ptr<Window> win;
	std::unique_ptr<D3DCOLOR[]> pSysBuffer;
	D3DLOCKED_RECT rect;
};


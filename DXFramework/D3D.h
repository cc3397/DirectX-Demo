// D3D.h
// Defines and initialises a DirectX 11 renderer

#ifndef _D3D_H_
#define _D3D_H_

// Includes
#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class D3D
{
public:
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}

	D3D(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	~D3D();

	void beginScene(float r, float g, float b, float a);
	void endScene();

	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();

	XMMATRIX getProjectionMatrix();
	XMMATRIX getWorldMatrix();
	XMMATRIX getOrthoMatrix();

	// Control render states
	void setZBuffer(bool b);
	bool getZBufferState();


	void setAlphaBlending(bool b);
	bool getAlphaBlendingState();
	
	void setWireframeMode(bool b);
	bool getWireframeState();

	void setBackBufferRenderTarget();
	void resetViewport();

protected:
	bool vsync_enabled;
	bool isWirefameEnabled;
	
	bool zbufferState;
	bool wireframeState;
	bool alphaBlendState;

	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11RasterizerState* rasterState;
	ID3D11RasterizerState* rasterStateWF;
	XMMATRIX projectionMatrix;
	XMMATRIX worldMatrix;
	XMMATRIX orthoMatrix;
	ID3D11DepthStencilState* depthDisabledStencilState;
	ID3D11BlendState* alphaEnableBlendingState;
	ID3D11BlendState* alphaDisableBlendingState;
	D3D11_VIEWPORT viewport;
};

#endif
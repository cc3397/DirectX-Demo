// Base shader object.
// A shader object loads shader sets. Minimum set is vertex and pixel shader.
// Other shader stages can also be loaded; domain, hull and geomoetry.
// TODO: add computer shader to this list.
// Base shader is the parent for other custom shader objects. Offers required functions and a standard Matrix buffer.

#ifndef _BASESHADER_H_
#define _BASESHADER_H_

#include <d3d11.h>
#include <D3Dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <fstream>
#include "imgui.h"

using namespace std;
using namespace DirectX;


class BaseShader
{
protected:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}

	BaseShader(ID3D11Device* device, HWND hwnd);
	~BaseShader();

	virtual void render(ID3D11DeviceContext* deviceContext, int vertexCount);

protected:
	virtual void initShader(WCHAR*, WCHAR*) = 0;
	void loadVertexShader(WCHAR* filename);
	void loadHullShader(WCHAR* filename);
	void loadDomainShader(WCHAR* filename);
	void loadGeometryShader(WCHAR* filename);
	void loadPixelShader(WCHAR* filename);

protected:
	ID3D11Device* renderer;
	HWND hwnd;
	
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11HullShader* hullShader;
	ID3D11DomainShader* domainShader;
	ID3D11GeometryShader* geometryShader;
	ID3D11InputLayout* layout;
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
};

#endif
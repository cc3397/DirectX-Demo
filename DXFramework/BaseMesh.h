// BaseMesh.h
// Base mesh class. The parent of all mesh objects. Provides required functions
// including loading textures, returning index count and transmitting geometry to the GPU

#ifndef _BASEMESH_H_
#define _BASEMESH_H_

#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

class BaseMesh
{
protected:

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

public:
	BaseMesh();
	~BaseMesh();

	virtual void sendData(ID3D11DeviceContext* deviceContext);
	int getIndexCount();

protected:
	virtual void initBuffers(ID3D11Device*) = 0;

	ID3D11Buffer *vertexBuffer, *indexBuffer;
	int vertexCount, indexCount;
};

#endif
// CubeMesh.h
// Simple cube mesh object.
// Inherits from BaseMesh. Builds a simple cube with texture coordinates and normals.

#ifndef _CUBEMESH_H_
#define _CUBEMESH_H_

#include "BaseMesh.h"

using namespace DirectX;

class CubeMesh : public BaseMesh
{

public:
	CubeMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution = 20);
	~CubeMesh();
void sendData(ID3D11DeviceContext* deviceContext);
protected:
	void initBuffers(ID3D11Device* device);
	int resolution;
	
};

#endif
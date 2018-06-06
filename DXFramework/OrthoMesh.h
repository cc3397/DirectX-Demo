// OrthoMesh.h
// A simple 2D quad designed for orthographic rendering.
// While normals are set, they are not expected to be used.

#ifndef _ORTHOMESH_H_
#define _ORTHOMESH_H_

#include "basemesh.h"

using namespace DirectX;

class OrthoMesh : public BaseMesh
{

public:
	OrthoMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int width, int height, int xPosition = 0, int yPosition = 0);
	~OrthoMesh();

protected:
	void initBuffers(ID3D11Device*);
	int width, height, xPosition, yPosition;
};

#endif
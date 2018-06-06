// SquareMesh.h
// Simple single triangle mesh (for example purposes).
// Mesh contains texture coordinates and normals.

#ifndef _SQUAREMESH_H_
#define _SQUAREMESH_H_

//#include "BaseMesh.h"
#include "../DXFramework/DXF.h"


using namespace DirectX;

class SquareMesh: public BaseMesh
{

	public:
		SquareMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
		~SquareMesh();

	protected:
		void initBuffers(ID3D11Device* device);

	};


#endif


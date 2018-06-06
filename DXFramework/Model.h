// Model.h
// Simple (limited) .ojb loader.
// Acts like a mesh object, but loads the obj file based on provided filename
// TODO: replace with better model loader.

#ifndef _MODEL_H_
#define _MODEL_H_

#include "BaseMesh.h"
#include "TokenStream.h"
#include <vector>
#include <fstream>

using namespace DirectX;

class Model : public BaseMesh
{
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	Model(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename);
	~Model();

protected:
	void initBuffers(ID3D11Device* device);
	void loadModel(char* filename);
	
	ModelType* model;
};

#endif
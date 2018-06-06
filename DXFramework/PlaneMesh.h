// PlaneMesh.h
// Generates a plane mesh based on resolution.
// Resolution is the width and depth of the generated plane, built from unit quads.

#ifndef _PLANEMESH_H_
#define _PLANEMESH_H_

#include "BaseMesh.h"

class PlaneMesh : public BaseMesh
{

public:
	PlaneMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution = 100);
	~PlaneMesh();

protected:
	void initBuffers(ID3D11Device* device);
	int resolution;
};

#endif
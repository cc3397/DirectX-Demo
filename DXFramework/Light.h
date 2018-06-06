// Light class
// Represents a single light object. Storing ambient, diffuse and specular colour, specular power and direction/position.
// Additional generates view, projection and ortho matrices used in shadow mapping.

#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <directxmath.h>

using namespace DirectX;

class Light
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

	void generateViewMatrix();
	void generateProjectionMatrix(float screenNear, float screenFar);
	void generateOrthoMatrix(float screenWidth, float screenHeight, float near, float far);

	// Setters
	void setAmbientColour(float red, float green, float blue, float alpha);
	void setDiffuseColour(float red, float green, float blue, float alpha);
	void setDirection(float x, float y, float z);
	void setSpecularColour(float red, float green, float blue, float alpha);
	void setSpecularPower(float power);
	void setPosition(float x, float y, float z);
	void setLookAt(float x, float y, float z);

	// Getters
	XMFLOAT4 getAmbientColour();
	XMFLOAT4 getDiffuseColour();
	XMFLOAT3 getDirection();
	XMFLOAT4 getSpecularColour();
	float getSpecularPower();
	XMFLOAT3 getPosition();
	XMMATRIX getViewMatrix();
	XMMATRIX getProjectionMatrix();
	XMMATRIX getOrthoMatrix();


protected:
	XMFLOAT4 ambientColour;
	XMFLOAT4 diffuseColour;
	XMFLOAT3 direction;
	XMFLOAT4 specularColour;
	float specularPower;
	XMVECTOR position;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;
	XMMATRIX orthoMatrix;
	XMVECTOR lookAt; 
};

#endif
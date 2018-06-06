// Camera object.
// Allong with maintaining camera position and rotation generates both view matrices (perspective and orthographic).

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <directxmath.h>

using namespace DirectX;

class Camera
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

	Camera();
	~Camera();

	void setPosition(float lx, float ly, float lz);
	void setRotation(float lx, float ly, float lz);

	XMFLOAT3 getPosition();
	XMVECTOR getRotation();

	void update();
	XMMATRIX getViewMatrix();
	XMMATRIX getOrthoViewMatrix();

	void setFrameTime(float);

	void moveForward();
	void moveBackward();
	void moveUpward();
	void moveDownward();
	void turnLeft();
	void turnRight();
	void turnUp();
	void turnDown();
	void strafeRight();
	void strafeLeft();
	void turn(int x, int y);

private:
	//float positionX, positionY, positionZ;
	//float rotationX, rotationY, rotationZ;
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMMATRIX viewMatrix;
	XMMATRIX orthoMatrix;
	float speed, frameTime;
	float lookSpeed;
};

#endif
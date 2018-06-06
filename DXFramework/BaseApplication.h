// BaseApplication.h
// Contains base application functionality. For inheritance purposes.

#ifndef _BASEAPPLICATION_H_
#define _BASEAPPLICATION_H_

// Globals
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 200.0f;	// 1000.0f
const float SCREEN_NEAR = 0.1f;		//0.1f

// Includes
#include "input.h"
#include "d3d.h"
#include "camera.h"
#include "timer.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "TextureManager.h"

class BaseApplication
{
public:

	BaseApplication();
	~BaseApplication();
	virtual void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in);

	virtual bool frame();

protected:
	virtual void handleInput(float);
	virtual bool render() = 0;

protected:
	HWND wnd;				// handle to the window
	int sWidth, sHeight;	// stores screen width and height
	int deltax, deltay;		// for mouse movement
	POINT cursor;			// Used for converting mouse coordinates for client to screen space
	Input* input;			// input class
	D3D* renderer;
	Camera* camera;
	Timer* timer;
	TextureManager* textureMgr;
};

#endif
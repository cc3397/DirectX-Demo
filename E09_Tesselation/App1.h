// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/DXF.h"
#include "ColourShader.h"
#include "TessellationShader.h"
#include "DepthShader.h"
#include "ShadowShader.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in);

	bool frame();
	bool wireFrame;

protected:
	bool render();
	void gui();
	void RenderToTexture();
	float getDistance();

private:
	ColourShader* colourShader;
	TessellationShader* tesShader;
	SphereMesh* sphereMesh;
	PlaneMesh* planeMesh;
	ID3D11DeviceContext* deviceContext;
	DepthShader* depthShader;
	ShadowShader* shadowShader;
	Light* light1;
	RenderTexture* tesToTexture;//texture to store tesselated object to do shadow mapping
	RenderTexture* rendText;//texture to store depth info for shadow calcs
	bool wireframe;
	bool overRideTesselation;
	float height;
	float tesFactor;//manual tesselation control
	float distanceTes;//tesselation over distance
	float counter; // to be used in water simulation


	
};

#endif
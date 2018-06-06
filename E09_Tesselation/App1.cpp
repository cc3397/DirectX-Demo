// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{
	//BaseApplication::BaseApplication();
	sphereMesh = nullptr;
	planeMesh = nullptr;
	tesShader = nullptr;
	depthShader = nullptr;
	shadowShader = nullptr;
	colourShader = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	// Create Mesh objects
	sphereMesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
	planeMesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext(), 8);

	//init shaders
	tesShader = new TessellationShader(renderer->getDevice(), hwnd);//shader to deal with tesselation
	shadowShader = new ShadowShader(renderer->getDevice(), hwnd);//shader to produce shadows
	depthShader = new DepthShader(renderer->getDevice(), hwnd);//depth shader for shadow calculations

	//setup lighting
	light1 = new Light;
	light1->setDiffuseColour(0.8f, 0.8f, 0.3f, 1.0f);
	light1->setDirection(10.0f, 10.0f, 10.0f);
	light1->setAmbientColour(0.4f, 0.4f, 0.4f, 1.0f);
	light1->setPosition(0.0f, 10.0f, -10.0f);
	light1->setLookAt(0.0f, 0.0f, 0.0f);
	light1->generateProjectionMatrix(1.f, 100.f);

	//init render textures
	rendText = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);

	//imGUI variables
	wireFrame = false;// wireframe mode for use in Imgui
	height = 0.1; //height to be controlled by user
	tesFactor = 12; //tesselation factor to be controlled by user
	distanceTes = 12;//init to 12, changed in main loop
	counter = 0;//incremented each loop then fed to domain shader, allows for water simulation
	overRideTesselation = false;//alllow user to change tesselation factor, used in IMgui

	textureMgr->loadTexture("worldHeight", L"../res/worldHeight.png");//load height map for land
	textureMgr->loadTexture("waterHeight", L"../res/waterHeight.jpg");//load height map for simple water simulation
	textureMgr->loadTexture("waterTexture", L"../res/waterTexture.jpg");//load texture for water
	textureMgr->loadTexture("brick", L"../res/brick1.dds");

	deviceContext = renderer->getDeviceContext();
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (sphereMesh)
	{
		delete sphereMesh;
		sphereMesh = 0;
	}

	if (planeMesh)
	{
		delete planeMesh;
		planeMesh = 0;
	}

	if (colourShader)
	{
		delete colourShader;
		colourShader = 0;
	}

	if(tesShader)
	{
		delete tesShader;
		tesShader = 0;
	}

	if (shadowShader)
	{
		delete shadowShader;
		shadowShader = 0;
	}

	if (depthShader)
	{
		delete depthShader;
		depthShader = 0;
	}
}


bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}

	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	renderer->setWireframeMode(wireFrame);//to enable imgui to set wireframe
	
	//// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	//// Generate the view matrix based on the camera's position.
	camera->update();
	
	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();
	//// Send geometry data (from mesh)
	sphereMesh->sendData2(renderer->getDeviceContext());

	distanceTes = getDistance();//change the Tesfactor based on distance from world
	//// Set shader parameters (matrices and texture)
	if (overRideTesselation)//allow user to manually change tesselation level
	{
		tesShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("worldHeight"), textureMgr->getTexture("waterHeight"), textureMgr->getTexture("waterTexture"), height, tesFactor, counter);
	}
	else if (!overRideTesselation)
	{
		tesShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("worldHeight"), textureMgr->getTexture("waterHeight"), textureMgr->getTexture("waterTexture"), height, distanceTes, counter);
	}
	
	//// Render object (combination of mesh geometry and shader process
	tesShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());

    RenderToTexture();
	
	//render shadow 
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), rendText->getShaderResourceView(), light1);
	planeMesh->sendData(renderer->getDeviceContext());
	shadowShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());
	// Render GUI
	gui();

	if (counter <= 0.5)//feed in a value to modify texture co-ords by for the sampler, for water simulation
	{
	counter += 0.0001;
	}
	else
	{
		counter = 0;
	}
	
	//// Present the rendered scene to the screen.
	renderer->endScene();

	return true;
}

void App1::gui()
{
	// Force turn off on Geometry shader
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Checkbox("Wireframe", &wireFrame);//enable wireframe mode
	ImGui::Checkbox("Manual Tesselation Control", &overRideTesselation);//override tess over distance
	ImGui::SliderFloat("Tesselation Factor", &tesFactor, 1, 32, "Slide");//allow user to change tesselation factor
	ImGui::SliderFloat("Change Height", &height, 0, 0.4, "Slide");//allow user to change height of land

	// Render UI
	ImGui::Render();
}

float App1::getDistance()//return a tesselation factor for use in tess over distance
{
	float biggestVal;
	XMFLOAT3 cam;
	cam = camera->getPosition();
	
	//make all values positive for comparison
	if (cam.x < 0)
	{
		cam.x *= -1;
	}
	if (cam.y < 0)
	{
		cam.y *= -1;
	}
	if (cam.z < 0)
	{
		cam.z *= -1;
	}
	//check for the biggest value (the furthest away position)
	if (cam.x > cam.y && cam.x > cam.z)
	{
		biggestVal = cam.x;
	}
	else if(cam.y > cam.x && cam.y > cam.z)
	{
		biggestVal = cam.y;
	}
	else if (cam.z > cam.x && cam.z > cam.y)
	{
		biggestVal = cam.z;
	}
	else
	{
		biggestVal = cam.x; //if all other statements fail, then all values must be same, so set it to one of them
	}
	
	//set tes factor based on value
	if (biggestVal < 1)
	{
		return 12;
	}
	else if (biggestVal  < 2)
	{
		return 8;
	}
	else if (biggestVal < 5)
	{
		return 4;
	}
	else if (biggestVal < 10)
	{
		return 2;
	}
	else if (biggestVal < 15 || biggestVal >= 15)
	{
		return 1;
	}

}
void App1::RenderToTexture()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix;;
	rendText->setRenderTarget(renderer->getDeviceContext());
	rendText->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);
	camera->update();

	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	//send data to a mesh
	light1->generateViewMatrix();
	lightViewMatrix = light1->getViewMatrix();
	lightProjectionMatrix = light1->getProjectionMatrix();
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	planeMesh->sendData(renderer->getDeviceContext());
	depthShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());
	sphereMesh->sendData(renderer->getDeviceContext());//send data of sphere with correct structure
	depthShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());//render sphere shadow

	renderer->setBackBufferRenderTarget();//reset render target
	renderer->resetViewport();
}


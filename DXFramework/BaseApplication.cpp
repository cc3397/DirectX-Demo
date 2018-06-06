// BaseApplication.cpp
// Base application functionality for inheritnace.
#include "BaseApplication.h"


BaseApplication::BaseApplication()
{
}

// Release resources.
BaseApplication::~BaseApplication()
{

	if (timer)
	{
		delete timer;
		timer = 0;
	}

	if (camera)
	{
		delete camera;
		camera = 0;
	}

	if (renderer)
	{
		delete renderer;
		renderer = 0;
	}

	if (textureMgr)
	{
		delete textureMgr;
		textureMgr = 0;
	}
}

// Default application initialisation. Create renderer, camera, timer and imGUI objects.
void BaseApplication::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	input = in;
	wnd = hwnd;
	sWidth = screenWidth;
	sHeight = screenHeight;

	// Create the Direct3D renderer.
	renderer = new D3D(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!renderer)
	{
		MessageBox(hwnd, L"Could not initialize DirectX 11.", L"Error", MB_OK);
		exit(EXIT_FAILURE);
	}

	// Create the camera object and set to default position.
	camera = new Camera();
	camera->setPosition(0.0f, 0.0f, -10.0f);
	camera->update();

	// Create the timer object (for delta time and FPS calculation.
	timer = new Timer();

	// Initialise texture manager
	textureMgr = new TextureManager(renderer->getDevice(), renderer->getDeviceContext());
	textureMgr->loadTexture("default", L"../res/DefaultDiffuse.png");

	//Initialise ImGUI
	ImGui_ImplDX11_Init(wnd, renderer->getDevice(), renderer->getDeviceContext());
}

// Default frame processing. Check for escape key to exit, update timer, handle input and start UI.
bool BaseApplication::frame()
{
	if (input->isKeyDown(VK_ESCAPE) == true)
	{
		return false;
	}

	timer->frame();

	handleInput(timer->getTime());

	ImGui_ImplDX11_NewFrame();

	return true;
}


void BaseApplication::handleInput(float frameTime)
{
	// Set the frame time for calculating the updated position.
	camera->setFrameTime(frameTime);

	// Handle the input.
	if (input->isKeyDown('W'))
	{
		// forward
		camera->moveForward();
	}
	if (input->isKeyDown('S'))
	{
		// back
		camera->moveBackward();
	}
	if (input->isKeyDown('A'))
	{
		// Strafe Left
		camera->strafeLeft();
	}
	if (input->isKeyDown('D'))
	{
		// Strafe Right
		camera->strafeRight();
	}
	if (input->isKeyDown('Q'))
	{
		// Down
		camera->moveDownward();
	}
	if (input->isKeyDown('E'))
	{
		// Up
		camera->moveUpward();
	}
	if (input->isKeyDown(VK_UP))
	{
		// rotate up
		camera->turnUp();
	}
	if (input->isKeyDown(VK_DOWN))
	{
		// rotate down
		camera->turnDown();
	}
	if (input->isKeyDown(VK_LEFT))
	{
		// rotate left
		camera->turnLeft();
	}
	if (input->isKeyDown(VK_RIGHT))
	{
		// rotate right
		camera->turnRight();
	}

	if (input->isMouseActive())
	{
		// mouse look is on
		deltax = input->getMouseX() - (sWidth / 2);
		deltay = input->getMouseY() - (sHeight / 2);
		camera->turn(deltax, deltay);
		cursor.x = sWidth / 2;
		cursor.y = sHeight / 2;
		ClientToScreen(wnd, &cursor);
		SetCursorPos(cursor.x, cursor.y);
	}

	if (input->isKeyDown(VK_SPACE))
	{
		// re-position cursor
		cursor.x = sWidth / 2;
		cursor.y = sHeight / 2;
		ClientToScreen(wnd, &cursor);
		SetCursorPos(cursor.x, cursor.y);
		input->setMouseX(sWidth / 2);
		input->setMouseY(sHeight / 2);
		input->SetKeyUp(VK_SPACE);
		// if space pressed toggle mouse
		input->setMouseActive(!input->isMouseActive());
		if (!input->isMouseActive())
		{
			ShowCursor(true);
		}
		else
		{
			ShowCursor(false);
		}
	}

	
}

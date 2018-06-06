// System.h
// Handles the setup and initialisation of the window.
// Captures window events.

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#define WIN32_LEAN_AND_MEAN

// INCLUDES 
#include <windows.h>
#include "baseapplication.h"
#include "Input.h"

class System
{
public:
	System(BaseApplication* application);
	~System();

	void run();

	LRESULT CALLBACK messageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool frame();
	void initialiseWindows(int&, int&);
	void shutdownWindows();

private:
	LPCWSTR applicationName;
	HINSTANCE hinstance;
	HWND hwnd;
	BaseApplication* app;
	Input input;

	//Prototypes
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
};

//Globals
static System* ApplicationHandle = 0;

#endif
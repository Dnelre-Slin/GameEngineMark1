#pragma once
// This is taken from a tutorial, but seems to have no real use. Not sure why we would need this. Can possibly remove
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_


// If this is defined, then Windows.h will skip include of a bunch of files
// see https://stackoverflow.com/questions/11040133/what-does-defining-win32-lean-and-mean-exclude-exactly
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Input/Input.h"
#include "Graphics/Graphics.h"
#include "Timer/Timer.h"
#include "Threading/ThreadPoolManager.h"

/**
 * Main class for running the game system
 */
class System
{
public:
	System();
	System(const System&);
	~System();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM) const;

private:
	bool Input() const;
	bool Timer();
	bool Physics();
	bool Render();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_ApplicationName;
	HINSTANCE m_Hinstance;
	HWND m_Hwnd;

	InputClass* m_Input;
	Graphics* m_Graphics;

	GameTime m_Timer;

	ThreadPoolManager m_ThreadPoolManager;
};


/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


/////////////
// GLOBALS //
/////////////
static System* ApplicationHandle = nullptr;


#endif
#include "System.h"
#include <iostream>
#include <chrono>

#include "Threading/ThreadJob.h"

void timerAction(int ms) {
	std::cout << "Ran action as " << ms << std::endl;
}

class test : public ThreadJob
{
	int Execute() const override
	{
		int n = 0;

		while (n++ < 10)
		{
			std::cout << "Doing some work in test 1, it " << n << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
		return 0;
	}
};

class test2 : public ThreadJob
{
	int Execute() const override
	{
		int n = 0;

		while (n++ < 10)
		{
			std::cout << "Doing some work in test 2, it " << n << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
		return 0;
	}
};

class test3 : public ThreadJob
{
	int Execute() const override
	{
		int n = 0;

		while (n++ < 10)
		{
			std::cout << "Time elapsed: " << GameTime::GetTimeElapsedSinceStart<Milliseconds>() << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(4000));
		}
		return 0;
	}
};

System::System()
{
	m_ApplicationName = L"GameEngineMark1";
	m_Hinstance = nullptr;
	m_Hwnd = nullptr;
	m_Graphics = nullptr;
	m_Input = nullptr;
	m_Graphics = nullptr;

	// test* a = new test();
	// test2* b = new test2();
	const test3* c = new test3();
	
	// m_ThreadPoolManager.Run(a);
	// m_ThreadPoolManager.Run(b);
	m_ThreadPoolManager.Run(c);


	/*m_Timer.QueueAction(5000, []() {timerAction(5); });
	m_Timer.QueueAction(12000, []() {timerAction(12); });
	m_Timer.QueueAction(700,[]() {timerAction(0.7); });*/
}



System::System(const System& other)
{
	m_ApplicationName = L"GameEngineMark1";
	m_Hinstance = nullptr;
	m_Hwnd = nullptr;
	m_Graphics = nullptr;
	m_Input = nullptr;
	m_Graphics = nullptr;
}


System::~System()
{
}


bool System::Initialize()
{
	// Initialize the width and height of the screen to zero before sending the variables into the function.
	int screenHeight = 0;
	int screenWidth = 0;

	// Initialize the windows api.
	InitializeWindows(screenHeight, screenWidth);

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_Input = new InputClass;
	if(!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	m_Input->Initialize();

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_Graphics = new Graphics;
	if(!m_Graphics)
	{
		return false;
	}

	// Initialize the graphics object.
	if(!m_Graphics->Initialize(screenHeight, screenWidth, m_Hwnd))
	{
		return false;
	}
	
	return true;
}


void System::Shutdown()
{
	// Release the graphics object.
	if(m_Graphics != nullptr)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = nullptr;
	}

	// Release the input object.
	if(m_Input != nullptr)
	{
		delete m_Input;
		m_Input = nullptr;
	}

	m_ThreadPoolManager.ShutDown();
	// Shutdown the window.
	ShutdownWindows();
}


void System::Run()
{
	MSG Msg;

	// Initialize the message structure.
	ZeroMemory(&Msg, sizeof(MSG));
	
	// Loop until there is a quit message from the window or the user.
	bool done = false;
	while(!done)
	{
		// Handle the windows messages.
		if(PeekMessage(&Msg, m_Hwnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}

		// If windows signals to end the application then exit out.
		if(Msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			

			bool result = Input() &&
				Timer() &&
				Physics() &&
				Render();

			if(!result)
			{
				done = true;
			}
		}

	}

	return;
}


bool System::Render()
{
	// Do the frame processing for the graphics object.
	//auto r = m_Graphics->Frame();
	//mutex_unlock(render);
	//return r;
	return m_Graphics->Frame();
}

bool System::Timer() {
	return m_Timer.Tick();
}

bool System::Physics() {
	return true;
}

bool System::Input() const
{
	
	// Check if the user pressed escape and wants to exit the application.
	if(m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	return true;
}


LRESULT CALLBACK System::MessageHandler(HWND Hwnd, UINT Umsg, WPARAM Wparam, LPARAM Lparam) const
{
	switch(Umsg)
	{
		// Check if a key has been pressed on the keyboard.
		case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			m_Input->KeyDown(static_cast<unsigned>(Wparam));
			return 0;
		}

		// Check if a key has been released on the keyboard.
		case WM_KEYUP:
		{
			// If a key is released then send it to the input object so it can unset the state for that key.
			m_Input->KeyUp(static_cast<unsigned>(Wparam));
			return 0;
		}

		// Any other messages send to the default message handler as our application won't make use of them.
		default:
		{
			return DefWindowProc(Hwnd, Umsg, Wparam, Lparam);
		}
	}
}


void System::InitializeWindows(int& screenHeight, int& screenWidth)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	m_Hinstance = GetModuleHandle(nullptr);

	// Give the application a name.
	m_ApplicationName = L"Mark 1.0";

	// Setup the windows class with default settings.
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = m_Hinstance;
	wc.hIcon		 = LoadIcon(nullptr, IDI_WINLOGO);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName  = nullptr;
	wc.lpszClassName = m_ApplicationName;
	wc.cbSize        = sizeof(WNDCLASSEX);
	
	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenHeight = GetSystemMetrics(SM_CYSCREEN);
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsHeight = static_cast<unsigned long>(screenHeight);
		dmScreenSettings.dmPelsWidth  = static_cast<unsigned long>(screenWidth);
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth  = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_Hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_ApplicationName, m_ApplicationName,
						    WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
						    posX, posY, screenWidth, screenHeight, NULL, NULL, m_Hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_Hwnd, SW_SHOW);
	SetForegroundWindow(m_Hwnd);
	SetFocus(m_Hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}


void System::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(nullptr, 0);
	}

	// Remove the window.
	DestroyWindow(m_Hwnd);
	m_Hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_ApplicationName, m_Hinstance);
	m_Hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}

#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL.h"

class Application;

class Window : public Module
{
public:

	Window(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~Window();

	bool Init();
	bool CleanUp();
	void WindowBrightness(float value);
	void WindowResize(int width, int height);
	void WindowSetFullscreen();
	void WindowSetWindowed();
	void WindowBorderless(bool win = false);
	void WindowResizable(bool res = true);
	void GetDesktopResolution(int& horizontal, int& vertical);
	void SetTitle(const char* title);
	void UpdateWindowSize();

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;


	SDL_GLContext gl_context;
};

#endif // __ModuleWindow_H__
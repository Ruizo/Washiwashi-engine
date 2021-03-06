#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "External/SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool startEnabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);
	void SetFullscreen(bool fullscreen);
	void SetWindowSize();
	void SetWindowBrightness();
	void SetResizable(bool resizable);
	void SetBorderless(bool resizable);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;
};

#endif // __ModuleWindow_H__
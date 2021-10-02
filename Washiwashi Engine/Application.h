#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "External/Parson/parson.h"
#include "External/MathGeoLib/include/MathGeoLib.h"

#include <shellapi.h>


class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleEditor* editor;

private:

	Timer	ms_timer;
	float	dt;
	p2List<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	bool RequestBrowser(const char* url);

	void Save();
	void Load();

	int maxFPS = 60;
	float brightness = 1.0f;
	int width = 1280;
	int height = 1024;

	bool fullscreen = false;
	bool resizable = true;
	bool borderless = false;
	bool fullscreenDesktop = false;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};
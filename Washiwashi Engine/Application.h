#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "External/Parson/parson.h"
#include "External/MathGeoLib/include/MathGeoLib.h"
#include "ModuleFileSystem.h"

#include <shellapi.h>

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleScene* scene;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleEditor* editor;
	ModuleFileSystem* fileManager;

private:
	Timer ms_timer;
	float dt;
	p2List<Module*> list_modules;

public:
	Application();
	~Application();

	bool Init();
	UpdateStatus Update();
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
#pragma once
#include "Module.h"
#include "Globals.h"
#include "ModuleFileSystem.h"
#include "External/imgui/imgui.h"
#include "External/imgui/backends/imgui_impl_sdl.h"
#include "External/imgui/backends/imgui_impl_opengl3.h"
#include "LoadMesh.h"

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool startEnabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt);

public:
	char* path = nullptr;
	Mesh mesh;
	bool CleanUp();
};

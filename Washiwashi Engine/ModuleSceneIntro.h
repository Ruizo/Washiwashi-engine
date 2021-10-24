#pragma once
#include "Module.h"
#include "Globals.h"
#include "ModuleFileSystem.h"
#include "External/imgui/imgui.h"
#include "External/imgui/backends/imgui_impl_sdl.h"
#include "External/imgui/backends/imgui_impl_opengl3.h"
#include "LoadMesh.h"

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	char* path = nullptr;
	Mesh mesh;
	bool CleanUp();
public:
};

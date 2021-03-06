#pragma once
#include "Module.h"
#include "Globals.h"
#include "ModuleFileSystem.h"
#include "External/imgui/imgui.h"
#include "External/imgui/backends/imgui_impl_sdl.h"
#include "External/imgui/backends/imgui_impl_opengl3.h"
#include "LoadMesh.h"
#include "glmath.h"

class GameObject;

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool startEnabled = true);
	~ModuleScene();

	bool Start();
	UpdateStatus Update(float dt);
	bool CleanUp();

	GameObject* SpawnGameObject(const char* name, GameObject* parent);

public:
	char* path = nullptr;
	GameObject* root = nullptr;
};

#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "External/imgui/imgui.h"
#include "External/imgui/backends/imgui_impl_sdl.h"
#include "External/imgui/backends/imgui_impl_opengl3.h"
#include <gl/GL.h>
#include <vector>

class GameObject;

class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool startEnabled = true);
	~ModuleEditor();

	bool Start();
	UpdateStatus Update(float dt);
	bool CleanUp();

public:
	bool showDemoWindow = false;
	bool showAboutWindow = false;
	bool showOptionsWindow = false;
	bool showHierarchyTab = false;
	bool showInspectorTab = false;
	bool active = false;
	bool console = false;
	ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// ----- Inputs -----
	char name[25];

	// ----- FPS/MS Graph -----
	char title[25];
	std::vector<float> fps_log;
	std::vector<float> ms_log;

	void FPSGraph(float dt, int size);
	void MSGraph(float dt, int size);

	// ----- Game Objects -----
	void HierarchyListTree(GameObject& go);

};

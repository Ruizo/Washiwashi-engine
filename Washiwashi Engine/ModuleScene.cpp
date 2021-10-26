#include "Globals.h"
#include "Primitive.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"

ModuleScene::ModuleScene(Application* app, bool startEnabled) : Module(app, startEnabled)
{
}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{
	WASHI_LOG("Loading Scene assets");

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	
	return true;
}

// Load assets
bool ModuleScene::CleanUp()
{
	WASHI_LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
UpdateStatus ModuleScene::Update(float dt)
{
	Primitive::Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	Primitive::Cube c;
	c.SetPos(2, 2, 2);
	c.Render();

	mesh.Render();

	return UPDATE_CONTINUE;
}


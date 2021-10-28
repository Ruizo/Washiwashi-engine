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
	
	root = CreateGameObject("GameObject Scene root", nullptr);

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

GameObject* ModuleScene::CreateGameObject(const char* name, GameObject* parent, int uid)
{
	GameObject* go = new GameObject(name, parent, uid);
	return go;
}

// Update: draw background
UpdateStatus ModuleScene::Update(float dt)
{
	for (int i = 0; i < root->children.size(); i++)
	{
		root->children.at(i)->Update();
	}

	
	Primitive::Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	return UPDATE_CONTINUE;
}


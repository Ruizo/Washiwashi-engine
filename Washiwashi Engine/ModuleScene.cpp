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
	
	root = SpawnGameObject("GameObject Scene root", nullptr);

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	GameObject* bakerHouse = SpawnGameObject("Baker House", root);
	bakerHouse->CreateComponent(Component::Type::MESH);
	bakerHouse->LocateComponent("Engine/Library/Meshes/Prefabs/ComplexShapes/baker_house.fbx", Component::Type::MESH);
	bakerHouse->CreateComponent(Component::Type::TEXTURE);
	bakerHouse->LocateComponent("Engine/Library/Materials/texture.dds", Component::Type::TEXTURE);

	return true;
}

// Load assets
bool ModuleScene::CleanUp()
{
	WASHI_LOG("Unloading Intro scene");

	return true;
}

GameObject* ModuleScene::SpawnGameObject(const char* name, GameObject* parent)
{
	GameObject* go = new GameObject(name, parent);
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


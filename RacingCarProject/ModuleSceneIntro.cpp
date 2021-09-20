#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->audio->PlayMusic("Assets/Songs/Deja Vu.ogg");

	

	//SpeedCube on level 1
	Cube speed_cube(1, 1, 1);
	speedCube = App->physics->AddBody(speed_cube, 0.0f);
	speedCube->collision_listeners.add(this);
	speedCube->SetPos(0, 0, 0);

	//Camera
	App->camera->Move(vec3(-2.0f, 4.0f, -5.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{

	//Level 1 speed cube render
	Cube speed_cube(1, 1, 1);
	speed_cube.SetPos(0, 0, 0);
	speed_cube.color = Green;
	speed_cube.Render();

	Plane p(0, 1, 0, 0);

	p.axis = true;
	p.Render();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{



}


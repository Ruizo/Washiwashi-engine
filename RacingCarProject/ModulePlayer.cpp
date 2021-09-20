#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	
	VehicleInfo car;


	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 2, 6);
	car.chassis_offset.Set(0, 1.5f, 0);
	car.mass = 500.0f;
	car.suspensionStiffness = 20.88f; //15.88f
	car.suspensionCompression = 1.83f; //0.83f
	car.suspensionDamping = 1.88f; //0.88f
	car.maxSuspensionTravelCm = 1000.0f; //100.0f
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.6f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	
	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(0, 0, -100);
	vehicle->body->setUserPointer(vehicle);
	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;
	if (stop == false) 
	{
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			if (acceleration < 0.0f)
			{
				acceleration = MAX_ACCELERATION + BRAKE_POWER(3);
			}
			if (acceleration >= 0.0f)
			{				
				if (speedCube == true)
				{
					//Acceleration boost with speed cube
					acceleration = 20000.0f;
					speedCube = false;
				}
				else
				{
					acceleration = MAX_ACCELERATION;
				}
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			if (turn < TURN_DEGREES)
				turn += TURN_DEGREES;
		}

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			if (turn > -TURN_DEGREES)
				turn -= TURN_DEGREES;
		}

		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			if (acceleration > 0.0f)
			{
				acceleration = -MAX_ACCELERATION - BRAKE_POWER(3);
			}
			if (acceleration <= 0.0f)
			{
				acceleration = -MAX_ACCELERATION;
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		{
			brake = BRAKE_POWER(3);
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		//When R is pressed, the car stops immediately and resets its position depending on the level you are currently
		stop = false;
		vehicle->Turn(90);
		vehicle->vehicle->resetSuspension();
		if (vehicle->body->getLinearVelocity() != 0)
		{
			vehicle->body->setAngularVelocity({ 0,0,0 });
		}
		if (vehicle->GetKmh() != 0)
			vehicle->body->setLinearVelocity({ 0,0,0 });
		if (resets <= 10 && resets > 0)
		{
			resets = resets - 1;

			if (level1 == true)
			{
				vehicle->SetPos(0, 0, -100);
			}
			if (level2 == true)
			{
				vehicle->SetPos(25, 0, -100);
			}
			if (level3 == true)
			{
				vehicle->SetPos(50, 30, -85);
			}
			if (level4 == true)
			{
				vehicle->SetPos(70, 0, -85);
			}
			if (win == true)
			{
				vehicle->SetPos(0, 0, -100);
				win = false;
				resets = 10;
				stop = false;
			}
		}
		else
		{
			vehicle->SetPos(0, 0, -100);
			resets = 10;
		}

	}
	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
	{
		//T does the same as R but it is a hard reset if the car is stuck face down so it can reset properly
		stop = false;
		vehicle->Turn(90);
		vehicle->vehicle->resetSuspension();
		if (vehicle->body->getLinearVelocity() != 0)
		{
			vehicle->body->setAngularVelocity({ 0,0,0 });
		}
		if (vehicle->GetKmh() != 0)
			vehicle->body->setLinearVelocity({ 0,0,0 });
		if (resets <= 10 && resets > 0)
		{
			resets = resets - 1;

			if (level1 == true)
			{
				vehicle->SetPos(0, 6, -100);
			}
			if (level2 == true)
			{
				vehicle->SetPos(25, 6, -100);
			}
			if (level3 == true)
			{
				vehicle->SetPos(50, 36, -85);
			}
			if (level4 == true)
			{
				vehicle->SetPos(70, 6, -85);
			}
			if (win == true)
			{
				vehicle->SetPos(0, 6, -100);
				win = false;
				resets = 10;
				stop = false;
			}
		}
		else
		{
			vehicle->SetPos(0, 0, -100);
			resets = 10;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		//F2 skips your current level
		if (level1 == true) 
		{
			win = false;
			level1 = false;
			level2 = true;
			vehicle->SetPos(25, 0, -100);
		}
		else if (level2 == true)
		{
			level2 = false;
			level3 = true;
			vehicle->SetPos(50, 30, -85);
		}
		else if (level3 == true)
		{
			level3 = false;
			level4 = true;
			vehicle->SetPos(70, 0, -85);
		}
		else if (level4 == true)
		{
			win = true;
			level4 = false;
			level0 = true;
			vehicle->SetPos(1000, 4, 0);
		}
		else if (level0 == true)
		{
			win = false;
			level0 = false;
			level1 = true;
			vehicle->SetPos(0, 0, -100);
		}
	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();

	if (win == false)
	{
		//Camera of the car
		vec3 myCamera;
		myCamera.x = vehicle->body->getCenterOfMassPosition().getX() + vehicle->vehicle->getForwardVector().x() * -10;
		myCamera.y = vehicle->body->getCenterOfMassPosition().getY() + vehicle->vehicle->getForwardVector().y() + 5;
		myCamera.z = vehicle->body->getCenterOfMassPosition().getZ() + vehicle->vehicle->getForwardVector().z() * -10;
		App->camera->Position = myCamera;
		char title[100];
		sprintf_s(title, "%.1f Km/h || Resets left: %d", vehicle->GetKmh(), resets);
		App->window->SetTitle(title);



		App->camera->LookAt(vec3(vehicle->body->getCenterOfMassPosition().getX(), vehicle->body->getCenterOfMassPosition().getY(), vehicle->body->getCenterOfMassPosition().getZ()));
	}
	else
	{
		App->camera->Position = vec3(1025, 5, 50);
		App->camera->LookAt(vec3(1000, 0, 0));
		char title[100];
		sprintf_s(title, "Congratulations! You beat the game! Press F2 two times to restart");
		App->window->SetTitle(title);
	}

	

	
	

	return UPDATE_CONTINUE;
}




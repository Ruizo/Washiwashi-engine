#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 800.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER(x) (((float)x * 1000.0f))

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:
	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;
	bool stop = false;
	bool speedCube = false;
	int resets = 10;

	bool level1 = true;
	bool level2 = false;
	bool level3 = false;
	bool level4 = false;
	bool level0 = false;
	bool win = false;
};
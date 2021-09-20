#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);


public:
	/*
	PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];
	*/

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	PhysBody3D* cube;
	PhysBody3D* cube2;
	PhysBody3D* cube3;
	PhysBody3D* cube4;
	PhysBody3D* cube5;
	PhysBody3D* cube6;
	PhysBody3D* cube7;
	PhysBody3D* cube8;
	PhysBody3D* cube9;
	PhysBody3D* cube10;
	PhysBody3D* cube11;
	PhysBody3D* cube12;
	PhysBody3D* cube13;
	PhysBody3D* cube14;
	PhysBody3D* cube15;
	PhysBody3D* cube16;
	PhysBody3D* cube17;
	PhysBody3D* cube18;
	PhysBody3D* cube19;
	PhysBody3D* cube20;	
	PhysBody3D* cube21;
	PhysBody3D* cube22;
	PhysBody3D* cube23;
	PhysBody3D* cube24;
	PhysBody3D* cube25;
	PhysBody3D* cube26;
	PhysBody3D* cube27;
	PhysBody3D* cube28;
	PhysBody3D* cube29;
	PhysBody3D* cube30;
	PhysBody3D* cube31;
	PhysBody3D* cube32;




	PhysBody3D* speedCube;
	PhysBody3D* speedCube2;
	PhysBody3D* speedCube3;
	PhysBody3D* speedCube4;
	PhysBody3D* speedCube5;
	PhysBody3D* sensor;
	PhysBody3D* sensor2;
	PhysBody3D* sensorLevel2;
	PhysBody3D* sensorLevel3;
	PhysBody3D* sensorLevel4;
	PhysBody3D* sensorStop;
	PhysBody3D* sensorStop2;
	PhysBody3D* sensorStop3;
	PhysBody3D* sensorStop4;


};

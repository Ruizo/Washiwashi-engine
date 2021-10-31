#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool startEnabled) : Module(app, startEnabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	WASHI_LOG("Setting up the camera");

	return true;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	WASHI_LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
UpdateStatus ModuleCamera3D::Update(float dt)
{
	vec3 newPos(0,0,0);
	float camSpeed = speed * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		camSpeed = speed * 2 * dt;
	// Mouse motion ----------------

	if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.4f;

		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos.y += camSpeed;
		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos.y -= camSpeed;

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * camSpeed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * camSpeed;

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * camSpeed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * camSpeed;

		Position -= Reference;

		if(dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if(dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if(Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

		Position = Reference + Z * length(Position);
	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) //WE NEED TO HANDLE THE CRASH!!!!!!!!! IT CRASHES WHEN THERE'S NO SELECTED GAME OBJECT!!!!!!!!!!!!!!!!!!!!!!
	{
		if (App->editor->selectedGameObject != nullptr)
		{
			ComponentTransform* transform = dynamic_cast<ComponentTransform*>(App->editor->selectedGameObject->GetComponent(Component::Type::TRANSFORM));
			LookAt(transform->position);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		ComponentTransform* transform = dynamic_cast<ComponentTransform*>(App->editor->selectedGameObject->GetComponent(Component::Type::TRANSFORM));
		LookAt(transform->position);
	}

	if (App->input->GetMouseZ() < 0)
		newPos += Z * camSpeed * 3;
	if (App->input->GetMouseZ() > 0)
		newPos -= Z * camSpeed * 3;

	// Recalculate matrix -------------
	Position += newPos;
	Reference += newPos;
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

#include "ComponentTransform.h"

#include "External/imgui/imgui.h"
#include "External/imgui/backends/imgui_impl_sdl.h"
#include "External/imgui/backends/imgui_impl_opengl3.h"

#include "ModuleInput.h"

#include "Module.h"

ComponentTransform::ComponentTransform() : Component(nullptr)
{
	transform = IdentityMatrix;
	name = "Transform";
}

ComponentTransform::ComponentTransform(GameObject* _gm) : Component(_gm)
{
	transform = IdentityMatrix;
	scale = (1, 1, 1);
	name = "Transform";
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Update()
{
	if (active)
	{
		if(updateTransform)
		UpdateTransform();
	}

}

void ComponentTransform::Draw()
{
	if (ImGui::CollapsingHeader("Local Transformation"))
	{
		if (ImGui::SliderFloat3("Position", &position, -200, 200)) updateTransform = true;
		if (ImGui::SliderFloat3("Rotation", &rotation, -180, 180)) updateTransform = true;
		if (ImGui::SliderFloat3("Scale", &scale, 0, 100)) updateTransform = true;
	}
}

void ComponentTransform::UpdateTransform()
{

	SetPos(position.x, position.y, position.z);

	SetRotation(rotation.x, vec3(1, 0, 0));
	SetRotation(rotation.y, vec3(0, 1, 0));
	SetRotation(rotation.z, vec3(0, 0, 1));

	Scale(scale.x, scale.y, scale.z);

	updateTransform = false;
}

void ComponentTransform::SetPos(float x, float y, float z)
{
	transform.translate(x, y, z);
}

// ------------------------------------------------------------
void ComponentTransform::SetRotation(float angle, const vec3& u)
{
	transform.rotate(angle, u);
}

// ------------------------------------------------------------
void ComponentTransform::Scale(float x, float y, float z)
{
	transform.scale(x, y, z);
}

mat4x4 ComponentTransform::GetTransform()
{
	return transform;
}
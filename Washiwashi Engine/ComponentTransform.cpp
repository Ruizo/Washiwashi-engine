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
	if (active)	UpdateTransform();
}

void ComponentTransform::UpdateInspector()
{
	if (ImGui::CollapsingHeader("Local Transformation"))
	{
		if (ImGui::DragFloat3("Position", &position, -1, 1));
		if (ImGui::DragFloat3("Rotation", &rotation, -1, 1));
		if (ImGui::DragFloat3("Scale", &scale, -1, 1));
		if (ImGui::Button(("Reset Transform"), ImVec2(140, 30))) reset = !reset;
	}
}

void ComponentTransform::UpdateTransform()
{
	if (reset)
	{
		position = (0, 0, 0);
		rotation = (0, 0, 0);
		scale = (1, 1, 1);
		reset = false;
	}
	SetPos(position.x, position.y, position.z);

	SetRotation(rotation.x, vec3(0, 0, 1));
	SetRotation(rotation.y, vec3(0, 0, 1));
	SetRotation(rotation.z, vec3(0, 0, 1));

	Scale(scale.x, scale.y, scale.z);
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
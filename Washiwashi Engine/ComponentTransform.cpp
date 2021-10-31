#include "ComponentTransform.h"
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
		if (ImGui::DragFloat3("Position", &position, 1));
		if (ImGui::DragFloat3("Rotation", &rotation, 1));
		if (ImGui::DragFloat3("Scale", &scale, 1));
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

	transform.translate(position.x, position.y, position.z);

	transform.rotate(rotation.x, vec3(1, 0, 0));
	transform.rotate(rotation.y, vec3(0, 1, 0));
	transform.rotate(rotation.z, vec3(0, 0, 1));

	transform.scale(scale.x, scale.y, scale.z);
}

mat4x4 ComponentTransform::GetTransform()
{
	return transform;
}
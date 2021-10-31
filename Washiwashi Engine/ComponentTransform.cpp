#include "ComponentTransform.h"
#include "ModuleInput.h"
#include "Module.h"

ComponentTransform::ComponentTransform() : Component(nullptr)
{
	transform.SetIdentity();
	transform.Decompose(position, rotQuaternion, scale);
	rotQuaternion.Normalize();
	rotation = rotQuaternion.ToEulerXYZ();
	
	name = "Transform";
}

ComponentTransform::ComponentTransform(GameObject* _gm) : Component(_gm)
{
	transform.SetIdentity();
	transform.Decompose(position, rotQuaternion, scale);
	rotQuaternion.Normalize();
	rotation = rotQuaternion.ToEulerXYZ();

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
		if (ImGui::DragFloat3("Position", &position[0], 1));
		if (ImGui::DragFloat3("Rotation", &rotation[0], 1));
		if (ImGui::DragFloat3("Scale", &scale[0], 1));
		if (ImGui::Button(("Reset Transform"), ImVec2(140, 30))) reset = !reset;
	}
}

void ComponentTransform::UpdateTransform()
{
	if (reset)
	{
		position = rotation = float3::zero;
		scale = float3::one;
		reset = false;
	}

	rotQuaternion = Quat::FromEulerXYZ(rotation.x * DEGTORAD, rotation.y * DEGTORAD, rotation.z * DEGTORAD);
	rotQuaternion.Normalize();

	transform = float4x4::FromTRS(position, rotQuaternion, scale);
	transform = transform.Transposed();
}

const float* ComponentTransform::GetTransform()
{
	return transform.ptr();
}
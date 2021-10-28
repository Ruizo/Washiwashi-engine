#include "ComponentTransform.h"

#include "External/imgui/imgui.h"
#include "External/imgui/backends/imgui_impl_sdl.h"
#include "External/imgui/backends/imgui_impl_opengl3.h"

TransformComponent::TransformComponent() : Component(nullptr)
{
	transform = IdentityMatrix;
	name = "Transform";
}

TransformComponent::TransformComponent(GameObject* _gm) : Component(_gm)
{
	transform = IdentityMatrix;
	scale = (1, 1, 1);
	name = "Transform";
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::Update()
{
	if (active)
	{
		UpdateTransform();
	}

}

void TransformComponent::Draw()
{
	if (ImGui::CollapsingHeader("Local Transformation"))
	{
		if (ImGui::InputFloat3("Position", &position, 0)) updateTransform = true;
		if (ImGui::SliderFloat3("Rotation", &rotation, -180, 180)) updateTransform = true;
		if (ImGui::InputFloat3("Scale", &scale, 0)) updateTransform = true;
		ImGui::Text("Bounding Box: -not generated-");
		ImGui::Text("Velocity: 0.00 0.00 0.00 (0.00 m/s)");
	}
}

void TransformComponent::UpdateTransform()
{

	SetPos(position.x, position.y, position.z);

	SetRotation(rotation.x, vec3(1, 0, 0));
	SetRotation(rotation.y, vec3(0, 1, 0));
	SetRotation(rotation.z, vec3(0, 0, 1));

	Scale(scale.x, scale.y, scale.z);

	updateTransform = false;
}

void TransformComponent::SetPos(float x, float y, float z)
{
	transform.translate(x, y, z);
}

// ------------------------------------------------------------
void TransformComponent::SetRotation(float angle, const vec3& u)
{
	transform.rotate(angle, u);
}

// ------------------------------------------------------------
void TransformComponent::Scale(float x, float y, float z)
{
	transform.scale(x, y, z);
}

mat4x4 TransformComponent::GetTransform()
{
	return transform;
}
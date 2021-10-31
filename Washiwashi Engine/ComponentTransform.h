#pragma once
#include "GameObject.h"
#include "glmath.h"
#include "MathGeoLib.h"
#include "Application.h"
#include <vector>

class ComponentTransform : public Component
{
public:
	ComponentTransform();
	ComponentTransform(GameObject* _gm);
	virtual ~ComponentTransform();

	void Update() override;

	void UpdateInspector() override;

	void UpdateTransform();

	static inline Type GetType() { return Type::TRANSFORM; };

	const float* GetTransform();

public:
	bool reset;

	float4x4 transform;

	float3 position, scale, rotation;

	Quat rotQuaternion;
};

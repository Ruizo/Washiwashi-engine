#pragma once
#include "GameObject.h"
#include "glmath.h"
#include "glmath.h"
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

	mat4x4 GetTransform();

public:
	bool reset;

	mat4x4 transform;

	vec3 position, scale, rotation;
};

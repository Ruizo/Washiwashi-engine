#pragma once
#include "GameObject.h"
#include "glmath.h"
#include "glmath.h"
#include "Application.h"
#include <vector>

class TransformComponent : public Component
{
public:
	TransformComponent();
	TransformComponent(GameObject* _gm);
	virtual ~TransformComponent();

	void Update() override;

	void Draw() override;

	void UpdateTransform();

	void setRotationX();
	void setRotationY();
	void setRotationZ();

	static inline Type GetType() { return Type::TRANSFORM; };

	void SetPos(float x, float y, float z);
	void SetRotation(float angle, const vec3& u);
	void Scale(float x, float y, float z);

	mat4x4 GetTransform();

public:
	bool updateTransform;

	bool rotationx;
	bool rotationy;
	bool rotationz;

	mat4x4 transform;

	vec3 position, scale, rotation;
};

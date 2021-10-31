#pragma once
#include "GameObject.h"
#include "glew.h"
#include "il.h"
#include "ilut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

class Component;

class ComponentTexture : public Component
{
public:
	ComponentTexture() {}
	ComponentTexture(GameObject* _go);
	~ComponentTexture() {}

	void UpdateInspector() override;


	void LoadTexture(const char* path);

public:
};
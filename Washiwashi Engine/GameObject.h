#pragma once
#include "Globals.h"
#include <vector>
#include <string>

#include "External/imgui/imgui.h"
#include "External/imgui/backends/imgui_impl_sdl.h"
#include "External/imgui/backends/imgui_impl_opengl3.h"

using namespace std;

class GameObject;
class ComponentMesh;
class ComponentTransform;
class ComponentTexture;

class Component
{
public:
	enum class Type
	{
		NONE,
		TRANSFORM,
		MESH,
		TEXTURE,
	};

	Type componentType;

public:
	Component() {}
	Component(GameObject* _go) : active(true), owner(_go), componentType(Type::NONE) {}
	virtual~Component() {}

	void LocateComponentData(const char*, Type _type);

	virtual void Update() {}

	virtual void UpdateInspector() {}

	virtual void Enable() { active = true; }
	virtual void Disable() { active = false; }
	virtual bool IsEnabled() { return active; }
	GameObject* GetOwner() { return owner; }

protected:
	// ----- Components Variables -----
	GameObject* owner;
	bool active;
	string name;
};

class GameObject 
{
public:
	GameObject(const char*, GameObject* _parent);
	~GameObject();

	void Update();

	Component* CreateComponent(Component::Type _componentType);
	Component* GetComponent(Component::Type _componentType);
	
	void LocateComponent(const char*, Component::Type _componentType);

	void Enable() { active = true; }
	void Disable() { active = false; }
	bool IsEnabled() { return active; }

public:
	// ----- GameObjects Variables -----
	GameObject* parent;
	string name;
	bool active;

	// ----- GameObjects Children's list -----
	vector<GameObject*> children;

	// ----- Components Variables -----
	ComponentMesh* mesh = nullptr;
	ComponentTransform* transform = nullptr;
	ComponentTexture* texture = nullptr;

	// ----- Components list -----
	vector<Component*> components;

};
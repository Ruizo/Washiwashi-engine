#pragma once
#include "Globals.h"
#include <vector>
#include <string>

using namespace std;

class GameObject;
class ComponentMesh;
class ComponentTransform;

class Component
{
public:
	Component() {}
	Component(GameObject* _go) : active(true), owner(_go), componentType(Type::NONE) {}
	virtual~Component() {}

	virtual void LoadData(const char*) {}

	virtual void Update() {}

	virtual void Draw() {}

	virtual void Enable() { active = true; }
	virtual void Disable() { active = false; }
	virtual bool IsEnabled() { return active; }
	GameObject* GetOwner() { return owner; }


	enum class Type
	{
		NONE,
		TRANSFORM,
		MESH,
	};

	Type componentType;

protected:
	// ----- Components Variables -----
	GameObject* owner;
	bool active;
	string name;
};

class GameObject 
{
public:
	GameObject(const char*, GameObject* _parent, int _uid = -1);
	~GameObject();

	void Update();
	Component* CreateComponent(Component::Type _componentType);
	Component* GetComponent(Component::Type _componentType);
	
	void LoadComponents(const char*);

	void Enable() { active = true; }
	void Disable() { active = false; }
	bool IsEnabled() { return active; }

public:
	// ----- GameObjects Variables -----
	GameObject* parent;
	string name;
	bool active;
	int uid;

	// ----- GameObjects Children's list -----
	vector<GameObject*> children;

	// ----- Components Variables -----
	ComponentMesh* mesh = nullptr;
	ComponentTransform* transform = nullptr;

	// ----- Components list -----
	vector<Component*> components;

};
#pragma once
#include "Globals.h"
#include <vector>
#include <string>

using namespace std;

class GameObject;

class Component
{
public:
	Component() {}
	Component(GameObject* _go) :active(true), owner(_go), componentType(Type::NONE) {}
	virtual~Component() {}

	virtual void Update() {};

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
	Component* GetComponent(Component::Type _componentType)
	{
		for (size_t i = 0; i < components.size(); i++)
		{
			if (components.at(i)->componentType == _componentType)
			{
				return components.at(i);
			}
		}

		return nullptr;
	}

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
	Mesh mesh;

	// ----- Components list -----
	vector<Component*> components;

};
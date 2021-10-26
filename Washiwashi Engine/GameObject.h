#pragma once
#include <list>
#include "ComponentManager.h"

enum GameObjectType
{
	EMPTY,
	CUBE,
	CYLINDER,
	SPHERE,
	PYRAMID,
	MESH
};

class GameObject 
{
public:
	GameObject(GameObjectType type);
	virtual ~GameObject();

	void Update();

	void Enable();
	void Disable();
	bool GetEnabled();
	void SetEnabled(bool set);
	void CleanUp();

	void AddComponent(ComponentType type);
	void RemoveComponent(ComponentType type);
	std::list<Component*> GetComponents();

	Component* FindComponentByType(ComponentType type);

private:
	bool enabled = false;
	int	 id = 0;
	GameObjectType objectType;
	std::list<Component*> components;
};
#pragma once
#include <list>
#include "ComponentManager.h"

class GameObject 
{
public:
	GameObject();
	virtual ~GameObject();

	void Update();

	void Enable();
	void Disable();
	bool GetEnabled();
	void SetEnabled(bool set);
	void CleanUp();

	void AddComponent(ComponentType type);
	void RemoveComponent(ComponentType type);
	std::list<ComponentManager*> GetComponents();

	ComponentManager* FindComponentByType(ComponentType type);

private:
	bool enabled = false;
	int	 id = 0;
	std::list<ComponentManager*> components;
};
#include "ComponentManager.h"

ComponentManager::ComponentManager(ComponentType _type, GameObject* _owner)
{
	type = _type;
	owner = _owner;

	enabled = true;

	OnCreate();
}

ComponentManager::~ComponentManager()
{
	Destroy();
}

GameObject* ComponentManager::GetOwner()
{
	return owner;
}

ComponentType ComponentManager::GetType()
{
	return type;
}

void ComponentManager::Enable()
{
	if (!enabled)
	{
		enabled = true;
		OnEnable();
	}
}

void ComponentManager::Disable()
{
	if (enabled)
	{
		enabled = true;
		OnDisable();
	}
}

bool ComponentManager::GetEnabled()
{
	return enabled;
}

void ComponentManager::SetEnabled(bool set)
{
	set ? Enable() : Disable();
}

void ComponentManager::Destroy()
{
	OnDestroy();
}
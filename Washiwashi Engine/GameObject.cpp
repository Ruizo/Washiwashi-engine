#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"

#include <assert.h>

GameObject::GameObject(const char* _name, GameObject* _parent, int _uid) : name(_name), active(true), parent(_parent), uid(_uid)
{
	if (parent != nullptr)
	{
		parent->children.push_back(this);
		WASHI_LOG("GameObject %s created. %s is its parent.", _name, parent->name.c_str());
	}
	transform = dynamic_cast<ComponentTransform*>(CreateComponent(Component::Type::TRANSFORM));
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components.at(i)->IsEnabled())
		{
			components.at(i)->Update();
		}
	}
}

Component* GameObject::CreateComponent(Component::Type _componentType)
{
	assert(_componentType != Component::Type::NONE, "Can't create a NONE component");
	Component* ret = nullptr;

	switch (_componentType)
	{
	case Component::Type::TRANSFORM:
		if (transform == nullptr)
		{
			ret = new ComponentTransform(this);
			WASHI_LOG("Added Transform component in %s", this->name.c_str());
		}
		break;
	case Component::Type::MESH:
		if (mesh == nullptr)
		{
			ret = new ComponentMesh(this);
			mesh = dynamic_cast<ComponentMesh*>(ret);
			WASHI_LOG("Added Mesh component in %s", this->name.c_str());
		}
		break;
	}

	if (ret != nullptr)
	{
		ret->componentType = _componentType;
		components.push_back(ret);
	}

	return ret;
}

Component* GameObject::GetComponent(Component::Type _componentType)
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

void GameObject::LoadComponents(const char* path)
{
	for (size_t i = 0; i < components.size(); i++)
	{
		if (components.at(i)->componentType == Component::Type::MESH)
		{
			components.at(i)->LoadData(path);
		}
	}
}


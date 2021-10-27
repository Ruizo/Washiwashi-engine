#include "GameObject.h"
#include "ComponentMesh.h"

#include <assert.h>

GameObject::GameObject(const char* _name, GameObject* _parent, int _uid) : name(_name), active(true), parent(_parent), uid(_uid)
{
	if (parent != nullptr)
	{
		parent->children.push_back(this);
		WASHI_LOG("GameObject %s created. %s is its parent.", _name, parent->name.c_str());
	}
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	for (auto& i : components)
	{
		i->Update();
	}
	for (auto& i : children)
	{
		i->Update();
	}
}

Component* GameObject::CreateComponent(Component::Type _componentType)
{
	assert(_componentType != Component::Type::NONE, "Can't create a NONE component");
	Component* ret = nullptr;

	switch (_componentType)
	{
	case Component::Type::MESH:
		if (mesh == nullptr)
		{
			ret = new MeshComponent();
			ret->componentType = _componentType;
			components.push_back(ret);
			WASHI_LOG("Added Mesh component in %s", this->name.c_str());
		}
		break;
	}

	return ret;
}


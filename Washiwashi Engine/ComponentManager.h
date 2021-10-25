#ifndef _COMPONENTMANAGER_H_
#define _COMPONENTMANAGER_H_

class GameObject;

enum ComponentType
{
	TRANSFORM,
	MESH,
};

class ComponentManager
{
public:
	ComponentManager(ComponentType type, GameObject* owner);
	virtual ~ComponentManager();

	virtual void Update() {}

	GameObject* GetOwner();
	ComponentType GetType();

	void Enable();
	void Disable();
	bool GetEnabled();
	void SetEnabled(bool set);
	void Destroy();

private:
	virtual void OnCreate() {};
	virtual void OnDestroy() {};
	virtual void OnEnable() {};
	virtual void OnDisable() {};

private:
	bool enabled = false;
	GameObject* owner = nullptr;
	ComponentType type;
};

#endif // !_COMPONENTMANAGER_H_
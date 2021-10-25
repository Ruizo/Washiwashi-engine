#pragma once
#include "Module.h"
#include "GameObject.h"
#include "Primitive.h"

class GameObjectManager : public Module
{
public:
	GameObjectManager(Application* app, bool startEnabled = true);
	~GameObjectManager();

	bool Start();
	UpdateStatus Update(float dt);
	UpdateStatus PostUpdate(float dt);
	bool CleanUp();

	GameObject* Create();
	void Destroy(GameObject* go);

private:
	void DestroyGameObjects();

private:
	std::list<GameObject*> gameObjects;
	std::list<GameObject*> toDelete;
};
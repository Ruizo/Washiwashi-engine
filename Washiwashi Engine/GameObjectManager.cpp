#include "Globals.h"
#include "Application.h"
#include "GameObjectManager.h"

GameObjectManager::GameObjectManager(Application* app, bool startEnabled)  : Module(app, startEnabled)
{}

GameObjectManager::~GameObjectManager()
{}

bool GameObjectManager::Start()
{
	WASHI_LOG("Setting up the game object manager");

	return true;
}

bool GameObjectManager::CleanUp()
{
	WASHI_LOG("Cleaning game objects and manager");

	for (std::list<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); it++)
	{
		Destroy((*it));
	}
	gameObjects.clear();

	DestroyGameObjects();

	return true;
}

UpdateStatus GameObjectManager::Update(float dt)
{
	for (std::list<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end();)
	{
		(*it)->Update();
	}

	return UPDATE_CONTINUE;
}

UpdateStatus GameObjectManager::PostUpdate(float dt)
{
	DestroyGameObjects();

	return UPDATE_CONTINUE;
}

GameObject* GameObjectManager::Create(GameObjectType type)
{
	GameObject* go;
	switch (type)
	{
	case EMPTY:
		break;
	case CUBE:
		go = new GameObject(type);

		break;
	case CYLINDER:
		break;
	case SPHERE:
		break;
	case PYRAMID:
		break;
	case MESH:
		break;
	default:
		break;
	}
	gameObjects.push_back(go);
	
	return go;
}

void GameObjectManager::Destroy(GameObject* go)
{
	toDelete.push_back(go);
}

void GameObjectManager::DestroyGameObjects()
{
	for (std::list<GameObject*>::iterator toDel = toDelete.begin(); toDel != toDelete.end();) 
	{
		for (std::list<GameObject*>::iterator go = gameObjects.begin(); go != gameObjects.end();)
		{
			if ((*toDel) == (*go))
			{
				go = gameObjects.erase(go);
				break;
			}
			else ++go;
		}
		(*toDel)->CleanUp();
		delete (*toDel);

		toDel = toDelete.erase(toDel);
	}
}
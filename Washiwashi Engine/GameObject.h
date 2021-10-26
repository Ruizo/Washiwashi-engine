#pragma once
#include <vector>
#include <string>

using namespace std;

class GameObject 
{
public:
	GameObject(std::string _name) : name(_name) 
	{
		active = true;
		parent = nullptr;
	}
	~GameObject() {}

	void Update();

public:
	string name;
	bool active;
	GameObject* parent;
	vector<GameObject*> gameObjects;
};
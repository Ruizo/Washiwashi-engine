#pragma once
#include "GameObject.h"
#include <string>

using namespace std;

class Mesh : public Component
{
public:
	Mesh() {}
	Mesh(const char* _path) : path(_path) {}
	~Mesh() {}

public:
	string path;

};

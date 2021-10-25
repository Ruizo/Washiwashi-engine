#pragma once

class Application;

enum UpdateStatus
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

class Module
{
private :
	bool enabled;

public:
	Application* App;

	Module(Application* parent, bool startEnabled = true) : App(parent)
	{}

	virtual ~Module()
	{}

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual UpdateStatus PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual UpdateStatus Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual UpdateStatus PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}
};
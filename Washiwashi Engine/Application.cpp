#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	editor = new ModuleEditor(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(editor);
	
	// Scenes
	AddModule(scene_intro);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL)
	{
		delete item->data;
		item = item->prev;
	}
}

bool Application::Init()
{
	bool ret = true;
	
	//Load();

	// Call Init() in all modules
	p2List_item<Module*>* item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		ret = item->data->Init();
		item = item->next;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}
	
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	float k = (1000 / maxFPS) - dt;
	if (k > 0)
	{
		SDL_Delay((Uint32)k);
	}
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	p2List_item<Module*>* item = list_modules.getFirst();
	
	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->PreUpdate(dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->Update(dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->PostUpdate(dt);
		item = item->next;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.add(mod);
}

bool Application::RequestBrowser(const char* path)
{
	ShellExecuteA(NULL, "open", path, NULL, NULL, SW_SHOWNORMAL);
	return true;
}

void Application::Save()
{
	JSON_Value* userData = json_parse_file("Save_File.json");
	JSON_Object* rootObject = json_value_get_object(userData);

	userData = json_value_init_object();

	// ----- Parameters to Save -----
	json_object_set_number(rootObject, "Max FPS", maxFPS);
	json_object_dotset_number(rootObject, "Window.Brightness", brightness);
	json_object_dotset_number(rootObject, "Window.Width", width);
	json_object_dotset_number(rootObject, "Window.Height", height);
	//
	
	json_serialize_to_file(userData, "Save_File.json");

	json_value_free(userData);
}

void Application::Load()
{
	JSON_Value* userData = json_parse_file("Save_File.json");
	if (userData == NULL);
	else
	{
		JSON_Object* rootObject = json_value_get_object(userData);
		
		// ----- Parameters to Load -----
		maxFPS = (int)json_object_get_number(rootObject, "Max FPS");
		brightness = (int)json_object_dotget_number(rootObject, "Window.Brightness");
		width = (int)json_object_dotget_number(rootObject, "Window.Width");
		height = (int)json_object_dotget_number(rootObject, "Window.Height");
		//
	}

	json_value_free(userData);
}

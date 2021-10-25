#include "Application.h"
#include "ModuleFileSystem.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene = new ModuleScene(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	editor = new ModuleEditor(this);
	fileManager = new ModuleFileSystem(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(fileManager);
	
	// Scenes
	AddModule(scene);
	AddModule(editor);

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
	Load();

	// Call Init() in all modules
	p2List_item<Module*>* item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		ret = item->data->Init();
		item = item->next;
	}

	// After all Init calls we call Start() in all modules
	WASHI_LOG("Application Start --------------");
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

	SDL_SetWindowBrightness(window->window, brightness);

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
	userData = json_value_init_object();

	userData = json_value_init_object();

	// ----- Parameters to Save -----
	json_object_set_number(json_object(userData), "Max FPS", maxFPS);
	json_object_dotset_number(json_object(userData), "Window.Brightness", brightness);
	json_object_dotset_number(json_object(userData), "Window.Width", width);
	json_object_dotset_number(json_object(userData), "Window.Height", height);
	json_object_dotset_boolean(json_object(userData), "Window.Fullscreen", fullscreen);
	//
	
	json_serialize_to_file(userData, "Save_File.json");

	json_value_free(userData);
}

void Application::Load()
{
	// ------------------- LOAD FILE -------------------
	JSON_Value* root_value = json_parse_file("Save_File.json");

	if (root_value == nullptr)
	{
		WASHI_LOG("FILE editor_config.json couldn't be loaded\n");
		maxFPS = 60;

		width = 1280;
		height = 720;

		brightness = 1;
	}
	else
	{
		JSON_Object* root_object = json_value_get_object(root_value);
		WASHI_LOG("LOADING...\n");

		maxFPS = (int)json_object_get_number(root_object, "Max FPS");

		width = (float)json_object_dotget_number(json_object(root_value), "Window.Width");
		height = (float)json_object_dotget_number(root_object, "Window.Height");
		brightness = (float)json_object_dotget_number(root_object, "Window.Brightness");
		fullscreen = (bool)json_object_dotget_boolean(root_object, "Window.Fullscreen");

		WASHI_LOG("%f", width);
		char* serialized_string = json_serialize_to_string_pretty(root_value);
		WASHI_LOG("%s\n", serialized_string);
		json_free_serialized_string(serialized_string);

		window->SetWindowSize();
		window->SetWindowBrightness();
		window->SetFullscreen(fullscreen);
	}

	json_value_free(root_value);
}

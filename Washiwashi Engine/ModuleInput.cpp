#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(Application* app, bool startEnabled) : Module(app, startEnabled)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	WASHI_LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		WASHI_LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
UpdateStatus ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;
	SDL_Event e;
	


	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / SCREEN_SIZE;
			mouse_y = e.motion.y / SCREEN_SIZE;

			mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
			mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
			break;

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_DROPFILE:
				SDL_free(dropped_filedir);
				dropped_filedir = e.drop.file;
				App->scene->path = dropped_filedir;
				if (App->editor->selectedGameObject == nullptr || App->editor->selectedGameObject == App->scene->root)
				{
					loadedObject = App->scene->SpawnGameObject("Loaded Mesh", App->scene->root);
					loadedObject->CreateComponent(Component::Type::MESH);
					loadedObject->LocateComponent(dropped_filedir, Component::Type::MESH);
					WASHI_LOG("Loaded Mesh from %s", dropped_filedir);
				}
				else
				{
					bool check = false;
					for (int i = 0; i < App->editor->selectedGameObject->components.size(); i++)
					{
						if ((App->editor->selectedGameObject->components.at(i)->componentType == Component::Type::TEXTURE))
						{
							check = true;
							break;
						}
						else
						{
							check = false;
						}
					}
					if (check)
					{
						App->editor->selectedGameObject->LocateComponent(dropped_filedir, Component::Type::TEXTURE);
					}
					else
					{
						App->editor->selectedGameObject->CreateComponent(Component::Type::TEXTURE);
						App->editor->selectedGameObject->LocateComponent(dropped_filedir, Component::Type::TEXTURE);
					}
					WASHI_LOG("Loaded Texture from %s", dropped_filedir);
				}
				break;
				

			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)	App->renderer3D->OnResize(e.window.data1, e.window.data2);
			}
		}
	}

	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	WASHI_LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}
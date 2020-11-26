#include "Application.h"
#include "Input.h"
#include "Window.h"
#include "ObjectManager.h"
#include "GameObject.h"
#include "Components.h"

#include "imgui_impl_sdl.h"
#include "imgui.h"

#define MAX_KEYS 300

Input::Input(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);

	quit = false;
}

// Destructor
Input::~Input()
{
	delete[] keyboard;
}

// Called before render is available
bool Input::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);
	

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	SDL_StartTextInput();

	return true;
}

// Called every draw update
update_status Input::PreUpdate()
{
	SDL_PumpEvents();
	ImGuiIO io = ImGui::GetIO();
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
	mouse_x_motion = io.MouseDelta.x;
	mouse_y_motion = io.MouseDelta.y;
	
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

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		if (event.type == SDL_QUIT)
			quit = true;
		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(App->window->window))
			quit = true;
		if (event.type == SDL_DROPFILE)
		{
			std::string file = event.drop.file;
			LOG("Loading file: %s", file.c_str());

			std::string type;
			for (uint i = file.size() - 1; i != -1; i--)
			{
				if (file[i] == '.')
				{
					std::string reverse;
					for (uint c = type.size() - 1; c != -1; c--)
						reverse.push_back(type[c]);
					type = reverse;
					
					break;
				}

				type.push_back(file[i]);
			}
			LOG("File type: %s", type.c_str());

			if (type == "fbx" || type == "FBX" || type == "obj" || type == "OBJ")
			{
				if (!App->objects->selected)
				{
					GameObject* object = App->objects->AddObject();
					object->AddMeshComponent(file.c_str());
					App->objects->selected = object;
				}
				else
				{
					App->objects->selected->AddMeshComponent(file.c_str());
				}
			}
			else if (type == "png" || type == "PNG" || type == "jpg" || type == "JPG" || type == "dds" || type == "DDS" || type == "tga" || type == "TGA")
			{
				GameObject* object = App->objects->selected;
				if (!object)
				{
					object = App->objects->AddObject(nullptr, App->objects->selected, true, "Plane");
					object->AddMeshComponent("Assets/Meshes/Primitives/plane.fbx", file.c_str());
				}
				else
				{
					bool found = false;
					for (uint c = 0; c < object->components.size(); c++)
					{
						if (object->components[c]->AddTexture(file.c_str()))
						{
							found = true;
							break;
						}
					}
					if(!found)
						object->AddMeshComponent("Assets/Meshes/Primitives/plane.fbx", file.c_str());
				}
			}
			else
			{
				LOG("Unable to load file, unknown file type");
			}
		}
	}

	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool Input::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}
#include "Globals.h"
#include "Application.h"
#include "Module.h"
#include "ModuleGui.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "SDL_opengl.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h" //ENABLE THE INPUT TEXT FUNCTIONS WITH STD::STRING
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "wtypes.h"
#include <string> 
#include "UI_MainMenu.h"

MainMenu::MainMenu(bool isActive) : GUI("Main Menu", isActive)
{
}

MainMenu::~MainMenu()
{
}

void MainMenu::Update()
{
	LOG("YOOOOOO LETS GOO");
}

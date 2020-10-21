#include "UI_GameObjects.h"
#include "Globals.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "SDL_opengl.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h" 

GameObjects::GameObjects(bool isActive, Application* application) : UI("GameObjects", isActive, App)
{
	App = application;
}

GameObjects::~GameObjects()
{
}

void GameObjects::Update(float dt)
{
	ImGui::Begin("GameObjects");
	ImGui::End();
}

#include "UI_GameObjects.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

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

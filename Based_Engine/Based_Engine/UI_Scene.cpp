#include "UI_Scene.h"
#include "Globals.h"
#include "Application.h"
#include "Module.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "SDL_opengl.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h" 
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "wtypes.h"
#include <string> 

Scene::Scene(bool isActive, Application* application) : GUI("Console", isActive, App)
{
	App = application;
}

Scene::~Scene()
{
}

void Scene::Update(float dt)
{
	ImGui::Begin("Scene");
	{
		ImGui::BeginChild("SceneImage");

		ImVec2 winSize = ImGui::GetWindowSize();
		if (winSize.x != windowSize.x || winSize.y != windowSize.y)
			OnResize(winSize);
		ImVec2 uvMin = ImVec2(0.0f, 1.0f);                 
		ImVec2 uvMax = ImVec2(1.0f, 0.0f);                 
		ImGui::Image((ImTextureID)App->renderer3D->texColorBuffer, winSize, uvMin, uvMax);
		ImGui::EndChild();
	}
	ImGui::End();

}

void Scene::Cleanup()
{
}

void Scene::OnResize(ImVec2 newWindowSize)
{
	windowSize.x = newWindowSize.x;
	windowSize.y = newWindowSize.y;

	App->renderer3D->OnResize(newWindowSize.x, newWindowSize.y);
}

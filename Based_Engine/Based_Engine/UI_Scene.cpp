#include "Application.h"
#include "UI_Scene.h"
#include "Window.h"
#include "Renderer3D.h"
#include "Camera3D.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "wtypes.h"

Scene::Scene(bool isActive, Application* application) : UI("Console", isActive, App)
{
	App = application;
}

Scene::~Scene()
{
}

void Scene::Update(float dt)
{

	if (ImGui::Begin("Scene"))
	{
		char* pauseButton;
		if (App->paused)
			pauseButton = "Play";
		else
			pauseButton = "Pause";
		if (ImGui::Button(pauseButton))
			App->paused = !App->paused;

		ImGui::BeginChild("SceneImage");

		App->camera->CameraMovement();

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

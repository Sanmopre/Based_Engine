#include "UI_GameObject.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "wtypes.h"
GameObject_Inspector::GameObject_Inspector(bool isActive, Application* application) : UI("GameObjects", isActive, App)
{
	App = application;
}

GameObject_Inspector::~GameObject_Inspector()
{
}

void GameObject_Inspector::Update(float dt)
{
	if (ImGui::Begin("Object Inspector")) {

		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::InputInt3("Position",position);
			ImGui::InputInt3("Rotation", &rotation_x);
			ImGui::InputInt3("Scale", &scale_x);
		}

	}
	ImGui::End();
}
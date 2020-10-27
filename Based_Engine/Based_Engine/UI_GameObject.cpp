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
			ImGui::InputFloat3("Position", position);
			ImGui::InputFloat3("Rotation", rotation);
			ImGui::InputFloat3("Scale", scale);
		}

		if (ImGui::CollapsingHeader("Geometry"))
		{

		}

		if (ImGui::CollapsingHeader("Material"))
		{

		}
	}
	ImGui::End();
}

void GameObject_Inspector::AddComponentUI(Component_ui* component)
{
}

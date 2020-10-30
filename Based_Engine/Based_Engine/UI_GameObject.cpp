#include "UI_GameObject.h"
#include "Application.h"
#include "ObjectManager.h"
#include "GameObject.h"
#include "Components.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

GameObject_Inspector::GameObject_Inspector(bool isActive, Application* application) : UI("GameObjects", isActive, App)
{
	App = application;
}

GameObject_Inspector::~GameObject_Inspector()
{
}

void GameObject_Inspector::Update(float dt)
{
	object = App->objects->selected;

	if (ImGui::Begin("Object Inspector")) 
		if (object)
		{
			if (ImGui::CollapsingHeader("GameObject", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Text(object->GetName());
			}
			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Text("position");
				ImGui::InputFloat("x", &object->transform.x, 0.10f, 1.0f);
				ImGui::InputFloat("y", &object->transform.y, 0.10f, 1.0f);
				ImGui::InputFloat("z", &object->transform.z, 0.10f, 1.0f);
			}
			for (uint i = 0; i < object->components.size(); i++)
			{
				object->components[i]->DisplayComponentMenu();
			}
		}

	ImGui::End();
}

void GameObject_Inspector::AddComponentUI(Component_ui* component)
{
}

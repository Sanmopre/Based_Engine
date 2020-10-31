#include "UI_GameObjects.h"
#include "ObjectManager.h"
#include "GameObject.h"
#include "Application.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "Input.h"

GameObjects::GameObjects(bool isActive, Application* application) : UI("GameObjects", isActive, App)
{
	App = application;
}

GameObjects::~GameObjects()
{

}

void GameObjects::Update(float dt)
{
	if (ImGui::Begin("GameObjects"))
	{
		i = 0;
		for (std::vector<GameObject*>::iterator obj = App->objects->gameobjects.begin(); obj != App->objects->gameobjects.end(); obj++)
			IterateGameObjects(*obj);
		if (ImGui::Button("deselect"))
			App->objects->selected = nullptr;
	}
	ImGui::End();
}

void GameObjects::IterateGameObjects(GameObject* gameobject)
{
	char bName[10];
	sprintf_s(bName, "select%d", i);
	i++;

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
	if (gameobject == App->objects->selected)
		flags |= ImGuiTreeNodeFlags_Selected;
	if (gameobject->children.size() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf;

	if (ImGui::TreeNodeEx(gameobject->GetName(), flags))
	{
		ImGui::SameLine();
		if (ImGui::Button(bName, { 100, 13 }))
			App->objects->selected = gameobject;

		for (std::vector<GameObject*>::iterator obj = gameobject->children.begin(); obj != gameobject->children.end(); obj++)
		{
			GameObject* object = *obj;

			IterateGameObjects(object);
		}
		ImGui::TreePop();
	}
	else
	{
		ImGui::SameLine();
		if (ImGui::Button(bName, { 100, 13 }))
			App->objects->selected = gameobject;
	}
}




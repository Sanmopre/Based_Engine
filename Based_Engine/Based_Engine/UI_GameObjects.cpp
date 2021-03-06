#include "UI_GameObjects.h"
#include "ObjectManager.h"
#include "GameObject.h"
#include "Application.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h" 

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
		if (ImGui::IsItemHovered())
			if (App->input->GetKey(1) == KEY_DOWN)
				App->objects->selected = nullptr;

		if (ImGui::BeginCombo(" ", "Create"))
		{
			char* ch = nullptr;
			if (ImGui::Selectable("Create GameObject"))
			{
				GameObject* go = App->objects->AddObject(ch, App->objects->selected);
			}
			if (ImGui::Selectable("Create Cube"))
			{
				GameObject* go = App->objects->AddObject(ch, App->objects->selected, true, "Cube");
				go->AddMeshComponent("Assets/Meshes/Primitives/cube.fbx");
			}
			if (ImGui::Selectable("Create Sphere"))
			{
				GameObject* go = App->objects->AddObject(ch, App->objects->selected, true, "Sphere");
				go->AddMeshComponent("Assets/Meshes/Primitives/sphere.fbx");
			}
			if (ImGui::Selectable("Create Pyramid"))
			{
				GameObject* go = App->objects->AddObject(ch, App->objects->selected, true, "Pyramid");
				go->AddMeshComponent("Assets/Meshes/Primitives/pyramid.fbx");
			}
			if (ImGui::Selectable("Create Cylinder"))
			{
				GameObject* go = App->objects->AddObject(ch, App->objects->selected, true, "Cylinder");
				go->AddMeshComponent("Assets/Meshes/Primitives/cylinder.fbx");
			}
			if (ImGui::Selectable("Create Plane"))
			{
				GameObject* go = App->objects->AddObject(ch, App->objects->selected, true, "Plane");
				go->AddMeshComponent("Assets/Meshes/Primitives/plane.fbx");
			}

			ImGui::EndCombo();
		}
		ImGui::Separator();

		i = 0;
		for (std::vector<GameObject*>::iterator obj = App->objects->parent->children.begin(); obj != App->objects->parent->children.end(); obj++)
			if (!IterateGameObjects(*obj))
				break;

		if (ImGui::Button("deselect"))
			App->objects->selected = nullptr;
	}
	ImGui::End();
}

bool GameObjects::IterateGameObjects(GameObject* gameobject)
{
	bool output = true;

	char bName[10];
	sprintf_s(bName, "select%d", i);
	i++;

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;
	if (gameobject == App->objects->selected)
		flags |= ImGuiTreeNodeFlags_Selected;
	if (gameobject->children.size() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf;

	if (ImGui::TreeNodeEx(gameobject->name.c_str(), flags))
	{
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			App->objects->selected = gameobject;

		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
			dragDropGo = gameobject;
			ImGui::Text(gameobject->name.c_str());
			ImGui::EndDragDropSource();
		}
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_TREENODE"))
				if (App->objects->ChildGameObject(dragDropGo, gameobject))
				{
					dragDropGo = nullptr;
					output = false;
				}
			ImGui::EndDragDropTarget();
		}

		for (std::vector<GameObject*>::iterator obj = gameobject->children.begin(); obj != gameobject->children.end(); obj++)
		{
			GameObject* object = *obj;

			if (!IterateGameObjects(object))
			{
				ImGui::TreePop();
				return false;
			}
		}
		ImGui::TreePop();
	}
	else
	{
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			App->objects->selected = gameobject;

		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
			dragDropGo = gameobject;
			ImGui::Text(gameobject->name.c_str());
			ImGui::EndDragDropSource();
		}
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_TREENODE"))
				if (App->objects->ChildGameObject(dragDropGo, gameobject))
				{
					dragDropGo = nullptr;
					output = false;
				}
			ImGui::EndDragDropTarget();
		}
	}

	return output;
}




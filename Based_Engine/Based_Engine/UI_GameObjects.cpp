#include "UI_GameObjects.h"
#include "ObjectManager.h"
#include "GameObject.h"
#include "Application.h"

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

	for (std::vector<GameObject*>::iterator obj = App->objects->gameobjects.begin(); obj != App->objects->gameobjects.end(); obj++)
		IterateGameObjects(*obj);

	ImGui::End();
}

void GameObjects::IterateGameObjects(GameObject* gameobject)
{
	if (ImGui::TreeNode(gameobject->GetName()))
	{
		for (std::vector<GameObject*>::iterator obj = gameobject->children.begin(); obj != gameobject->children.end(); obj++)
		{
			GameObject* object = *obj;

			IterateGameObjects(object);
		}
		ImGui::TreePop();
	}
}



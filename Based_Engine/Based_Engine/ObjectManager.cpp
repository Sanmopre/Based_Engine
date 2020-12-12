#include "ObjectManager.h"
#include "Application.h"
#include "GameObject.h"
#include "Transform.h"
#include "Input.h"

ObjectManager::ObjectManager(Application* app, bool active) : Module(app, active)
{
	parent = new GameObject("BASEDObject", nullptr, App, true);

	go_id = 0;
}

ObjectManager::~ObjectManager()
{
	delete parent;
}

bool ObjectManager::Start()
{
	GameObject* street = AddObject("street");
	street->AddMeshComponent("Library/LMeshes/Street environment_V01.monki");

	return true;
}

update_status ObjectManager::Update(float dt)
{
	parent->Update(dt);

	return UPDATE_CONTINUE;
}

update_status ObjectManager::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ObjectManager::CleanUp()
{
	return true;
}

GameObject* ObjectManager::AddObject(char* name, GameObject* parent, bool active, const char* type)
{
	if (name == nullptr)
	{
		char str[64];
		sprintf_s(str, "%s%d", type, go_id);
		go_id++;
		name = str;
	}

	if (!parent)
		parent = this->parent;
	GameObject* output = new GameObject(name, parent, App, active);

	parent->children.push_back(output);
	
	return output;
}

bool ObjectManager::ChildGameObject(GameObject* newChild, GameObject* newParent)
{
	if (newChild->parent == newParent)
		return false;
	else if (newParent->parent == newChild)
		ChildGameObject(newParent, newChild->parent);

	for (std::vector<GameObject*>::iterator g = newChild->parent->children.begin(); g != newChild->parent->children.end(); g++)
		if (*g == newChild)
		{
			newChild->parent->children.erase(g);
			break;
		}
	newChild->parent = newParent;
	newParent->children.push_back(newChild);

	return true;
}

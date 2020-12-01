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
	street->AddMeshComponent("Library/Meshes/cube.monki", "Assets/Textures/Street.png");
	street->transform->scale.x = 25;
	street->transform->scale.z = 25;
	street->transform->position.z = -12.5;
	street->DontTransformChilds();
	
	GameObject* house = AddObject("house", street);
	house->AddMeshComponent("Library/Meshes/Baker_House.monki", "Assets/Textures/Baker_House.png");
	house->transform->position.x = -0.5;
	house->transform->position.y = 0.5;
	
	house = nullptr;

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

void ObjectManager::ChildGameObject(GameObject* child, GameObject* parent)
{
	if (child->parent == parent)
		return;
	for (uint i = 0; i < parent->children.size(); i++)
		if (parent->children[i] == child)
			return;

	for(std::vector<GameObject*>::iterator g = child->parent->children.begin(); g != child->parent->children.begin(); g++)
		if (*g == child)
		{
			child->parent->children.erase(g);
			break;
		}
	child->parent = parent;
	parent->children.push_back(child);
}

#include "ObjectManager.h"
#include "Application.h"
#include "GameObject.h"
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
	GameObject* house = AddObject("house");
	house->AddMeshComponent("Assets/Meshes/Baker_House.fbx", "Assets/Textures/Baker_House.png");

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

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
	GameObject * park = AddObject("park");
	AddObject("dog", park);
	GameObject* room = AddObject("bedroom", house);
	AddObject("bed", room);
	GameObject* gun = AddObject("gun", room);
	AddObject("window", room);
	room = AddObject("kitchen", house);
	GameObject* fridge = AddObject("fridge", room);
	AddObject("yolk", fridge);
	AddObject("chicken", fridge);
	AddObject("carrot", fridge);
	AddObject("microwave", room);
	for (int i = 0; i < 5; i++)
	{
		bool out = false;
		char n[10];
		sprintf_s(n, "tree%d", i);
		AddObject(n, park);
	}
	house->AddMeshComponent("Assets/Meshes/Baker_House.fbx");
	gun->AddMeshComponent("Assets/Meshes/Handgun.fbx", nullptr, false);

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

#include "ObjectManager.h"
#include "Application.h"
#include "GameObject.h"
#include "Input.h"

ObjectManager::ObjectManager(Application* app, bool active) : Module(app, active)
{
	go_id = 0;
}

ObjectManager::~ObjectManager()
{
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
	for (std::vector<GameObject*>::iterator obj = gameobjects.begin(); obj != gameobjects.end(); obj++)
	{
		GameObject* object = *obj;

		object->Update(dt);
	}

	return UPDATE_CONTINUE;
}

update_status ObjectManager::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ObjectManager::CleanUp()
{
	while (gameobjects.size() != 0)
	{
		(*gameobjects.begin())->CleanUp();
		delete* gameobjects.begin();
		gameobjects.erase(gameobjects.begin());
	}

	return true;
}

GameObject* ObjectManager::AddObject(char* name, GameObject* parent, bool active)
{
	if (name == nullptr)
	{
		char str[64];
		sprintf_s(str, "GameObject%d", go_id);
		go_id++;
		name = str;
	}
	GameObject* output = new GameObject(name, parent, App, active);

	if (!parent)
	{
		gameobjects.push_back(output);
	}
	else
	{
		parent->children.push_back(output);
	}



	return output;
}

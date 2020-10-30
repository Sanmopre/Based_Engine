#include "ObjectManager.h"
#include "GameObject.h"

ObjectManager::ObjectManager(Application* app, bool active) : Module(app, active)
{
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
		std::string name = n;
		AddObject(name, park);
	}

	house->AddMeshComponent("Assets/House/BakerHouse.FBX");
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

GameObject* ObjectManager::AddObject(std::string name, GameObject* parent, bool active)
{
	GameObject* output = new GameObject(name, parent, App, active);
	if (!parent)
		gameobjects.push_back(output);
	else
		parent->children.push_back(output);

	return output;
}

#pragma once
#include "Module.h"

class GameObject;

class ObjectManager : public Module
{
public:

	ObjectManager(Application* app, bool active = true);
	virtual ~ObjectManager();

	bool Start();
	update_status Update(float dt);
	update_status PostUpdate();
	bool CleanUp();

	GameObject* AddObject(std::string name, GameObject* parent = nullptr, bool active = true);

	std::vector<GameObject*> gameobjects;

private:

	std::vector<GameObject*> buffer;
	std::vector <std::vector<GameObject*>::iterator> to_delete;
};
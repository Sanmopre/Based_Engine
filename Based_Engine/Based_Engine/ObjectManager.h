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

	GameObject* AddObject(char* name = nullptr, GameObject* parent = nullptr, bool active = true, const char* type = "GameObject");

	GameObject* parent;
	GameObject* selected = nullptr;

private:

	std::vector<GameObject*> buffer;
	std::vector <std::vector<GameObject*>::iterator> to_delete;

	int go_id;
};
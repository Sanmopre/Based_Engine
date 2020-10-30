#pragma once
#include "UI.h"

class GameObject;

struct Component_ui {
	char* name;
};

class GameObject_Inspector : public UI
{
public:
	GameObject_Inspector(bool isActive, Application* application);
	virtual ~GameObject_Inspector();

	void Update(float dt);
	void AddComponentUI(Component_ui* component);

private:

	GameObject* object = nullptr;
	GameObject* last_object = nullptr;
	std::string path;
};
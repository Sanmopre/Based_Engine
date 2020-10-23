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

public:

	GameObject* object;

	std::vector<Component_ui*> components;

	float position[3] = { 0, 0, 0 };
	float rotation[3] = { 0, 0 ,0 };
	float scale[3] = { 0, 0, 0 };
};
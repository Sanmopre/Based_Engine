#pragma once
#include "UI.h"

class GameObject;

class GameObject_Inspector : public UI
{
public:
	GameObject_Inspector(bool isActive, Application* application);
	virtual ~GameObject_Inspector();

	void Update(float dt);

public:

	GameObject* object;

	float position[3] = { 0, 0, 0 };
	float rotation[3] = { 0, 0 ,0 };
	float scale[3] = { 0, 0, 0 };
};
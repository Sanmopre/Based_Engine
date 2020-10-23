#pragma once
#include "UI.h"

class GameObject_Inspector : public UI
{
public:
	GameObject_Inspector(bool isActive, Application* application);
	virtual ~GameObject_Inspector();

	void Update(float dt);

	int* position;
	int rotation_x = 1;
	int scale_x = 0;

	char* name = nullptr;
};
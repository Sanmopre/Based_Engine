#pragma once

#include "Globals.h"

class Application;

struct Transform
{
	Transform()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	float x, y, z;
};

class Component;

class GameObject
{
public:

	GameObject(std::string name, GameObject* parent, Application* app, bool active = true);
	virtual ~GameObject();

	bool Update(float dt);
	bool CleanUp();

	Transform transform;
	std::vector<Component*> components;

	GameObject* parent;
	std::vector<GameObject*> children;

	void AddMeshComponent(const char* path, bool active = true);
	const char* GetName() const { return name.c_str(); }

	bool active;

private:

	std::string name;
	Application* App;
};

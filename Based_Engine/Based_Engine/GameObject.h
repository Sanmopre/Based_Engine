#pragma once
#include "Globals.h"

struct Transform
{
	Transform()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	int x, y, z;
};

class Component
{
};

class GameObject
{
public:

	GameObject(std::string name, GameObject* parent);
	virtual ~GameObject();

	bool Update(float dt);
	bool CleanUp();

	Transform transform;
	std::vector<Component*> components;

	GameObject* parent;
	std::vector<GameObject*> children;

	void AddComponent() {}
	const char* GetName() const { return name.c_str(); }

private:

	std::string name;
};

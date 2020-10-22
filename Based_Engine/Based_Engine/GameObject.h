#pragma once
#include <vector>
#include <string>

struct Transform
{
};

class Component
{
};

class GameObject
{
public:

	GameObject();
	virtual ~GameObject();

	bool Update(float dt);
	bool CleanUp();

	Transform transform;
	std::vector<Component*> components;

	std::string name;
	unsigned int id;
};

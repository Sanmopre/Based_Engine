#pragma once

#include "Globals.h"
#include "Math/float3.h"

class Application;

struct Transform
{
	Transform()
	{
		position = { 0,0,0 };
		rotation = { 0,0,0 };
		scale = { 1,1,1 };
	}

	float3 position, rotation, scale;

	bool operator==(Transform t)
	{
		if(position.x == t.position.x && position.y == t.position.y && position.z == t.position.z &&
			rotation.x == t.rotation.x && rotation.y == t.rotation.y && rotation.z == t.rotation.z &&
			scale.x == t.scale.x && scale.y == t.scale.y && scale.z == t.scale.z)
			return true;
		return false;
	}
	bool operator!=(Transform t)
	{
		if (position.x == t.position.x && position.y == t.position.y && position.z == t.position.z &&
			rotation.x == t.rotation.x && rotation.y == t.rotation.y && rotation.z == t.rotation.z &&
			scale.x == t.scale.x && scale.y == t.scale.y && scale.z == t.scale.z)
			return false;
		return true;
	}
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

	GameObject* parent = nullptr;
	std::vector<GameObject*> children;

	void AddMeshComponent(const char* path, char* name = nullptr, bool active = true);
	const char* GetName() const { return name.c_str(); }

	bool active;
	bool to_delete;

private:

	int comp_id;

	std::string name;
	Application* App = nullptr;
};

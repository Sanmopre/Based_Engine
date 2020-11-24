#pragma once

#include "Globals.h"
#include "Math/float3.h"

class Application;
class Mesh;

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
	Transform last_transform;

	std::vector<Component*> components;

	GameObject* parent = nullptr;
	std::vector<GameObject*> children;

	void AddMeshComponent(const char* path, const char* texture_path = nullptr, char* name = nullptr, bool active = true);
	void AddMeshComponent(Mesh mesh, const char* texture_path = nullptr, char* name = nullptr, bool active = true);

	void DontTransformChilds() { transform_child = false; }

	bool active;
	bool to_delete;

	std::string name;

private:

	void CarryTransformChange(GameObject*);

	int comp_id;
	bool transform_child;

	Application* App = nullptr;
};

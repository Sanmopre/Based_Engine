#pragma once

#include "Globals.h"
#include "Math/float3.h"

class Application;
class Mesh;
class Transform;
class MeshComponent;
class RigidBodyComponent;
class Component;

enum class colider_type;

class GameObject
{
public:

	GameObject(std::string name, GameObject* parent, Application* app, bool active = true);
	virtual ~GameObject();

	bool Update(float dt);
	bool CleanUp();

	Transform* transform = nullptr;
	MeshComponent* meshComp = nullptr;
	RigidBodyComponent* rigidbody = nullptr;

	std::vector<Component*> components;

	GameObject* parent = nullptr;
	std::vector<GameObject*> children;

	void AddMeshComponent(const char* path, const char* texture_path = nullptr, char* name = nullptr, bool active = true);
	void AddMeshComponent(Mesh mesh, const char* path, const char* texture_path = nullptr, char* name = nullptr, bool active = true);

	void AddCameraComponent(char* name = nullptr);

	void AddRigidBodyComponent();

	void AddColliderComponent(colider_type type, char* name = nullptr);

	void DontTransformChilds() { transform_child = false; }

	bool active;
	bool to_delete;

	std::string name;

private:

	int comp_id;
	bool transform_child;

	Application* App = nullptr;
};

#include "GameObject.h"
#include "Application.h"
#include "Transform.h"
#include "Component.h"
#include "MeshComponent.h"
#include "RigidBodyComponent.h"
#include "CameraComponent.h"
#include "ColliderComponent.h"
#include "ConstraintComponent.h"
#include "PlayerController.h"

GameObject::GameObject(std::string name, GameObject* parent, Application* app, bool active)
{
	this->name = name;
	this->parent = parent;
	transform = new Transform(this);
	App = app;
	this->active = active;

	to_delete = false;

	comp_id = 0;
}

GameObject::~GameObject()
{
	delete(transform);
	CleanUp(); 
}

bool GameObject::Update(float dt)
{
	int last = 0;
	bool end = false;

	while (!end)
	{
		if (transform->ToUdate())
		{
			this->transform->RecalculateTransform();
			if (rigidbody)
				rigidbody->UpdateRigidBodyByTransform(true);
		}

		std::vector<Component*>::iterator comp = components.begin() + last;
		for (comp; comp != components.end(); comp++)
		{
			Component* component = *comp;

			if (!component->to_delete)
				component->Update(dt);
			else
			{
				delete component;
				components.erase(comp);

				break;
			}
			last++;
		}
		end = true;
	}


	last = 0;
	end = false;
	while (!end)
	{
		std::vector<GameObject*>::iterator ch = children.begin();
		for (ch; ch != children.end(); ch++)
		{
			GameObject* child = *ch;

			if (!child->to_delete)
			{
				child->Update(dt);
			}
			else
			{
				delete child;
				children.erase(ch);

				break;
			}
			last++;
		}
		end = true;
	}

	return true;
}

bool GameObject::CleanUp()
{
	while (components.size() != 0)
	{
		delete* components.begin();
		components.erase(components.begin());
	}
	while (children.size() != 0)
	{
		delete* children.begin();
		children.erase(children.begin());
	}

	return true;
}

void GameObject::AddMeshComponent(const char* path, const char* texture_path, char* name, bool active)
{
	if (!meshComp)
	{
		Component* comp = new MeshComponent(name, path, texture_path, this, App, active);
		meshComp = (MeshComponent*)comp;
		components.push_back(comp);
	}
	else
		LOG("This GameObject aready has a Mesh Component");
}

void GameObject::AddMeshComponent(Mesh mesh, const char* path, const char* texture_path, char* name, bool active)
{
	MeshComponent* meshptr = new MeshComponent(name, mesh, path, texture_path, this, App, active);
	Component* comp = meshptr;
	meshComp = meshptr;
	components.push_back(comp);
}

void GameObject::AddCameraComponent(char* name)
{
	if (!name)
	{
		char str[10];
		sprintf_s(str, "%d", comp_id);
		name = str;
	}
	comp_id++;

	CameraComponent* cameraptr = new CameraComponent(name, this, App, true);
	Component* comp = cameraptr;
	components.push_back(comp);
}

void GameObject::AddRigidBodyComponent()
{
	if (!rigidbody)
	{
		RigidBodyComponent* rigidbodyptr = new RigidBodyComponent("cuck", this, App, active);
		rigidbody = rigidbodyptr;
		Component* comp = rigidbodyptr;
		components.push_back(comp);
	}
	else if(rigidbody->IsStatic())
	{
		rigidbody->MakeDynamic();
	}
	else
		LOG("This GameObject aready has a RigidBody");
}

void GameObject::AddColliderComponent(colider_type type, float3 offset, float3 size, char* name)
{
	if (!name)
	{
		char str[10];
		sprintf_s(str, "%d", comp_id);
		name = str;
	}
	comp_id++;

	ColliderComponent* collidercomp = new ColliderComponent(name, type, offset, size, this, App, true);
	Component* comp = collidercomp;
	components.push_back(comp);
}

void GameObject::AddConstraintComponent(JointType type, char* name)
{
	if (rigidbody)
	{
		if (!name)
		{
			char str[10];
			sprintf_s(str, "%d", comp_id);
			name = str;
		}
		comp_id++;

		ConstraintComponent* constraintptr = new ConstraintComponent(name, type, this, App, active);
		Component* comp = constraintptr;
		constraint = constraintptr;
		components.push_back(comp);
	}
	else
		LOG("A RigidBody is required to create a Constrain Component");
}

void GameObject::AddPlayerController()
{
	if (!controller)
	{
		Component* comp = new PlayerController(nullptr, this, App, active);
		controller = (PlayerController*)comp;
		components.push_back(comp);
	}
	else
		LOG("This GameObject aready has a Player Controller Component");
}




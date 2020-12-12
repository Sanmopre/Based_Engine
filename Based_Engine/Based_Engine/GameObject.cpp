#include "GameObject.h"
#include "Application.h"
#include "Transform.h"
#include "Component.h"
#include "MeshComponent.h"
#include "CameraComponent.h"

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
		if (!name)
		{
			char str[10];
			sprintf_s(str, "%d", comp_id);
			name = str;
		}
		comp_id++;

		Component* comp = new MeshComponent(name, path, texture_path, this, App, active);
		meshComp = (MeshComponent*)comp;
		components.push_back(comp);
	}
	else
		LOG("This GameObject aready has a Mesh Component");
}

void GameObject::AddMeshComponent(Mesh mesh, const char* path, const char* texture_path, char* name, bool active)
{
	if (!name)
	{
		char str[10];
		sprintf_s(str, "%d", comp_id); 
		name = str;
	}
	comp_id++;

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


#include "GameObject.h"
#include "Application.h"
#include "Components.h"
#include "Transform.h"

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
		(*children.begin())->CleanUp();
		delete* children.begin();
		children.erase(children.begin());
	}

	return true;
}

void GameObject::AddMeshComponent(const char* path, const char* texture_path, char* name, bool active)
{
	if (!name)
	{
		char str[10];
		sprintf_s(str, "%d", comp_id);
		name = str;
	}
	comp_id++;

	MeshComponent* mesh = new MeshComponent(name, path, texture_path, this, App, active);

	Component* comp = mesh;
	components.push_back(comp);
}

void GameObject::AddMeshComponent(Mesh mesh, const char* texture_path, char* name, bool active)
{
	if (!name)
	{
		char str[10];
		sprintf_s(str, "%d", comp_id);
		name = str;
	}
	comp_id++;

	MeshComponent* meshptr = new MeshComponent(name, mesh, texture_path, this, App, active);

	Component* comp = meshptr;
	components.push_back(comp);
}


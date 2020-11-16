#include "GameObject.h"
#include "Application.h"

#include "Components.h"

GameObject::GameObject(std::string name, GameObject* parent, Application* app, bool active)
{
	this->name = name;
	this->parent = parent;

	App = app;
	this->active = active;

	to_delete = false;

	comp_id = 0;
}

GameObject::~GameObject()
{
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

	bool trans = false;
	if (transform_child)
		if (last_transform != transform)
			trans = true;
	transform_child = true;

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
				if (trans)
					CarryTransformChange(child);

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

	last_transform = transform;

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

void GameObject::CarryTransformChange(GameObject* child)
{
	child->transform.position.x -= last_transform.position.x;
	child->transform.position.y -= last_transform.position.y;
	child->transform.position.z -= last_transform.position.z;

	child->transform.position.x += transform.position.x;
	child->transform.position.y += transform.position.y;
	child->transform.position.z += transform.position.z;

	child->transform.rotation.x -= last_transform.rotation.x;
	child->transform.rotation.y -= last_transform.rotation.y;
	child->transform.rotation.z -= last_transform.rotation.z;

	child->transform.rotation.x += transform.rotation.x;
	child->transform.rotation.y += transform.rotation.y;
	child->transform.rotation.z += transform.rotation.z;

	child->transform.scale.x -= last_transform.scale.x;
	child->transform.scale.y -= last_transform.scale.y;
	child->transform.scale.z -= last_transform.scale.z;

	child->transform.scale.x += transform.scale.x;
	child->transform.scale.y += transform.scale.y;
	child->transform.scale.z += transform.scale.z;
}

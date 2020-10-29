#include "GameObject.h"
#include "Application.h"

#include "Components.h"

GameObject::GameObject(std::string name, GameObject* parent, Application* app, bool active)
{
	this->name = name;
	this->parent = parent;

	App = app;
	this->active = active;
}

GameObject::~GameObject()
{

}

bool GameObject::Update(float dt)
{
	for (std::vector<Component*>::iterator comp = components.begin(); comp != components.end(); comp++)
	{
		
	}

	for (std::vector<GameObject*>::iterator ch = children.begin(); ch != children.end(); ch++)
	{
		GameObject* child = *ch;

		child->Update(dt);
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

void GameObject::AddMeshComponent(const char* path, bool active)
{
	MeshComponent* mesh = new MeshComponent(path, App, active);

	Component* comp = mesh;
	components.push_back(comp);
}

#include "GameObject.h"

GameObject::GameObject(std::string name, GameObject* parent)
{
	this->name = name;
	this->parent = parent;
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

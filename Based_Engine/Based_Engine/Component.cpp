#include "Component.h"
#include "Application.h"

Component::Component(char* name, GameObject* parent, Application* app, bool active)
{
	this->name = name;
	name_buffer = name;
	this->parent = parent;
	App = app;
	this->active = active;
	to_activate = active;
	to_delete = false;
}

Component::~Component()
{
}

bool Component::Update(float dt)
{
	return true;
}

void Component::Activate()
{
	active = true;
}

void Component::Deactivate()
{
	active = false;
}

bool Component::IsActive()
{
	return active;
}
#include "Components.h"
#include "Application.h"

Component::Component(GameObject* parent, Application* app, bool active)
{
	this->parent = parent;
	App = app;
	this->active = active;
	toActivate = active;
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
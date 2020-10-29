#include "Components.h"
#include "Application.h"

Component::Component(Application* app, bool active)
{
	App = app;
	this->active = active;
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
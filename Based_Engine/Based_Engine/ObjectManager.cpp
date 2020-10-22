#include "ObjectManager.h"

ObjectManager::ObjectManager(Application* app, bool active) : Module(app, active)
{
}

ObjectManager::~ObjectManager()
{
}

bool ObjectManager::Start()
{
	return true;
}

update_status ObjectManager::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ObjectManager::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ObjectManager::CleanUp()
{
	return true;
}

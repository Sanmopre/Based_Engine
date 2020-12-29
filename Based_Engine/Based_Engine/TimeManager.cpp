#include "Application.h"
#include "TimeManager.h"

TimeManager::TimeManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

TimeManager::~TimeManager()
{
}

bool TimeManager::Start()
{

	return true;
}

update_status TimeManager::PreUpdate()
{

	return UPDATE_CONTINUE;
}

update_status TimeManager::Update(float dt)
{


	return UPDATE_CONTINUE;
}

update_status TimeManager::PostUpdate()
{

	return UPDATE_CONTINUE;
}



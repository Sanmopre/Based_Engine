#pragma once
#include "Module.h"
#include "PxPhysicsAPI.h"

class PhysicsEngine : public Module
{
public:

	PhysicsEngine(Application* app, bool active = true);
	virtual ~PhysicsEngine();

	bool Start();
	update_status PreUpdate();
	update_status Update(float dt);
	update_status PostUpdate();
	bool CleanUp();

private:

	bool gravity = true;
	float gravityValue = 9.8f;
};
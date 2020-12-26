#include "ObjectManager.h"
#include "Application.h"
#include "PhysicsEngine.h"
#include "Transform.h"
#include "Input.h"

#include "extensions/PxDefaultAllocator.h"
#include "extensions/PxDefaultErrorCallback.h"
#include "pvd/PxPvd.h"
#include "pvd/PxPvdSceneClient.h"
#include "pvd/PxPvdTransport.h"
#include "PxPhysicsAPI.h"
#include "characterkinematic/PxControllerManager.h"
#include "foundation/PxAllocatorCallback.h"
#include "PxQueryReport.h"
#include "PxQueryFiltering.h"
#include "extensions/PxRaycastCCD.h"
#include "cooking/PxCooking.h"

PhysicsEngine::PhysicsEngine(Application* app, bool active) : Module(app, active)
{

}

PhysicsEngine::~PhysicsEngine()
{
}

bool PhysicsEngine::Start()
{
	return true;
}

update_status PhysicsEngine::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status PhysicsEngine::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status PhysicsEngine::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool PhysicsEngine::CleanUp()
{
	return true;
}


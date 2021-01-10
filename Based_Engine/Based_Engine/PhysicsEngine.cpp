#include "Application.h"
#include "ObjectManager.h"
#include "PhysicsEngine.h"
#include "Application.h"
#include "GameObject.h"

#include "PxPhysicsAPI.h"

#define BOUNCE_THRESHOLD 0.2f
#define THREADS 4

#ifndef _DEBUG
	#pragma comment(lib, "External/PhysX_3.4/libx86_/Release_/PhysX3_x86.lib")
	#pragma comment(lib, "External/PhysX_3.4/libx86_/Release_/PhysX3Common_x86.lib")
	#pragma comment(lib, "External/PhysX_3.4/libx86_/Release_/PhysX3Extensions.lib")
	#pragma comment(lib, "External/PhysX_3.4/libx86_/Release_/PxFoundation_x86.lib")
	#pragma comment(lib, "External/PhysX_3.4/libx86_/Release_/PxPvdSDK_x86.lib")
	#pragma comment(lib, "External/PhysX_3.4/libx86_/Release_/PhysX3CharacterKinematic_x86.lib")
	#pragma comment(lib, "External/PhysX_3.4/libx86_/Release_/SceneQuery.lib")
	#pragma comment(lib, "External/PhysX_3.4/libx86_/Release_/PhysX3Cooking_x86.lib")
#else
	#pragma comment(lib, "External/PhysX_3.4/libx86_/Debug_/PhysX3CommonDEBUG_x86.lib")
	#pragma comment(lib, "External/PhysX_3.4/libx86_/Debug_/PhysX3DEBUG_x86.lib")
	#pragma comment(lib, "External/PhysX_3.4/libx86_/Debug_/PhysX3ExtensionsDEBUG.lib")
	#pragma comment(lib, "External/PhysX_3.4/libx86_/Debug_/PxFoundationDEBUG_x86.lib")
	#pragma comment(lib, "External/PhysX_3.4/libx86_/Debug_/PxPvdSDKDEBUG_x86.lib")
	#pragma comment(lib, "External/PhysX_3.4/libx86_/Debug_/PhysX3CharacterKinematicDEBUG_x86.lib")
	#pragma comment(lib, "External/PhysX_3.4/libx86_/Debug_/SceneQueryDEBUG.lib")
	#pragma comment(lib, "External/PhysX_3.4/libx86_/Debug_/PhysX3CookingDEBUG_x86.lib")
#endif // _DEBUG

PhysicsEngine::PhysicsEngine(Application* app, bool active) : Module(app, active)
{
}

PhysicsEngine::~PhysicsEngine()
{
}

bool PhysicsEngine::Start()
{
	LOG("Initializing PhysX 3.4 ---");
	static physx::PxDefaultErrorCallback gDefaultErrorCallback;
	static physx::PxDefaultAllocator gDefaultAllocatorCallback;

	foundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	if (!foundation)
	{
		LOG("PxCreateFoundation failed!");
		return false;
	}
	LOG("Physics Foundation created succesfully");

	bool recordMemoryAllocations = true;

	physics = PxCreateBasePhysics(PX_PHYSICS_VERSION, *foundation, physx::PxTolerancesScale(), recordMemoryAllocations);
	if (!physics)
	{
		LOG("PxCreatePhysics failed!");
		return false;
	}
	LOG("Physics created succesfully");

	cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, physx::PxCookingParams(physx::PxTolerancesScale()));
	if (!cooking)
	{
		LOG("PxCreateCooking failed!");
		return false;
	}
	LOG("Physics Cooking created succesfully");

	physx::PxSceneDesc sceneDesc(physics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -gravity, 0.0f);
	sceneDesc.bounceThresholdVelocity = gravity * BOUNCE_THRESHOLD;
	sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(THREADS);
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_KINEMATIC_PAIRS | physx::PxSceneFlag::eENABLE_KINEMATIC_STATIC_PAIRS;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	scene = physics->createScene(sceneDesc);
	if (!scene)
	{
		LOG("createScene failed!");
		return false;
	}
	LOG("Physics Scene created succesfully");

	controllerManager = PxCreateControllerManager(*scene);
	if (!controllerManager)
	{
		LOG("PxCreateControllerManager failed!");
		return false;
	}

	material = physics->createMaterial(2, 1, 0.25);

	LOG("Physics Controller Manager created succesfully");
	LOG("PhysX 3.4 Initialized correctly ---");
	
	return true;
}

update_status PhysicsEngine::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status PhysicsEngine::Update(float dt)
{
	if (scene && !App->paused)
	{
		scene->simulate(dt);
		scene->fetchResults(true);
	}

	return UPDATE_CONTINUE;
}

update_status PhysicsEngine::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool PhysicsEngine::CleanUp()
{
	if (controllerManager)
		controllerManager->release();
	if (material)
		material->release();
	if (cooking)
		cooking->release();
	if (scene)
		scene->release();
	if (physics)
		physics->release();
	if (pvd)
		pvd->release();
	if (foundation)
		foundation->release();
	if (raycastManager)
		delete raycastManager;

	controllerManager = nullptr;
	physics = nullptr;
	foundation = nullptr;
	scene = nullptr;
	pvd = nullptr;

	return true;
}

void PhysicsEngine::AddActor(physx::PxActor* actor)
{
	if (actor)
		scene->addActor(*actor);
}

void PhysicsEngine::DeleteActor(physx::PxActor* actor)
{
	if (actor)
		scene->removeActor(*actor);
}

float* PhysicsEngine::GetGravityPtr()
{
	return &gravity;
}

void PhysicsEngine::SetGravity()
{
	scene->setGravity(physx::PxVec3(0.0f, -gravity, 0.0f));
}

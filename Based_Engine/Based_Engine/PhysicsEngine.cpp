#include "Application.h"
#include "ObjectManager.h"
#include "PhysicsEngine.h"
#include "Application.h"
#include "GameObject.h"

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
	static physx::PxDefaultErrorCallback gDefaultErrorCallback;
	static physx::PxDefaultAllocator gDefaultAllocatorCallback;

	foundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	if (!foundation)
	{
		LOG("Error creating fundation");
		return false;
	}

	cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, physx::PxCookingParams(physx::PxTolerancesScale()));
	if (!cooking)
	{
		LOG("Error creating cooking");
		return false;
	}

	physx::PxCookingParams params = cooking->getParams();
	params.convexMeshCookingType = physx::PxConvexMeshCookingType::eQUICKHULL;
	params.gaussMapLimit = 32;
	cooking->setParams(params);

#ifdef _DEBUG
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("localhost", 5425, 10000);
	if (transport == NULL)
	{
		LOG("Error creating transport socket")
		return false;
	}

	physx::PxPvdInstrumentationFlags pvdFlags = physx::PxPvdInstrumentationFlag::eALL;
	pvd = physx::PxCreatePvd(*foundation);
	pvd->connect(*transport, pvdFlags);

	physics = PxCreateBasePhysics(PX_PHYSICS_VERSION, *foundation, physx::PxTolerancesScale(), true, pvd);
#else
	physics = PxCreateBasePhysics(PX_PHYSICS_VERSION, *foundation, physx::PxTolerancesScale(), true);
#endif
	
	if (!physics) 
	{
		LOG("Error creating the base physics");
		return false;
	}

	PxRegisterParticles(*physics);

	physx::PxSceneDesc sceneDesc(physics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -GRAVITY, 0.0f);
	sceneDesc.bounceThresholdVelocity = GRAVITY * BOUNCE_THRESHOLD;
	sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(THREADS);
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_KINEMATIC_PAIRS | physx::PxSceneFlag::eENABLE_KINEMATIC_STATIC_PAIRS;

	scene = physics->createScene(sceneDesc);
	if (!scene)
	{
		LOG("Error creating scene");
		return false;
	}

#ifdef _DEBUG
	scene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0);
	scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
	scene->setVisualizationParameter(physx::PxVisualizationParameter::eACTOR_AXES, 1.0f);

	//Setup Configuration-----------------------------------------------------------------------
	pvdClient = scene->getScenePvdClient();
	if (pvdClient) 
	{
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	//-------------------------------------
#endif

	controllerManager = PxCreateControllerManager(*scene);

	return true;
}

update_status PhysicsEngine::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status PhysicsEngine::Update(float dt)
{
	scene->simulate(dt);
	scene->fetchResults(true);

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
#ifdef _DEBUG
	if (pvd)
		pvd->release();
#endif
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
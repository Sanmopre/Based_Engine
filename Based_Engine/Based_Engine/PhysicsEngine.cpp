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

	mFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	if (!mFoundation)
		LOG("PxCreateFoundation failed!");

	mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, physx::PxCookingParams(physx::PxTolerancesScale()));
	if (!mCooking)
	{
		LOG("PxCreateCooking failed!");
	}
	else 
	{
		physx::PxCookingParams params = mCooking->getParams();
		params.convexMeshCookingType = physx::PxConvexMeshCookingType::eQUICKHULL;
		params.gaussMapLimit = 32;
		mCooking->setParams(params);
	}

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


#pragma once
#include "Module.h"
#include "PxPhysicsAPI.h"

namespace physx
{
	class PxPvd;
	class PxPvdSceneClient;
	class PxFoundation;
	class PxPhysics;
	class PxScene;
	class PxMaterial;
	class PxRigidStatic;
	class PxControllerManager;
	class PxRigidActor;
	class PxVolumeCache;
	class PxSimulationEventCallback;
	class PxActorShape;
	class PxQueryFilterCallback;
	class RaycastCCDManager;
	class PxCooking;
	class PxConvexMesh;
	class PxBase;

	typedef uint32_t PxU32;
};

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

public:

	physx::PxPvd* mPvd = nullptr;
	physx::PxCooking* mCooking = nullptr;
	physx::PxPvdSceneClient* pvdClient = nullptr;
	physx::PxFoundation* mFoundation = nullptr;
	physx::PxControllerManager* mControllerManager = nullptr;
	physx::PxPhysics* mPhysics = nullptr;
	physx::PxScene* mScene = nullptr;
	physx::PxMaterial* mMaterial = nullptr;
	physx::PxRigidStatic* plane = nullptr;
	physx::RaycastCCDManager* raycastManager = nullptr;

private:

	float gravity = 9.8;
};
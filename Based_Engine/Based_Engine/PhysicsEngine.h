#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "Module.h"
#include "MathGeoLib.h"

#define GRAVITY 9.8f

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
	class PxActor;
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

	void AddActor(physx::PxActor*);
	void DeleteActor(physx::PxActor*);

public:

	physx::PxPvd* pvd = nullptr;
	physx::PxCooking* cooking = nullptr;
	physx::PxPvdSceneClient* pvdClient = nullptr;
	physx::PxFoundation* foundation = nullptr;
	physx::PxControllerManager* controllerManager = nullptr;
	physx::PxPhysics* physics = nullptr;
	physx::PxScene* scene = nullptr;
	physx::PxMaterial* material = nullptr;
	physx::PxRigidStatic* plane = nullptr;
	physx::RaycastCCDManager* raycastManager = nullptr;
};

#endif //__PHYSICS_H__
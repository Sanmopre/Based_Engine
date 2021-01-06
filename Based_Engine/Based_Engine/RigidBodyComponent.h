#ifndef __RIGIDBODYCOMPONENT_H__
#define __RIGIDBODYCOMPONENT_H__

#include "Component.h"
#include "PxRigidActor.h"
#include "MathGeoLib.h"
#include "PxPhysicsAPI.h"

enum class State {
	STATIC,
	KINEMATIC,
	NONE
};
class RigidBodyComponent : public Component
{
public:

	RigidBodyComponent(char* name , GameObject* parent, Application* app, bool active = true);
	virtual ~RigidBodyComponent();

	bool Update(float dt);

	void DisplayComponentMenu();

	inline physx::PxReal GetMass() { return rigidBody->getMass(); }
	inline void SetMass(physx::PxReal mass) { if (rigidBody)rigidBody->setMass(mass); }
	inline void SetDensity(physx::PxReal density) { physx::PxRigidBodyExt::updateMassAndInertia(*rigidBody, density); }
	
	inline void UseGravity(bool enable) { if (rigidBody)rigidBody->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !enable); }
	inline void SetKinematic(bool enable) { if (rigidBody)rigidBody->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, enable); }
	
	inline float3 GetLinearVelocity() { return float3(rigidBody->getLinearVelocity().x, rigidBody->getLinearVelocity().y, rigidBody->getLinearVelocity().z); }
	inline float3 GetAngularVelocity() { return float3(rigidBody->getAngularVelocity().x, rigidBody->getAngularVelocity().y, rigidBody->getAngularVelocity().z); }
	inline void SetLinearVelocity(float3 vel) { if (rigidBody)rigidBody->setLinearVelocity(physx::PxVec3(vel.x, vel.y, vel.z)); }
	inline void SetAngularVelocity(float3 vel) { if (rigidBody)rigidBody->setAngularVelocity(physx::PxVec3(vel.x, vel.y, vel.z)); }
	
	inline void SetAngularDamping(physx::PxReal value) { if (rigidBody)rigidBody->setAngularDamping(value); }
	inline void SetLinearDamping(physx::PxReal value) { if (rigidBody)rigidBody->setLinearDamping(value); }
	inline physx::PxReal GetAngularDamping() { return rigidBody->getAngularDamping(); }
	inline physx::PxReal GetLinearDamping() { return rigidBody->getLinearDamping(); }
	
	inline void AddForce(physx::PxVec3 force, physx::PxForceMode::Enum mode) { if (rigidBody) rigidBody->addForce(force, mode); }
	inline void AddTorque(physx::PxVec3 force, physx::PxForceMode::Enum mode) { if (rigidBody)rigidBody->addTorque(force, mode); }
	
	inline void FreezePosition_X(bool enable) { if (rigidBody)rigidBody->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, enable); }
	inline void FreezePosition_Y(bool enable) { if (rigidBody)rigidBody->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, enable); }
	inline void FreezePosition_Z(bool enable) { if (rigidBody)rigidBody->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, enable); }
	inline void FreezeRotation_X(bool enable) { if (rigidBody)rigidBody->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, enable); }
	inline void FreezeRotation_Y(bool enable) { if (rigidBody)rigidBody->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, enable); }
	inline void FreezeRotation_Z(bool enable) { if (rigidBody)rigidBody->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, enable); }

	physx::PxRigidDynamic* rigidBody = nullptr;
	bool update = false;
	
	void UpdateRBValues();
	void setRBValues();

private:

	void StaticToDynamicRigidBody();

	void ApplyPhysicsChanges();

	void UpdateTransformByRigidBody();

	float mass = 10.f;
	float density = 1.f;
	bool useGravity = true;
	bool isKinematic = false;
	float3 linearVel = float3::zero;
	float3 angularVel = float3::zero;
	float linearDamping = 0.f;
	float angularDamping = 0.f;
	bool freezePositionX = false, freezePositionY = false, freezePositionZ = false;
	bool freezeRotationX = false, freezeRotationY = false, freezeRotationZ = false;
	bool toPlay = false;


	float massBuffer = 0.0f;
	float densityBuffer = 0.0f;

	float4x4 localMatrix = float4x4::identity;
	float4x4 globalMatrix = float4x4::identity;
};

#endif //__PHYSICSCOMPONENT_H__
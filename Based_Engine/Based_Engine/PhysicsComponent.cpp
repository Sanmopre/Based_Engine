#include "PhysicsComponent.h"

//#include "PxActor.h"
//#include "PxRigidActor.h"
//#include "PxRigidBody.h"
#include "Application.h"
#include "GameObject.h"
#include "Transform.h"
#include "Input.h"

#include "GL/glew.h"

#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h" 

PhysicsComponent::PhysicsComponent(char* name, GameObject* parent, Application* app, bool active) : Component(name, parent, app, active)
{
	if (rigidBody != nullptr)
	{
		SetMass(mass);
		SetDensity(density);
		UseGravity(use_gravity);
		SetKinematic(is_kinematic);
		SetLinearVelocity(linear_vel);
		SetAngularVelocity(angular_vel);
		SetLinearDamping(linear_damping);
		SetAngularDamping(angular_damping);
		FeezePosition_X(freezePosition_X);
		FeezePosition_Y(freezePosition_Y);
		FeezePosition_Z(freezePosition_Z);
		FreezeRotation_X(freezeRotation_X);
		FreezeRotation_Y(freezeRotation_Y);
		FreezeRotation_Z(freezeRotation_Z);
	}
}

PhysicsComponent::~PhysicsComponent()
{
}

bool PhysicsComponent::Update(float dt)
{
	setRBValues();

	UpdateRBValues();

	return true;
}


void PhysicsComponent::DisplayComponentMenu()
{
}

void PhysicsComponent::StaticToDynamicRigidBody()
{
	if (/*parent->colliderComponent != nullptr &&*/ rigidBody == nullptr)
	{
		//parent->colliderComponent->collider->CreateCollider(collider->type, true);
		update = true;
	}
}

void PhysicsComponent::UpdateRBValues()
{
	if (rigidBody != nullptr && update) {
	SetMass(mass);
	SetDensity(density);
	UseGravity(use_gravity);
	SetKinematic(is_kinematic);
	SetLinearVelocity(linear_vel);
	SetAngularVelocity(angular_vel);
	SetLinearDamping(linear_damping);
	SetAngularDamping(angular_damping);
	FeezePosition_X(freezePosition_X);
	FeezePosition_Y(freezePosition_Y);
	FeezePosition_Z(freezePosition_Z);
	FreezeRotation_X(freezeRotation_X);
	FreezeRotation_Y(freezeRotation_Y);
	FreezeRotation_Z(freezeRotation_Z);
	update = false;
}
}

void PhysicsComponent::setRBValues()
{
	if (rigidBody != nullptr)
	{
		if (linear_vel.x != 0.0f || linear_vel.y != 0.0f || linear_vel.z != 0.0f)
			SetLinearVelocity(linear_vel);

		if (angular_vel.x != 0.0f || angular_vel.y != 0.0f || angular_vel.z != 0.0f)
			SetAngularVelocity(angular_vel);
	}
}

void PhysicsComponent::UpdateTransformByRigidBody(physx::PxTransform* globalPos)
{
	physx::PxTransform transform;
	if (rigidBody != nullptr) {

		rigidBody->setGlobalPose(*globalPos);

		transform = rigidBody->getGlobalPose();
		float3 position = float3(

			(transform.p.x - globalMatrix.x) + parent->transform->global_transformation.x,
			(transform.p.y - globalMatrix.y) + parent->transform->global_transformation.y,
			(transform.p.z - globalMatrix.z) + parent->transform->global_transformation.z);

		float4x4 new_transform = float4x4::FromTRS(
			position,
			Quat(transform.q.x, transform.q.y, transform.q.z, transform.q.w),
			parent->transform->global_transformation.Transposed().GetScale());

		parent->transform->SetTransform(new_transform);
	}
}


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

	if (to_delete) {
		//Delete
	}

	return true;
}


void PhysicsComponent::DisplayComponentMenu()
{
}

void PhysicsComponent::StaticToDynamicRigidBody()
{
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


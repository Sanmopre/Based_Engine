#include "ConstraintComponent.h"

#include "Application.h"
#include "GameObject.h"
#include "Transform.h"
#include "Input.h"
#include "PhysicsEngine.h"

#include "GL/glew.h"

#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h" 

ConstraintComponent::ConstraintComponent(char* name, GameObject* parent, Application* app, bool active) : Component(name, parent, app, active)
{
}

ConstraintComponent::~ConstraintComponent()
{

}

bool ConstraintComponent::Update(float dt)
{
	return true;
}

void ConstraintComponent::DisplayComponentMenu()
{
	
}

void ConstraintComponent::CreateConstraint(PxRigidActor* actor0, const PxTransform& localFrame0, PxRigidActor* actor1, const PxTransform& localFrame1)
{
	physx::PxRevoluteJointCreate(*App->physics->physics, actor0, localFrame0, actor1, localFrame1);
}

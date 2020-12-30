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

}

PhysicsComponent::~PhysicsComponent()
{
}

bool PhysicsComponent::Update(float dt)
{

	return true;
}


void PhysicsComponent::DisplayComponentMenu()
{
}
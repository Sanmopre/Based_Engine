#include "ColliderComponent.h"

#include "Application.h"
#include "GameObject.h"
#include "Transform.h"
#include "Input.h"

#include "GL/glew.h"

#include "PxPhysicsAPI.h"
#include "PxSimulationEventCallback.h"

#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h" 

ColliderComponent::ColliderComponent(char* name, colider_type col_type, GameObject* parent, Application* app, bool active) : Component(name, parent, app, active)
{
	isTrigger = false;
	type = col_type;
}

ColliderComponent::~ColliderComponent()
{

}

bool ColliderComponent::Update(float dt)
{

	return true;
}

void ColliderComponent::Enable()
{
}

void ColliderComponent::Disable()
{
}

void ColliderComponent::CreateCollider(colider_type type, bool createAgain)
{
	if (shape != nullptr && (lastIndex != (int)type || createAgain)) {
		shape->release();
		shape = nullptr;
		if (/*parent->physicsComponent != nullptr*/true)
		{
			if (/*parent->physicsComponent->rigidBody != nullptr*/true) {
				//App->physics->DeleteActor(parent->physicsComponent->rigidBody);
				//parent->physicsComponent->rigidBody = nullptr;
			}
			if (createAgain && rigidStatic) {
				//App->physics->DeleteActor(rigidStatic);
				rigidStatic = nullptr;
			}
		}
		else {
			//App->physics->DeleteActor(rigidStatic);
			rigidStatic = nullptr;
		}


		/*if (mesh && mesh->IsInMemory())
		{
			mesh->Release();
		}*/
	}

	if (lastIndex == (int)type && !createAgain) {
		return;
	}

	physx::PxTransform localTransform(physx::PxVec3(centerPosition.x, centerPosition.y, centerPosition.z));
	parent->transform;
	float3 tScale = parent->transform->global_transformation.GetScale();

	float3 pos, scale;
	Quat quat;
	parent->transform->global_transformation.Decompose(pos, quat, scale);

	switch (type) {
	//cases with collider types
	}
	editCollider = false;
}

void ColliderComponent::UpdateCollider()
{
}

void ColliderComponent::UpdateLocalMatrix()
{
}


void ColliderComponent::UpdateTransformByRigidBody(physx::PxTransform* globalPos)
{
}

void ColliderComponent::DisplayComponentMenu()
{
}
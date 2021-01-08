#include "ColliderComponent.h"

#include "Application.h"
#include "GameObject.h"
#include "Transform.h"
#include "Input.h"
#include "PhysicsEngine.h"
#include "RigidBodyComponent.h"
#include "MeshComponent.h"

#include "GL/glew.h"

#include "PxPhysicsAPI.h"
#include "PxSimulationEventCallback.h"

#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h" 

ColliderComponent::ColliderComponent(char* name, colider_type col_type, GameObject* parent, Application* app, bool active) : Component(name, parent, app, active)
{
	type = col_type;
	isTrigger = false;
	type = col_type;
	CreateCollider(type, false);
}

ColliderComponent::~ColliderComponent()
{
	if (parent->rigidbody && parent->rigidbody->IsStatic())
		parent->rigidbody->to_delete = true;

	if (shape)
		shape->release();
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
	if (shape)
	{
		shape->release();
		shape = nullptr;
	}

	if (parent->rigidbody)
		App->physics->DeleteActor(parent->rigidbody->rigidBody);
	else
	{
		parent->AddRigidBodyComponent();
		parent->rigidbody->MakeStatic();
	}

	float3 size = parent->transform->GetGlobalScale();

	switch (type) 
	{
		case colider_type::BOX:
		{
			physx::PxBoxGeometry boxGeometry = physx::PxBoxGeometry(physx::PxVec3(size.x / 2, size.y / 2, size.z / 2));
			shape = App->physics->physics->createShape(boxGeometry, *App->physics->material);
			break;
		}
		case colider_type::SPHERE:
		{
			physx::PxSphereGeometry SphereGeometry(radius);
			shape = App->physics->physics->createShape(SphereGeometry, *App->physics->material);
			break;
		}
		case colider_type::CAPSULE:
		{
			physx::PxCapsuleGeometry CapsuleGeometry(radius, height);
			shape = App->physics->physics->createShape(CapsuleGeometry, *App->physics->material);
			break;
		}
	}

	parent->rigidbody->rigidBody->attachShape(*shape);
	parent->rigidbody->update = true;
	parent->rigidbody->ApplyPhysicsChanges();

	App->physics->AddActor(parent->rigidbody->rigidBody);
}
void ColliderComponent::UpdateCollider()
{
	CreateCollider(type, true);
}

void ColliderComponent::UpdateLocalMatrix()
{
}


void ColliderComponent::UpdateTransformByRigidBody(physx::PxTransform* globalPos)
{
}

void ColliderComponent::DisplayComponentMenu()
{
	std::string uiName;
	switch (type)
	{
	case colider_type::BOX:
		uiName = "Box Colider [" + name + "]";
		break;
	case colider_type::SPHERE:
		uiName = "Sphere Colider [" + name + "]";
		break;
	case colider_type::CAPSULE:
		uiName = "Capsule Colider [" + name + "]";
		break;
	case colider_type::MESH: 
		uiName = "Mesh Colider [" + name + "]";
		break;
	}
	std::string active = "active [" + name + "]";
	std::string deleted = "delete [" + name + "]";
	std::string trigger = "is triger [" + name + "]";
	std::string centr = "center [" + name + "]";
	std::string sze = "size [" + name + "]";
	if (ImGui::CollapsingHeader(uiName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox(active.c_str(), &to_activate);
		ImGui::SameLine();

		if (ImGui::Button(deleted.c_str()))
			to_delete = true;

		if (ImGui::Checkbox(trigger.c_str(), &isTrigger))
			isTrigger = !isTrigger;

		float c[3] = { centerPosition.x, centerPosition.y, centerPosition.z };
		ImGui::InputFloat3(centr.c_str(), c);

		float s[3] = { colliderSize.x, colliderSize.y, colliderSize.z };
		ImGui::InputFloat3(sze.c_str(), s);
	}
}

template <class Geometry>
void ColliderComponent::CreateRigidbody(Geometry geometry, physx::PxTransform position) {

	if (!HasDynamicRigidBody(geometry, position))
	{
		if (rigidStatic)
			App->physics->DeleteActor(rigidStatic);

		if (isTrigger) {
			shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
			shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
		}
		else {
			shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
			shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
		}

		physx::PxFilterData filterData;

		shape->setSimulationFilterData(filterData);
		shape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);
		shape->setQueryFilterData(filterData);

		rigidStatic = PxCreateStatic(*App->physics->physics, position, *shape);

		App->physics->AddActor(rigidStatic);

	}
}

template <class Geometry>
bool ColliderComponent::HasDynamicRigidBody(Geometry geometry, physx::PxTransform transform)
{
	if (parent->rigidbody != nullptr)
	{
		if (rigidStatic) {
			App->physics->DeleteActor(rigidStatic);
		}
		if (parent->rigidbody->rigidBody) {
			App->physics->DeleteActor(parent->rigidbody->rigidBody);
		}

		shape = App->physics->physics->createShape(geometry, *App->physics->material);
		
		if (isTrigger) {
			shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
			shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
		}
		else {
			shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
			shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
		}

		physx::PxFilterData filterData;
		shape->setSimulationFilterData(filterData);
		shape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);
		shape->setQueryFilterData(filterData);


		if (parent->rigidbody->rigidBody) 
		{
			parent->rigidbody->rigidBody->attachShape(*shape); // = PxCreateDynamic(*App->physics->physics, transform, *shape, 1.0f);
			parent->rigidbody->update = true;
			parent->rigidbody->ApplyPhysicsChanges();
		

	//	App->physics->DeleteActor(parent->rigidbody->rigidBody);
	//	parent->rigidbody->rigidBody->setGlobalPose(physx::PxTransform(position.x, position.y, position.z, physx::PxQuat(rot.x, rot.y, rot.z, rot.w)));
		App->physics->AddActor(parent->rigidbody->rigidBody);
		}
		return true;
	}
	return false;
}
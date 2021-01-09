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
			colliderSize = { size.x, size.y, size.z };
			physx::PxBoxGeometry boxGeometry = physx::PxBoxGeometry(physx::PxVec3(size.x / 2, size.y / 2, size.z / 2));
			shape = App->physics->physics->createShape(boxGeometry, *App->physics->material);
			break;
		}
		case colider_type::SPHERE:
		{
			physx::PxSphereGeometry SphereGeometry(radius * 1.5);
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

void ColliderComponent::DisplayComponentMenu()
{
	std::string uiName;
	switch (type)
	{
	case colider_type::BOX:
		BoxColliderUI();
		break;
	case colider_type::SPHERE:
		SphereColliderUI();
		break;
	case colider_type::CAPSULE:
		CapsuleColliderUI();
		break;
	//case colider_type::MESH: 
	//	uiName = "Mesh Colider [" + name + "]";
		break;
	}
}

void ColliderComponent::BoxColliderUI()
{
	std::string uiName = "Box Colider [" + name + "]";
	if (ImGui::CollapsingHeader(uiName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		std::string active = "active [" + name + "]";
		std::string deleted = "delete [" + name + "]";
		std::string trigger = "is triger [" + name + "]";
		std::string centr = "offset [" + name + "]";

		bool toUpdate = false;

		std::string sze = "size [" + name + "]";

		ImGui::Checkbox(active.c_str(), &to_activate);
		ImGui::SameLine();

		if (ImGui::Button(deleted.c_str()))
			to_delete = true;

		if (ImGui::Checkbox(trigger.c_str(), &isTrigger))
			isTrigger = !isTrigger;

		float c[3] = { centerPosition.x, centerPosition.y, centerPosition.z };
		if (ImGui::InputFloat3(centr.c_str(), c, 4, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			centerPosition.x = c[0];
			centerPosition.y = c[1];
			centerPosition.z = c[2];

			toUpdate = true;
		}

		float s[3] = { colliderSize.x, colliderSize.y, colliderSize.z };
		if (ImGui::InputFloat3(sze.c_str(), s, 4, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			colliderSize.x = s[0];
			colliderSize.y = s[1];
			colliderSize.z = s[2];

			toUpdate = true;
		}

		if (toUpdate)
		{
			parent->rigidbody->rigidBody->detachShape(*shape);
			shape->release();

			physx::PxBoxGeometry boxGeometry = physx::PxBoxGeometry(physx::PxVec3(colliderSize.x / 2, colliderSize.y / 2, colliderSize.z / 2));
			shape = App->physics->physics->createShape(boxGeometry, *App->physics->material);

			physx::PxVec3 p = physx::PxVec3(centerPosition.x, centerPosition.y, centerPosition.z);
			shape->setLocalPose(physx::PxTransform(p));

			parent->rigidbody->rigidBody->attachShape(*shape);
			parent->rigidbody->update = true;
			parent->rigidbody->ApplyPhysicsChanges();
		}
	}
}

void ColliderComponent::SphereColliderUI()
{
	std::string uiName = "Sphere Colider [" + name + "]";
	if (ImGui::CollapsingHeader(uiName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		std::string active = "active [" + name + "]";
		std::string deleted = "delete [" + name + "]";
		std::string trigger = "is triger [" + name + "]";
		std::string centr = "offset [" + name + "]";

		bool toUpdate = false;

		std::string rad = "radius [" + name + "]";

		ImGui::Checkbox(active.c_str(), &to_activate);
		ImGui::SameLine();

		if (ImGui::Button(deleted.c_str()))
			to_delete = true;

		if (ImGui::Checkbox(trigger.c_str(), &isTrigger))
			isTrigger = !isTrigger;

		float c[3] = { centerPosition.x, centerPosition.y, centerPosition.z };
		if (ImGui::InputFloat3(centr.c_str(), c, 4, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			centerPosition.x = c[0];
			centerPosition.y = c[1];
			centerPosition.z = c[2];

			toUpdate = true;
		}

		float r = radius;
		if (ImGui::InputFloat(rad.c_str(), &r, 1, 1, 4, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			radius = r;

			toUpdate = true;
		}

		if (toUpdate)
		{
			parent->rigidbody->rigidBody->detachShape(*shape);
			shape->release();

			physx::PxSphereGeometry SphereGeometry(radius);
			shape = App->physics->physics->createShape(SphereGeometry, *App->physics->material);

			physx::PxVec3 p = physx::PxVec3(centerPosition.x, centerPosition.y, centerPosition.z);
			shape->setLocalPose(physx::PxTransform(p));

			parent->rigidbody->rigidBody->attachShape(*shape);
			parent->rigidbody->update = true;
			parent->rigidbody->ApplyPhysicsChanges();
		}
	}
}

void ColliderComponent::CapsuleColliderUI()
{
	std::string uiName = "Capsule Colider [" + name + "]";
	if (ImGui::CollapsingHeader(uiName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		std::string active = "active [" + name + "]";
		std::string deleted = "delete [" + name + "]";
		std::string trigger = "is triger [" + name + "]";
		std::string centr = "offset [" + name + "]";

		bool toUpdate = false;

		std::string rad = "radius [" + name + "]";
		std::string he = "height [" + name + "]";

		ImGui::Checkbox(active.c_str(), &to_activate);
		ImGui::SameLine();

		if (ImGui::Button(deleted.c_str()))
			to_delete = true;

		if (ImGui::Checkbox(trigger.c_str(), &isTrigger))
			isTrigger = !isTrigger;

		float c[3] = { centerPosition.x, centerPosition.y, centerPosition.z };
		if (ImGui::InputFloat3(centr.c_str(), c, 4, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			centerPosition.x = c[0];
			centerPosition.y = c[1];
			centerPosition.z = c[2];

			toUpdate = true;
		}

		float r = radius;
		if (ImGui::InputFloat(rad.c_str(), &r, 1, 1, 4, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			radius = r;

			toUpdate = true;
		}
		float h = height;
		if (ImGui::InputFloat(he.c_str(), &h, 1, 1, 4, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			height = h;

			toUpdate = true;
		}

		if (toUpdate)
		{
			parent->rigidbody->rigidBody->detachShape(*shape);
			shape->release();

			physx::PxCapsuleGeometry CapsuleGeometry(radius, height);
			shape = App->physics->physics->createShape(CapsuleGeometry, *App->physics->material);

			physx::PxVec3 p = physx::PxVec3(centerPosition.x, centerPosition.y, centerPosition.z);
			shape->setLocalPose(physx::PxTransform(p));

			parent->rigidbody->rigidBody->attachShape(*shape);
			parent->rigidbody->update = true;
			parent->rigidbody->ApplyPhysicsChanges();
		}
	}
}

#include "ConstraintComponent.h"

#include "Application.h"
#include "GameObject.h"
#include "Transform.h"
#include "Input.h"
#include "PhysicsEngine.h"

#include "GameObject.h"
#include "RigidBodyComponent.h"

#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h" 

#include "PxPhysicsAPI.h"

ConstraintComponent::ConstraintComponent(char* name, JointType type, GameObject* parent, Application* app, bool active)
	: Component(name, parent, app, active)
{
}

ConstraintComponent::~ConstraintComponent()
{
	if (constraint)
		constraint->release();
}

bool ConstraintComponent::Update(float dt)
{
	if (!parent->rigidbody)
	{
		if (constraint)
		{
			constraint->release();
			constraint = nullptr;
		}
	}
	else if (!constraint)
	{
		physx::PxTransform transform = physx::PxTransform(position.x, position.y, position.z);
		switch (type)
		{
		case JointType::SINGLE:
			CreateConstraint(transform);
			break;
		}
	}
	return true;
}

void ConstraintComponent::DisplayComponentMenu()
{
	if (!parent->rigidbody || !constraint)
	{
		ImGui::Text("A Constrain Component needs a RigidBody to function.");
		return;
	}

	std::string uiName = "Single Joint [" + name + "]";
	if (ImGui::CollapsingHeader(uiName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		std::string actived = "active [" + name + "]";
		std::string deleted = "delete [" + name + "]";
		std::string centr = "position [" + name + "]";

		bool toUpdate = false;

		ImGui::Checkbox(actived.c_str(), &active);
		ImGui::SameLine();

		if (ImGui::Button(deleted.c_str()))
			to_delete = true;

		float p[3] = { position.x, position.y, position.z };
		if (ImGui::InputFloat3(centr.c_str(), p, 4, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			position.x = p[0];
			position.y = p[1];
			position.z = p[2];

			toUpdate = true;
		}

		if (toUpdate)
		{
			constraint->release();
			constraint = nullptr;

			physx::PxTransform transform = physx::PxTransform(position.x, position.y, position.z);
			switch (type)
			{
			case JointType::SINGLE:
				CreateConstraint(transform);
				break;
			}
		}
	}
}

void ConstraintComponent::CreateConstraint(const physx::PxTransform& localFrame)
{
	physx::PxTransform transform = physx::PxTransform(0, 0, 0);
	constraint = physx::PxRevoluteJointCreate(*App->physics->physics, parent->rigidbody->rigidBody, transform, NULL, localFrame);

	parent->rigidbody->rigidBody->wakeUp();
}

#include "RigidBodyComponent.h"

//#include "PxActor.h"
//#include "PxRigidActor.h"
//#include "PxRigidBody.h"
#include "Application.h"
#include "GameObject.h"
#include "Transform.h"
#include "Input.h"
#include "PhysicsEngine.h"

#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h" 

RigidBodyComponent::RigidBodyComponent(char* name, GameObject* parent, Application* app, bool active) : Component(name, parent, app, active)
{
	float3 position = parent->transform->GetGlobalPosition();
	Quat rotation = parent->transform->GetGlobalRotation();
	rigidBody = App->physics->physics->createRigidDynamic(
		physx::PxTransform(physx::PxVec3(position.x, position.y, position.z), 
		physx::PxQuat(rotation.x, rotation.y, rotation.z, rotation.w)));

	if (rigidBody != nullptr)
	{
		massBuffer = mass;
		densityBuffer = density;

		SetMass(mass);
		SetDensity(density);
		UseGravity(useGravity);
		SetKinematic(isKinematic);
		SetLinearVelocity(linearVel);
		SetAngularVelocity(angularVel);
		SetLinearDamping(linearDamping);
		SetAngularDamping(angularDamping);
		FreezePosition_X(freezePositionX);
		FreezePosition_Y(freezePositionY);
		FreezePosition_Z(freezePositionZ);
		FreezeRotation_X(freezeRotationX);
		FreezeRotation_Y(freezeRotationY);
		FreezeRotation_Z(freezeRotationZ);
	}

	App->physics->AddActor(rigidBody);
}

RigidBodyComponent::~RigidBodyComponent()
{
	if (rigidBody)
	{
		App->physics->DeleteActor(rigidBody);
		rigidBody->release();

		parent->rigidbody = nullptr;
	}
}

bool RigidBodyComponent::Update(float dt)
{
	if (App->paused)
		return true;

	if (rigidBody != nullptr)
		UpdateTransformByRigidBody();
	
	ApplyPhysicsChanges();

	return true;
}

void RigidBodyComponent::DisplayComponentMenu()
{
	if (isStatic)
	{
		ImGui::CollapsingHeader("Static RigidBody", ImGuiTreeNodeFlags_Leaf);
		return;
	}

	if (ImGui::CollapsingHeader("RigidBody", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Is active", &to_activate);
		ImGui::SameLine();

		if (ImGui::Button("delete rigidbody"))
			to_delete = true;

		if (ImGui::InputFloat("Mass", &massBuffer, 0, 0, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
		{
			mass = massBuffer;
			update = true;
		}
		if (ImGui::InputFloat("Density", &densityBuffer, 0, 0, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
		{
			density = densityBuffer;
			update = true;
		}

		if (ImGui::Checkbox("Use gravity", &useGravity))
			update = true;
		if (ImGui::Checkbox("Is kinamatic", &isKinematic))
			update = true;

		ImGui::Text("Freeze");
		ImGui::Separator();
		if (ImGui::Checkbox("position X", &freezePositionX))
			update = true;
		ImGui::SameLine();
		if (ImGui::Checkbox("position Y", &freezePositionY))
			update = true;
		ImGui::SameLine();
		if (ImGui::Checkbox("position Z", &freezePositionZ))
			update = true;
		ImGui::Separator();
		if (ImGui::Checkbox("rotation X", &freezeRotationX))
			update = true;
		ImGui::SameLine();
		if (ImGui::Checkbox("rotation Y", &freezeRotationY))
			update = true;
		ImGui::SameLine();
		if (ImGui::Checkbox("rotation Z", &freezeRotationZ))
			update = true;
	}
}

void RigidBodyComponent::ApplyPhysicsChanges()
{
	if (rigidBody != nullptr && update) 
	{
		App->physics->DeleteActor(rigidBody);

		SetMass(mass);
		SetDensity(density);
		UseGravity(useGravity);
		SetKinematic(isKinematic);
		SetLinearVelocity(linearVel);
		SetAngularVelocity(angularVel);
		SetLinearDamping(linearDamping);
		SetAngularDamping(angularDamping);
		FreezePosition_X(freezePositionX);
		FreezePosition_Y(freezePositionY);
		FreezePosition_Z(freezePositionZ);
		FreezeRotation_X(freezeRotationX);
		FreezeRotation_Y(freezeRotationY);
		FreezeRotation_Z(freezeRotationZ);

		App->physics->AddActor(rigidBody);

		rigidBody->wakeUp();
		update = false;
	}
}

void RigidBodyComponent::setRBValues()
{
	if (rigidBody != nullptr)
	{
		if (linearVel.x != 0.0f || linearVel.y != 0.0f || linearVel.z != 0.0f)
			SetLinearVelocity(linearVel);

		if (angularVel.x != 0.0f || angularVel.y != 0.0f || angularVel.z != 0.0f)
			SetAngularVelocity(angularVel);
	}
}

void RigidBodyComponent::UpdateTransformByRigidBody()
{
	if (!rigidBody)
		return;

	physx::PxTransform transform = rigidBody->getGlobalPose();
	float3 position = float3(transform.p.x, transform.p.y, transform.p.z);
	Quat rotation = Quat(transform.q.x, transform.q.y, transform.q.z, transform.q.w);
	float3 scale = parent->transform->GetGlobalScale();

	float4x4 newTransform = float4x4::FromTRS(position, rotation, scale);
	parent->transform->SetTransform(newTransform);
}

void RigidBodyComponent::UpdateRigidBodyByTransform(bool stop)
{
	if (!rigidBody)
		return;

	if (stop)
		SetLinearVelocity(float3::zero);

	App->physics->DeleteActor(rigidBody);

	float3 position = parent->transform->GetGlobalPosition();
	physx::PxVec3 newPosition = physx::PxVec3(position.x, position.y, position.z);
	Quat rotation = parent->transform->GetGlobalRotation();
	physx::PxQuat newRotation = physx::PxQuat(rotation.x, rotation.y, rotation.z, rotation.w);

	physx::PxTransform transform = physx::PxTransform(newPosition, newRotation);
	rigidBody->setGlobalPose(transform, true);

	App->physics->AddActor(rigidBody);
}

void RigidBodyComponent::MakeStatic()
{
	isStatic = true;

	freezePositionX = true;
	freezePositionY = true;
	freezePositionZ = true;
	freezeRotationX = true;
	freezeRotationY = true;
	freezeRotationZ = true;
	useGravity = false;
	massBuffer = 1000.0f;

	update = true;
	ApplyPhysicsChanges();
}

void RigidBodyComponent::MakeDynamic()
{
	isStatic = false;

	freezePositionX = false;
	freezePositionY = false;
	freezePositionZ = false;
	freezeRotationX = false;
	freezeRotationY = false;
	freezeRotationZ = false;
	useGravity = true;
	massBuffer = 10.0f;

	update = true;
	ApplyPhysicsChanges();
}


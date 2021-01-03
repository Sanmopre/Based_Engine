#include "ColliderComponent.h"

#include "Application.h"
#include "GameObject.h"
#include "Transform.h"
#include "Input.h"
#include "PhysicsEngine.h"

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
				App->physics->DeleteActor(rigidStatic);
				rigidStatic = nullptr;
			}
		}
		else {
			App->physics->DeleteActor(rigidStatic);
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
	Transform* transform = parent->transform;
	float3 tScale = parent->transform->global_transformation.GetScale();

	float3 pos, scale;
	Quat quat;
	parent->transform->global_transformation.Decompose(pos, quat, scale);

	//////////////////////////////////////////////////////////////////////////////////////////////

	switch (type) {
	case colider_type::BOX: {

		float3 center;

		physx::PxBoxGeometry boxGeometry;
		Quat q = quat;
		Quat qInverse = q;
		if (!firstCreation)
		{
			qInverse.InverseAndNormalize();
			Quat tmp = q * qInverse;
			//parent->transform->global_transformation->SetRotation(tmp);

			//UpdateAABB();

			if (parent->meshComp != nullptr)
				originalSize = {0,0,0}; //GetOBB().Size().Div(scale);
			else
				originalSize = float3::one;

			//center = GetAABB().CenterPoint();

		}

		boxGeometry = physx::PxBoxGeometry(physx::PxVec3(originalSize.x * scale.x * colliderSize.x * 0.5, originalSize.y * scale.y * colliderSize.y * 0.5, originalSize.z * scale.z * colliderSize.z * 0.5));

		physx::PxTransform position(physx::PxVec3(center.x, center.y, center.z));

		shape = App->physics->physics->createShape(boxGeometry, *App->physics->material);

		//CreateRigidbody(boxGeometry, position);

		if (!firstCreation)
		{
			//transform->SetRotation(q); //RESET TO ORIGNAL ROTATION

			//UpdateAABB();
			firstCreation = true;

			//center = GO->GetOBB().CenterPoint();
			float3 dir = center - transform->GetGlobalPosition();
			float3 dir2 = quat.Inverted().Mul(dir); // rotate it
			offset = (dir2.Div(scale));

			offset.Mul(scale);
		}

		lastIndex = (int)colider_type::BOX;
		break;
	}
	case colider_type::SPHERE: {
		physx::PxSphereGeometry SphereGeometry(radius);
		shape = App->physics->physics->createShape(SphereGeometry, *App->physics->material);
		shape->setGeometry(SphereGeometry);

		//physx::PxTransform transform(transform->position.x, transform->position.y, transform->position.z);
		//CreateRigidbody(SphereGeometry, transform);

		lastIndex = (int)colider_type::SPHERE;
		break;
	}
	case colider_type::CAPSULE: {
		physx::PxCapsuleGeometry CapsuleGeometry(radius, height);
		shape = App->physics->physics->createShape(CapsuleGeometry, *App->physics->material);
		physx::PxTransform relativePose(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0, 0, 1)));
		shape->setLocalPose(relativePose);

		//physx::PxTransform transform(transform->position.x, transform->position.y, transform->position.z);
		//CreateRigidbody(CapsuleGeometry, transform);

		lastIndex = (int)colider_type::CAPSULE;
		break;
	}
	case colider_type::MESH: {

	}
	case colider_type::NONE: {
		lastIndex = -1;
		break;
	}
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

template <class Geometry>
void ColliderComponent::CreateRigidbody(Geometry geometry, physx::PxTransform position) {
	if (!HasDynamicRigidBody(geometry, position))
	{
		if (rigidStatic)
			App->physics->DeleteActor(rigidStatic);

		physx::PxFilterData filterData;
		filterData.word0 = (1 << GO->layer); // word0 = own ID
		filterData.word1 = App->physics->layer_list.at(GO->layer).LayerGroup; // word1 = ID mask to filter pairs that trigger a contact callback;

		if (isTrigger) {
			shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
			shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
		}
		else {
			shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
			shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
		}

		shape->setSimulationFilterData(filterData);
		shape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);
		shape->setQueryFilterData(filterData);

		rigidStatic = PxCreateStatic(*App->physics->mPhysics, position, *shape);

		App->physics->addActor(rigidStatic, GO);

	}
}

template <class Geometry>
bool ColliderComponent::HasDynamicRigidBody(Geometry geometry, physx::PxTransform transform)
{
	ComponentDynamicRigidBody* dynamicRB = GO->GetComponent<ComponentDynamicRigidBody>();

	if (dynamicRB != nullptr)
	{

		float3 position, scale = float3::zero;
		Quat rot = Quat::identity;

		globalMatrix.Decompose(position, rot, scale);

		if (rigidStatic) {
			App->physics->DeleteActor(rigidStatic);
		}
		if (dynamicRB->rigidBody) {
			App->physics->DeleteActor(dynamicRB->rigidBody);
		}

		shape = App->physics->mPhysics->createShape(geometry, *App->physics->mMaterial);

		if (isTrigger) {
			shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
			shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
		}
		else {
			shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
			shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
		}

		physx::PxFilterData filterData;
		filterData.word0 = (1 << GO->layer); // word0 = own ID
		filterData.word1 = App->physics->layer_list.at(GO->layer).LayerGroup; // word1 = ID mask to filter pairs that trigger a contact callback;

		shape->setSimulationFilterData(filterData);
		shape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);
		shape->setQueryFilterData(filterData);

		dynamicRB->rigidBody = PxCreateDynamic(*App->physics->mPhysics, transform, *shape, 1.0f);
		dynamicRB->update = true;
		dynamicRB->UpdateRBValues();

		dynamicRB->rigidBody->setGlobalPose(physx::PxTransform(position.x, position.y, position.z, physx::PxQuat(rot.x, rot.y, rot.z, rot.w)));

		App->physics->addActor(dynamicRB->rigidBody, GO);

		return true;
	}
	return false;
}
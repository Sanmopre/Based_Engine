#ifndef __COLLIDERCOMPONENT_H__
#define __COLLIDERCOMPONENT_H__

#include "Component.h"
#include "MathGeoLib.h"

enum class colider_type {
BOX,
SPHERE,
CAPSULE,
MESH,
NONE
};

namespace physx
{
	class PxRigidActor;
	class PxRigidStatic;
	class PxTransform;
	class PxShape;
	class PxConvexMesh;
	class PxTriangleMesh;
	class PxVec3;
	typedef uint16_t PxU16;
}

class Mesh;


class ColliderComponent : public Component
{
public:

	ColliderComponent(char* name,colider_type type, GameObject* parent, Application* app, bool active = true);
	virtual ~ColliderComponent();

	bool Update(float dt);
	void Enable();
	void Disable();

	void CreateCollider(colider_type type, bool createAgain = false);
	void UpdateCollider();

	void UpdateLocalMatrix();

	void UpdateTransformByRigidBody(physx::PxTransform* globalPos = nullptr);

	void DisplayComponentMenu();

public:
	colider_type type = colider_type::NONE;
	Mesh* mesh = nullptr;
	Mesh* current_mesh = nullptr;
	Mesh* dragged_mesh = nullptr;
	bool editCollider = false;
	bool updateValues = false;
	float3 centerPosition = float3::zero;
	float3 originalSize = float3::one;
	physx::PxRigidStatic* rigidStatic = nullptr;
	float3 offset = float3::zero;
	int colliderType = 0;
	bool hasBeenDeactivated = false;
	bool isTrigger = false;
	bool isConvex = false;
	Quat dragged_rot = Quat::identity;
	float3 dragged_scale = float3::one;
	int dragged_UID = 0;


private:
	physx::PxShape* shape = nullptr;
	physx::PxConvexMesh* convex_mesh = nullptr;
	physx::PxTriangleMesh* triangle_mesh = nullptr;
	float3 colliderSize = float3(1, 1, 1);
	float4x4 localMatrix = float4x4::identity;
	float4x4 globalMatrix = float4x4::identity;
	float radius = 1.0f, height = 1.0f;
	int lastIndex = -1;
	float3 tmpScale = float3::one;
	bool firstCreation = false;
	bool toPlay = false;
	bool draw = false;
	bool localMesh = false;
};

#endif //__COLLIDERCOMPONENT_H__

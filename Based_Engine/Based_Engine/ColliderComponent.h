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

private:

	template<class Geometry>
	void CreateRigidbody(Geometry geometry, physx::PxTransform position);
	template <class Geometry>
	bool HasDynamicRigidBody(Geometry geometry, physx::PxTransform transform);

public:

	colider_type type = colider_type::NONE;

	float3 centerPosition = float3::zero;
	float3 originalSize = float3::one;
	physx::PxRigidStatic* rigidStatic = nullptr;
	float3 offset = float3::zero;

	bool isTrigger = false;
	bool isConvex = false;

private:

	physx::PxShape* shape = nullptr;
	physx::PxConvexMesh* convex_mesh = nullptr;
	physx::PxTriangleMesh* triangle_mesh = nullptr;
	float3 colliderSize = float3(1, 1, 1);

	float radius = 1.0f, height = 1.0f;

	bool draw = false;
	bool localMesh = false;
};

#endif //__COLLIDERCOMPONENT_H__


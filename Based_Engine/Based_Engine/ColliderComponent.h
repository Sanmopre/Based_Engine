#ifndef __COLLIDERCOMPONENT_H__
#define __COLLIDERCOMPONENT_H__

#include "Component.h"

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

class ColliderComponent : public Component
{
public:

	ColliderComponent(char* name,colider_type type, GameObject* parent, Application* app, bool active = true);
	virtual ~ColliderComponent();

	bool Update(float dt);
	void Enable();
	void Disable();

	void UpdateCollider();

	void UpdateLocalMatrix();

	void UpdateTransformByRigidBody(physx::PxTransform* globalPos = nullptr);

	void DisplayComponentMenu();

private:

	bool isTrigger;
	colider_type type;
};

#endif //__COLLIDERCOMPONENT_H__

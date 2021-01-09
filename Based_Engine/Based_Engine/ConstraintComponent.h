#ifndef __CONSTRAINTCOMPONENT_H__
#define __CONSTRAINTCOMPONENT_H__

#include "Component.h"
#include "PxRigidActor.h"
#include "MathGeoLib.h"
#include "PxPhysicsAPI.h"

using namespace physx;

class ConstraintComponent : public Component
{
public:

	ConstraintComponent(char* name, GameObject* parent, Application* app, bool active = true);
	virtual ~ConstraintComponent();

	bool Update(float dt);
	void DisplayComponentMenu();

	void CreateConstraint(PxRigidActor* actor0, const PxTransform& localFrame0, PxRigidActor* actor1, const PxTransform& localFrame1);

private:

	PxRevoluteJoint* constraint = nullptr;
};

#endif //__CONSTRAINTCOMPONENT_H__
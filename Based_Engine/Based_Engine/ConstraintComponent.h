#ifndef __CONSTRAINTCOMPONENT_H__
#define __CONSTRAINTCOMPONENT_H__

#include "Component.h"
#include "MathGeoLib.h"

enum class JointType
{
	SINGLE
};

namespace physx
{
	class PxRigidActor;
	class PxTransform;
	class PxRevoluteJoint;
}

class ConstraintComponent : public Component
{
public:

	ConstraintComponent(char* name, JointType type, GameObject* parent, Application* app, bool active = true);
	virtual ~ConstraintComponent();

	bool Update(float dt);
	void DisplayComponentMenu();

	void CreateConstraint(const physx::PxTransform& localFrame);

private:

	JointType type = JointType::SINGLE;
	physx::PxRevoluteJoint* constraint = nullptr;

	float3 position = float3::zero;
};

#endif //__CONSTRAINTCOMPONENT_H__
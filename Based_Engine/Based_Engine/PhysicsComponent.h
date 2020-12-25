#ifndef __PHYSICSCOMPONENT_H__
#define __PHYSICSCOMPONENT_H__

#include "Component.h"
#include "PxRigidActor.h"


enum class State {
	STATIC,
	KINEMATIC,
	NONE
};
class PhysicsComponent : public Component
{
public:

	PhysicsComponent(char* name , GameObject* parent, Application* app, bool active = true);
	virtual ~PhysicsComponent();

	bool Update(float dt);

	void DisplayComponentMenu();

private:

	physx::PxRigidActor* ra = nullptr;

	bool isTrigger;
	bool freezedRotation;

	float mass;
	State state = State::NONE;
};

#endif //__PHYSICSCOMPONENT_H__
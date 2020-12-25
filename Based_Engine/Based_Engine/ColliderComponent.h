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


class ColliderComponent : public Component
{
public:

	ColliderComponent(char* name,colider_type type, GameObject* parent, Application* app, bool active = true);
	virtual ~ColliderComponent();

	bool Update(float dt);

	void DisplayComponentMenu();

private:

	bool isTrigger;
	colider_type type;
};

#endif //__COLLIDERCOMPONENT_H__

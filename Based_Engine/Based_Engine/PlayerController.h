#ifndef __PLAYERCONTROLLER_H__
#define __PLAYERCONTROLLER_H__

#include "Component.h"

class PlayerController : public Component
{
public:

	PlayerController(char* name, GameObject* parent, Application* app, bool active = true);
	virtual ~PlayerController();

	bool Update(float dt);

	void DisplayComponentMenu();

private:

	float maxVelocity = 20;
	float acceleration = 500;
	float deceleration = 100;
};

#endif //__PLAYERCONTROLLER_H__

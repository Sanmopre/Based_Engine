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

	void SetAccAndDec(float acceleration, float deceleration);

private:

	float maxVelocity = 20;
	float acceleration = 100;
	float deceleration = 50;
};

#endif //__PLAYERCONTROLLER_H__

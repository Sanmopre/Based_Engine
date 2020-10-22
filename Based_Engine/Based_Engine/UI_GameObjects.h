#pragma once
#include "UI.h"

class GameObjects : public UI
{
public:
	GameObjects(bool isActive, Application* application);
	virtual ~GameObjects();

	void Update(float dt);

};
#pragma once
#include "UI.h"

class GameObject;

class GameObjects : public UI
{
public:

	GameObjects(bool isActive, Application* application);
	virtual ~GameObjects();

	void Update(float dt);


	void IterateGameObjects(GameObject*);

	int i;

private:

	std::string create_name;
};
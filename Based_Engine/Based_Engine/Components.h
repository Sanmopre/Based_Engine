#pragma once

#include "Globals.h"

class Application;
class Mesh;
class GameObject;

class Component
{
public:

	Component(GameObject* parent, Application* app, bool active = true);
	virtual ~Component();

	virtual bool Update(float dt);

	virtual void Activate();
	virtual void Deactivate();
	bool IsActive();

	virtual void DisplayComponentMenu() = 0;

protected:

	bool toActivate;

	Application* App;
	bool active;

	GameObject* parent;
};

class MeshComponent : public Component
{
public:

	MeshComponent(const char* path, GameObject* parent, Application* app, bool active = true);
	virtual ~MeshComponent();

	bool Update(float dt);

	void Activate();
	void Deactivate();

	void DisplayComponentMenu();

private:

	MESH mesh;
};

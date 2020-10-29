#pragma once

#include "Globals.h"

class Application;
class Mesh;

class Component
{
public:

	Component(Application* app, bool active = true);
	virtual ~Component();

	virtual bool Update(float dt);

	virtual void Activate();
	virtual void Deactivate();
	bool IsActive();

protected:

	bool active;
	Application* App;
};

class MeshComponent : public Component
{
public:

	MeshComponent(const char* path, Application* app, bool active = true);
	virtual ~MeshComponent();

	bool Update(float dt);

	void Activate();
	void Deactivate();

private:

	MESH mesh;
};

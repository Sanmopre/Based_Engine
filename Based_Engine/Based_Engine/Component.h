#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "Globals.h"

class Application;
class GameObject;

class Component
{
public:

	Component(char* name, GameObject* parent, Application* app, bool active = true);
	virtual ~Component();

	virtual bool Update(float dt);

	virtual void Activate();
	virtual void Deactivate();
	bool IsActive();

	virtual void DisplayComponentMenu() = 0;

	virtual bool AddTexture(const char* path) { return false; }

	bool to_delete;


	std::string name;


protected:

	bool to_activate;

	Application* App;
	bool active;

	std::string name_buffer;

	GameObject* parent;
};

#endif //__COMPONENT_H__
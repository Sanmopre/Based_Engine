#pragma once
#include "Globals.h"

class Application;

class UI
{
public:

	UI(char* ui_name, bool is_enabled,Application* application);
	virtual ~UI();

	virtual void Start();
	virtual void PreUpdate();
	virtual void Update(float dt);
	virtual void PostUpdate();
	char* GetName();
	virtual void CleanUp();

	bool docked;
	bool enabled;

	Application* App;

private:

	char* name;

};
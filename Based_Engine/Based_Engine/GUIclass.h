#pragma once
#include "Globals.h"

#include <vector>

class Application;

class GUI
{
public:
	GUI(char* ui_name, bool is_enabled,Application* application);
	virtual ~GUI();

	virtual void Start();
	virtual void PreUpdate();
	virtual void Update(float dt);
	virtual void PostUpdate();
	char* GetName();
	virtual void CleanUp();

public:	

	bool docked;
	bool enabled;
	Application* App;
private:
	char* name;

};
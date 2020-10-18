#pragma once
#include "Module.h"
#include "SDL.h"
#include "Application.h"

#include<vector>
class GUI;

class Module_UI : public Module
{
public:

	Module_UI(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~Module_UI();
	update_status PreUpdate();
	update_status Update(float dt);
	update_status PostUpdate();
	bool CleanUp();

public:

	std::vector<GUI*> gui;

};

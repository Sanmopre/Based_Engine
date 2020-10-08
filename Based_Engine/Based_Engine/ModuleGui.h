#pragma once
#include "Module.h"

#include <string>

class ModuleGui : public Module
{
public:
	ModuleGui(Application* app, bool start_enabled = true);
	~ModuleGui();

	bool Start();
	update_status Update(float dt);
	update_status PostUpdate();
	bool CleanUp();
private:

	bool showcase = false;
	bool active_window = true;

	int brightness = 50;
	int height = 10;
	int width = 10;

	DEVMODEA devmode;

	std::string app_name;
};

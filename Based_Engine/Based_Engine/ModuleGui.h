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
	int GetWindowRefresh();

private:

	bool showcase = false;
	bool active_window = true;

	float brightness = 1.0f;
	int height = 10;
	int width = 10;

	//window checkboxes
	bool fullscreen = false;
	bool resizable = true;
	bool borderless = false;
	bool full_desktop = false;

	DEVMODEA devmode;
	std::string app_name;
};

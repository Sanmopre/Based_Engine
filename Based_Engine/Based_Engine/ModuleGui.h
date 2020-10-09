#pragma once
#include "Module.h"

#include <string>
#include <vector>

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

	bool showcase;
	bool active_window;

	//window stats
	float brightness;
	int height;
	int width;
	int mon_height;
	int mon_width;
	int fps_cap;
	std::vector<int> fps_log;

	//window checkboxes
	bool fullscreen;
	bool resizable;
	bool borderless;
	bool full_desktop;


	SDL_version compiled;
	DEVMODEA devmode;
	std::string app_name;
	std::string organization;
};

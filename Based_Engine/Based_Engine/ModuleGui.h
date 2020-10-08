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

	std::string app_name;
};

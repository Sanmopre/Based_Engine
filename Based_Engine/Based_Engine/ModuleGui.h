#pragma once
#include "Module.h"


class ModuleGui : public Module
{
public:
	ModuleGui(Application* app, bool start_enabled = true);
	~ModuleGui();

	bool Start();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
private:
	bool showcase = false;
};

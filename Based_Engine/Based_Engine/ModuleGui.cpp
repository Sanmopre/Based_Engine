#include "Globals.h"
#include "Application.h"
#include "ModuleGui.h"

ModuleGui::ModuleGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleGui::~ModuleGui()
{}

// Load assets
bool ModuleGui::Start()
{
	bool ret = true;

	return ret;
}

// Update: draw background
update_status ModuleGui::Update()
{
	return UPDATE_CONTINUE;
}

// Load assets
bool ModuleGui::CleanUp()
{
	return true;
}
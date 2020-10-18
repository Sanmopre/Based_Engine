#include "Globals.h"
#include "Application.h"
#include "Module_UI.h"
#include "GUIclass.h"

#include "UI_MainMenu.h"

Module_UI::Module_UI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

Module_UI::~Module_UI()
{
}

bool Module_UI::Start()
{
	gui.push_back(mainmenu = new MainMenu(true));
	return true;
}

update_status Module_UI::PreUpdate()
{
	for (uint i = 0; i < gui.size(); i++)
	{
		if (gui[i]->enabled == true)
		{
			gui[i]->PreUpdate();
		}
	}
	return UPDATE_CONTINUE;
}

update_status Module_UI::Update(float dt)
{

	for (uint i = 0; i < gui.size(); i++)
	{
		if (gui[i]->enabled == true)
		{
			gui[i]->Update();
		}
	}

	return UPDATE_CONTINUE;
}

update_status Module_UI::PostUpdate()
{
	for (uint i = 0; i < gui.size(); i++)
	{
		if (gui[i]->enabled == true)
		{
			gui[i]->PostUpdate();
		}
	}


	return UPDATE_CONTINUE;
}

bool Module_UI::CleanUp()
{
	for (uint i = 0; i < gui.size(); i++)
	{
		if (gui[i]->enabled == true)
		{
			gui[i]->CleanUp();
		}
	}
	return true;
}

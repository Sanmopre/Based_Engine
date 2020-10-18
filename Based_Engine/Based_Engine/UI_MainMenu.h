#pragma once

#include "imgui.h"
#include "GUIclass.h"
#include "Module.h"

class MainMenu : public GUI
{
public:
	MainMenu(bool isActive);
	virtual ~MainMenu();

	virtual void Update();

};

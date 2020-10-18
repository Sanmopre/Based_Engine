#include "GUIclass.h"

GUI::GUI(char* ui_name, bool is_enabled, Application* application)
{
	name = ui_name;
	enabled = is_enabled;
	App = application;
}

GUI::~GUI()
{
}

void GUI::Start()
{
}

void GUI::PreUpdate()
{
}

void GUI::Update(float dt)
{
}

void GUI::PostUpdate()
{
}

char* GUI::GetName()
{
	return name;
}

void GUI::CleanUp()
{
}

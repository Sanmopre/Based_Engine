#include "GUIclass.h"

GUI::GUI(char* ui_name, bool is_enabled)
{
	name = ui_name;
	enabled = is_enabled;
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

void GUI::Update()
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

#include "UI.h"

UI::UI(char* ui_name, bool is_enabled, Application* application)
{
	name = ui_name;
	enabled = is_enabled;
	App = application;
}

UI::~UI()
{
}

void UI::Start()
{
}

void UI::PreUpdate()
{
}

void UI::Update(float dt)
{
}

void UI::PostUpdate()
{
}

char* UI::GetName()
{
	return name;
}

void UI::CleanUp()
{
}

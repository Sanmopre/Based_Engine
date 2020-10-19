#include "UI_Console.h"
#include "Globals.h"
#include "Application.h"
#include "Module.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "SDL_opengl.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h" 
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "wtypes.h"
#include <string> 

Console::Console(bool isActive, Application* application) : GUI("Console", isActive, App)
{
	App = application;
}

Console::~Console()
{
}

void Console::Update(float dt)
{
	if (ImGui::Begin("Console"))
	{
		std::vector<std::string> logs = GetLogs();
		for (std::vector<std::string>::iterator l = logs.begin(); l != logs.end(); l++)
		{
			ImGui::Text((*l).c_str());
		}
		if (ImGui::InputText("", &inputlog, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			LOG("%s", inputlog.c_str());
			if (*inputlog.begin() == '/')
			{
				inputlog.erase(inputlog.begin());

			}
			inputlog = "";
		}
	}
	ImGui::End();
}

#include "UI_Console.h"

#include "Application.h"
#include "Window.h"
#include "Renderer3D.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "misc/cpp/imgui_stdlib.h" 

#include "wtypes.h"

Console::Console(bool isActive, Application* application) : UI("Console", isActive, App)
{
	App = application;
	inputlog = "";
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
		if (ImGui::InputText(" ", &inputlog, ImGuiInputTextFlags_EnterReturnsTrue))
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

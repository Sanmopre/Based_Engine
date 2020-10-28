#include "UI_Topbar.h"
#include "Application.h"
#include "Window.h"
#include "Renderer3D.h"
#include "Input.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "wtypes.h"


Topbar::Topbar(bool isActive, Application* application) : UI("Console", isActive, App)
{
	App = application;
}

Topbar::~Topbar()
{
}

void Topbar::Start()
{
	showcase = false;
}

void Topbar::Update(float dt)
{
	bool show_demo_window;


	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("Files")) {
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("Show ImGui demo"))
			showcase = !showcase;
		if (ImGui::MenuItem("Documentation"))
			ShellExecute(NULL, "open", "www.google.com", NULL, NULL, SW_SHOWNORMAL);

		if (ImGui::MenuItem("Download latest"))
			ShellExecute(NULL, "open", "www.google.com", NULL, NULL, SW_SHOWNORMAL);

		if (ImGui::MenuItem("Report a bug"))
			ShellExecute(NULL, "open", "www.google.com", NULL, NULL, SW_SHOWNORMAL);

		ImGui::ShowStyleSelector("Style selector");

		if (ImGui::CollapsingHeader("About"))
		{
			ImGui::Text("BASED engine.");
			ImGui::Text("The future of 3D Graphics by Santiago Moliner and David Rami.");
			ImGui::Text("Source code:    https://github.com/Sanmopre/Based_Engine");
			ImGui::NewLine();
			ImGui::Text("3rd party libraries used: ");
			ImGui::Text(" - SDL");
			ImGui::Text(" - OpenGL");
			ImGui::Text(" - Glew");
			ImGui::Text(" - JSON parser");
			ImGui::Text(" - MathGeoLib");
		}
		ImGui::EndMenu();
	}

	if (ImGui::Button("Quit", ImVec2(40, 20))) 
	{
		App->input->quit = true;
	}
	ImGui::EndMainMenuBar();

	if (showcase)
	{
		ImGui::ShowDemoWindow(&show_demo_window);
		if (!show_demo_window)
			showcase = false;
	}

}

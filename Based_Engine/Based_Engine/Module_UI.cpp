#include "Globals.h"
#include "Application.h"
#include "Module_UI.h"
#include "GUIclass.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"


#include "UI_MainMenu.h"
#include "UI_Console.h"
#include "UI_Topbar.h"
#include "UI_Scene.h"

Module_UI::Module_UI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

Module_UI::~Module_UI()
{
}

bool Module_UI::Start()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->window->gl_context);
	ImGui_ImplOpenGL3_Init("#version 130");


	gui.push_back(mainmenu = new MainMenu(true,App));
	gui.push_back(console = new Console(true, App));
	gui.push_back(topbar = new Topbar(true, App));
	gui.push_back(scene = new Scene(true, App));

	for (uint i = 0; i < gui.size(); i++)
	{
		if (gui[i]->enabled == true)
		{
			gui[i]->Start();
		}
	}

	return true;
}

update_status Module_UI::PreUpdate()
{
	//NEW GUI FRAME
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	//CreateDocking();


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
			gui[i]->Update(dt);
		}
	}

	//UI RENDERING
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();

	return true;
}

void Module_UI::CreateDocking()
{
	static bool optFullscreen = true;
	static bool optPadding = false;
	static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (optFullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspaceFlags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
		windowFlags |= ImGuiWindowFlags_NoBackground;

	if (!optPadding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("DockSpace Demo", NULL, windowFlags);
	if (!optPadding)
		ImGui::PopStyleVar();

	if (optFullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
	}
	else
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("ERROR: Docking is not enabled! See Demo > Configuration.");
		ImGui::Text("Set io.ConfigFlags |= ImGuiConfigFlags_DockingEnable in your code, or ");
		ImGui::SameLine(0.0f, 0.0f);
		if (ImGui::SmallButton("click here"))
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	}

	ImGui::End();
}

void Module_UI::SceneSize(float& width, float& height)
{

}

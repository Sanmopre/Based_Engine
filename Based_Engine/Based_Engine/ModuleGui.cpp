#include "Globals.h"
#include "Application.h"
#include "ModuleGui.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

ModuleGui::ModuleGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleGui::~ModuleGui()
{}

// Load assets
bool ModuleGui::Start()
{
	bool ret = true;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsClassic();
	//ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->window->gl_context);
	ImGui_ImplOpenGL3_Init("#version 130");


	return ret;
}

// Update: draw background
update_status ModuleGui::Update()
{	

	bool show_demo_window = true;

	//create new ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();


	//Main Menu
	ImGui::Begin("BASED Engine");
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

		if (ImGui::MenuItem("About"))
		ShellExecute(NULL, "open", "www.google.com", NULL, NULL, SW_SHOWNORMAL);

		ImGui::EndMenu();
	}
	ImGui::End();

	//demo window
	if (showcase)
		ImGui::ShowDemoWindow(&show_demo_window);


	//UI rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	
	
	return UPDATE_CONTINUE;
}

update_status ModuleGui::PostUpdate()
{

	return UPDATE_CONTINUE;
}


// Load assets
bool ModuleGui::CleanUp()
{
	return true;
}
#include "Globals.h"
#include "Application.h"
#include "ModuleGui.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h" //ENABLE THE INPUT TEXT FUNCTIONS WITH STD::STRING
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include <string> 

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
	//ImGui::StyleColorsClassic();
	//ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();


	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->window->gl_context);
	ImGui_ImplOpenGL3_Init("#version 130");

	app_name = "";

	return ret;
}

// Update: draw background
update_status ModuleGui::Update(float dt)
{	


	bool show_demo_window = true;
	
	//create new ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();


	//TOP BAR MENU
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("Files")){
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

		if (ImGui::MenuItem("About"))
		ShellExecute(NULL, "open", "www.google.com", NULL, NULL, SW_SHOWNORMAL);

		ImGui::ShowStyleSelector("Style selector");

		ImGui::EndMenu();
	}

	if (ImGui::Button("Quit", ImVec2(40, 20))) {
		return UPDATE_STOP;
	}

	ImGui::EndMainMenuBar();

	ImGuiID id = 1;
	//Main window
	if (ImGui::Begin("BASED Engine"))
	{
		if (ImGui::CollapsingHeader("Application"))
		{
			ImGui::InputText("App name", &app_name);
		}

		if (ImGui::CollapsingHeader("Window"))
		{			


			//Window refresh			
			ImGui::Text("Refresh rate:");
			ImGui::SameLine();
			char refresh[5];
			sprintf(refresh, "%d", GetWindowRefresh());
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), refresh);

			//Brightness
			ImGui::SliderFloat("Brightness",&brightness,0.0f,1.0f);

			//Window size
			ImGui::NewLine();
			ImGui::InputInt("Width", &width, 1, 2000);
			ImGui::InputInt("Height", &height, 1, 2000);
			ImGui::Button("Apply", ImVec2(50, 25));

			//Window checkboxes
			ImGui::NewLine();
			ImGui::Checkbox("Fullscreen",&fullscreen);
			ImGui::SameLine();
			ImGui::Checkbox("Resizable", &resizable);
			ImGui::Checkbox("Borderless",&borderless );
			ImGui::SameLine();
			ImGui::Checkbox("Full Desktop", &full_desktop);
		}

		if (ImGui::CollapsingHeader("File System"))
		{
		}

		if (ImGui::CollapsingHeader("Input"))
		{
		}

		if (ImGui::CollapsingHeader("Hardware"))
		{

		}

	}
	ImGui::End();

	//demo window
	if (showcase)
	{
		ImGui::ShowDemoWindow(&show_demo_window);
		if (!show_demo_window)
			showcase = false;
	}
		
	//UI rendering	
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	

	//Window brightness
	App->window->WindowBrightness(brightness);

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

int ModuleGui::GetWindowRefresh()
{
	EnumDisplaySettingsA(NULL, 0, &devmode);
	int frequency = devmode.dmDisplayFrequency;
	return frequency;
}

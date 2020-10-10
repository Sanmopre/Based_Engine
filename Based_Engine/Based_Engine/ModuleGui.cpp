#include "Globals.h"
#include "Application.h"
#include "ModuleGui.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h" //ENABLE THE INPUT TEXT FUNCTIONS WITH STD::STRING
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "wtypes.h"
#include <string> 

ModuleGui::ModuleGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleGui::~ModuleGui()
{}

// Load assets
bool ModuleGui::Start()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->window->gl_context);
	ImGui_ImplOpenGL3_Init("#version 130");

	//Initialize variables
	height = SCREEN_HEIGHT * SCREEN_SIZE;
	width = SCREEN_WIDTH * SCREEN_SIZE;
	App->window->GetDesktopResolution(mon_width, mon_height);
	fullscreen = false;
	resizable = true;
	borderless = false;
	full_desktop = false;
	showcase = false;
	active_window = true;
	brightness = 1.0f;
	fps_cap = 60;
	app_name = "BASED Engine";
	organization = "UPC CITM";


	return true;
}

// Update: draw background
update_status ModuleGui::Update(float dt)
{	

	MoveOne(fps, HISTOGRAM_SIZE);
	fps[HISTOGRAM_SIZE - 1] = 1/dt;

	MoveOne(dt_log, HISTOGRAM_SIZE);
	dt_log[HISTOGRAM_SIZE - 1] = dt;
	
	bool show_demo_window;
	

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
			ImGui::InputText("Organization", &organization);
			ImGui::InputInt("Framerate cap", &fps_cap, 1, 240);

			//Histograms
			ImGui::NewLine();
			char title[30] = "";
			sprintf_s(title, 25, "Framerate: %.1f", fps[HISTOGRAM_SIZE - 1]);
			ImGui::PlotHistogram("FPS", fps, IM_ARRAYSIZE(fps), 0, title, 0.0f, 100.0f, ImVec2(300, 90.0f));

			ImGui::NewLine();
			char title_dt[30] = "";
			sprintf_s(title_dt, 25, "Delta time: %f", dt_log[HISTOGRAM_SIZE - 1]);
			ImGui::PlotHistogram("Delta time", dt_log, IM_ARRAYSIZE(dt_log), 0, title_dt, 0.0f, 0.05f, ImVec2(300, 90.0f));
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
			if (ImGui::Button("Apply", ImVec2(50, 25))) 
			{
				App->window->WindowResize(width, height);
			}

			//Window checkboxes
			ImGui::NewLine();
			ImGui::Checkbox("Fullscreen", &fullscreen);
			ImGui::SameLine();
			ImGui::Checkbox("Resizable", &resizable);
			ImGui::Checkbox("Borderless",&borderless );
			ImGui::SameLine();
			ImGui::Checkbox("Full Desktop", &full_desktop);
		}

		if (ImGui::CollapsingHeader("File System"))
		{
			ImGui::Text("Base Path:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "(C:)Pog/Pog/PogU");
		}

		if (ImGui::CollapsingHeader("Input"))
		{
			ImGuiIO& io = ImGui::GetIO();
			// Display ImGuiIO output flags
			ImGui::Text("WantCaptureMouse: %d", io.WantCaptureMouse);
			ImGui::Text("WantCaptureKeyboard: %d", io.WantCaptureKeyboard);
			ImGui::Text("WantTextInput: %d", io.WantTextInput);
			ImGui::Text("WantSetMousePos: %d", io.WantSetMousePos);
			ImGui::Text("NavActive: %d, NavVisible: %d", io.NavActive, io.NavVisible);

			// Display Keyboard/Mouse state
			if (ImGui::TreeNode("Keyboard, Mouse & Navigation State"))
			{
				if (ImGui::IsMousePosValid())
					ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
				else
					ImGui::Text("Mouse pos: <INVALID>");
				ImGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
				ImGui::Text("Mouse down:");     for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (io.MouseDownDuration[i] >= 0.0f) { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
				ImGui::Text("Mouse clicked:");  for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseClicked(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
				ImGui::Text("Mouse dblclick:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseDoubleClicked(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
				ImGui::Text("Mouse released:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseReleased(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
				ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);

				ImGui::Text("Keys down:");      for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (io.KeysDownDuration[i] >= 0.0f) { ImGui::SameLine(); ImGui::Text("%d (0x%X) (%.02f secs)", i, i, io.KeysDownDuration[i]); }
				ImGui::Text("Keys pressed:");   for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyPressed(i)) { ImGui::SameLine(); ImGui::Text("%d (0x%X)", i, i); }
				ImGui::Text("Keys release:");   for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyReleased(i)) { ImGui::SameLine(); ImGui::Text("%d (0x%X)", i, i); }
				ImGui::Text("Keys mods: %s%s%s%s", io.KeyCtrl ? "CTRL " : "", io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "", io.KeySuper ? "SUPER " : "");
				ImGui::Text("Chars queue:");    for (int i = 0; i < io.InputQueueCharacters.Size; i++) { ImWchar c = io.InputQueueCharacters[i]; ImGui::SameLine();  ImGui::Text("\'%c\' (0x%04X)", (c > ' ' && c <= 255) ? (char)c : '?', c); } // FIXME: We should convert 'c' to UTF-8 here but the functions are not public.

				ImGui::Text("NavInputs down:");     for (int i = 0; i < IM_ARRAYSIZE(io.NavInputs); i++) if (io.NavInputs[i] > 0.0f) { ImGui::SameLine(); ImGui::Text("[%d] %.2f", i, io.NavInputs[i]); }
				ImGui::Text("NavInputs pressed:");  for (int i = 0; i < IM_ARRAYSIZE(io.NavInputs); i++) if (io.NavInputsDownDuration[i] == 0.0f) { ImGui::SameLine(); ImGui::Text("[%d]", i); }
				ImGui::Text("NavInputs duration:"); for (int i = 0; i < IM_ARRAYSIZE(io.NavInputs); i++) if (io.NavInputsDownDuration[i] >= 0.0f) { ImGui::SameLine(); ImGui::Text("[%d] %.2f", i, io.NavInputsDownDuration[i]); }



				ImGui::TreePop();
			}
		}

		if (ImGui::CollapsingHeader("Hardware"))
		{
			SDL_VERSION(&compiled);
			ImGui::Text("SDL Version:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "2.0.12");

			char cpu[5];
			sprintf(cpu, "%d", SDL_GetCPUCount());
			ImGui::Text("CPU cores:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), cpu);

			char cpucache[5];
			sprintf(cpucache, "%d", SDL_GetCPUCacheLineSize());
			ImGui::Text("CPU cache line size:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), cpucache);
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "kb");

			char ram[20];
			sprintf(ram, "%d", SDL_GetSystemRAM()/1000);
			ImGui::Text("RAM:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), ram);
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "GB");

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
	

	return UPDATE_CONTINUE;
}

update_status ModuleGui::PostUpdate()
{

	if (full_desktop) 
	{
		App->window->WindowResize(mon_width, mon_height);
		fullscreen = true;
	}

	//Manage window stats
	if (fullscreen) 
	{
		App->window->WindowSetFullscreen();
	}
	else 
	{
		App->window->WindowSetWindowed();
	}

	//Window brightness
	App->window->WindowBrightness(brightness);

	//Window resize
	App->window->WindowResizable(resizable);

	App->window->WindowBorderless(!borderless);
	///////////////////



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

void ModuleGui::MoveOne(float* array, int size)
{
	for (int i = 0; i < size - 1; i++) 
	{
		array[i] = array[i + 1];
	}
}


#include "Application.h"

Application::Application()
{
	window = new Window(this);
	input = new Input(this);
	renderer3D = new Renderer3D(this);
	camera = new Camera3D(this);
	ui = new GUI(this);

	modules.reserve(10);

	// Main Modules
	AddModule(window);	
	AddModule(renderer3D);
	AddModule(camera);
	AddModule(input);
	AddModule(ui);

	//UI last
}

Application::~Application()
{
	while (modules.size() != 0)
	{
		delete *modules.begin();
		modules.erase(modules.begin());
	}
}

bool Application::Init()
{
	//Init
	for (std::vector<Module*>::iterator m = modules.begin(); m != modules.end(); m++)
		if (!(*m)->Init())
			return false;

	//Start
	for (std::vector<Module*>::iterator m = modules.begin(); m != modules.end(); m++)
		if (!(*m)->Start())
			return false;
	
	return true;
}


// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	starting_tick = SDL_GetTicks();
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	//PreUpdate
	for (std::vector<Module*>::iterator m = modules.begin(); m != modules.end(); m++)
	{
		ret = (*m)->PreUpdate();
		if (ret != UPDATE_CONTINUE)
			return ret;
	}

	//Update
	for (std::vector<Module*>::iterator m = modules.begin(); m != modules.end(); m++)
	{
		ret = (*m)->Update(dt);
		if (ret != UPDATE_CONTINUE)
			return ret;
	}

	//PostUpdate
	for (std::vector<Module*>::iterator m = modules.begin(); m != modules.end(); m++)
	{
		ret = (*m)->PostUpdate();
		if (ret != UPDATE_CONTINUE)
			return ret;
	}

	FinishUpdate();

	if ((1000 / FPS) > SDL_GetTicks() - starting_tick) {
		SDL_Delay(1000/FPS - (SDL_GetTicks() - starting_tick));
	}

	return ret;
}

bool Application::CleanUp()
{
	//CleanUp
	for (std::vector<Module*>::iterator m = modules.end() - 1; m != modules.begin(); m--)
		if (!(*m)->CleanUp())
			return false;

	return true;
}

void Application::AddModule(Module* module)
{
	modules.emplace_back(module);
}
#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(0.0f, 2.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	plane = new Plane(0, 1, 0, 0);
	plane->axis = true;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	delete plane;

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{

	plane->Render();

	return UPDATE_CONTINUE;
}


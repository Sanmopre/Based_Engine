#pragma once
                       
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "Window.h"
#include "Input.h"
#include "Renderer3D.h"
#include "Camera3D.h"
#include "GUI.h"

#include <vector>

class Application
{
public:
	Window* window;
	Input* input;
	Renderer3D* renderer3D;
	Camera3D* camera;
	GUI* ui;

private:

	std::vector<Module*> modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	int FPS = 60;
private:

	Timer	ms_timer;
	float	dt;
	Uint32 starting_tick;

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};
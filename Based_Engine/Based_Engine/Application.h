#pragma once           
#include "Globals.h"
#include "Timer.h"

class Module;
class Camera3D;
class GUI;
class Input;
class Renderer3D;
class Window;
class ObjectManager;
class ResourceManager;
class PhysicsEngine;

class Application
{
public:
	Window* window = nullptr;
	Input* input = nullptr;
	Renderer3D* renderer3D = nullptr;
	Camera3D* camera = nullptr;
	GUI* ui = nullptr;
	ObjectManager* objects = nullptr;
	ResourceManager* resources = nullptr;
	PhysicsEngine* physics = nullptr;

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
#pragma once
#include "Module.h"
#include "SDL.h"
#include "Application.h"

#include<vector>
class GUI;
class MainMenu;
class Console;
class Topbar;
class Scene;

class Module_UI : public Module
{
public:

	Module_UI(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~Module_UI();
	bool Start();
	update_status PreUpdate();
	update_status Update(float dt);
	update_status PostUpdate();
	bool CleanUp();
	void CreateDocking();
	void SceneSize(float& width, float& height);

public:
	
	std::vector<GUI*> gui;

	MainMenu* mainmenu;
	Console* console;
	Topbar* topbar;
	Scene* scene;
};

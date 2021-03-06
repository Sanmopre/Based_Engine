#pragma once
#include "Module.h"

class UI;
class MainMenu;
class Console;
class Topbar;
class Scene;
class GameObjects;
class GameObject_Inspector;
class Assets;

class GUI : public Module
{
public:

	GUI(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~GUI();
	bool Start();
	update_status PreUpdate();
	update_status Update(float dt);
	update_status PostUpdate();
	bool CleanUp();
	void CreateDocking();
	void SceneSize(float& width, float& height);

public:
	
	std::vector<UI*> gui;

	MainMenu* mainmenu;
	Console* console;
	Topbar* topbar;
	Scene* scene;
	GameObjects* game_objects;
	GameObject_Inspector* inspector;
	Assets* assets;
};

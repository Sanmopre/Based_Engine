#pragma once
#include "UI.h"

class MainMenu : public UI
{
public:
	MainMenu(bool isActive, Application* application);
	virtual ~MainMenu();

	void Start();
	void Update(float dt);
	void PostUpdate();
	int GetWindowRefresh();
	void MoveOne(float* array, int size);
	void ManageOpenGlCheckboxes();
	void UpdateWindowStats();

private:

	//window stats
	float brightness;
	int height;
	int width;
	int mon_height;
	int mon_width;
	int fps_cap;
	float fps[HISTOGRAM_SIZE];
	float dt_log[HISTOGRAM_SIZE];

	//OpenGl checkboxes
	bool depth;
	bool cull_face;
	bool lighting;
	bool color_material;
	bool texture2d;

	//window checkboxes
	bool fullscreen;
	bool resizable;
	bool borderless;
	bool full_desktop;


	SDL_version compiled;
	DEVMODEA devmode;
	std::string app_name;
	std::string organization;
};

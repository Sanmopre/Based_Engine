#include "UI_Scene.h"
#include "Globals.h"
#include "Application.h"
#include "Module.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "SDL_opengl.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h" 
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "wtypes.h"
#include <string> 

Scene::Scene(bool isActive, Application* application) : GUI("Console", isActive, App)
{
	App = application;
}

Scene::~Scene()
{
}

void Scene::Update(float dt)
{

}

void Scene::Cleanup()
{
}

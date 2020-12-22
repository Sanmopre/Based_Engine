#include "ColliderComponent.h"

#include "Application.h"
#include "GameObject.h"
#include "Transform.h"
#include "Input.h"

#include "GL/glew.h"

#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h" 

ColliderComponent::ColliderComponent(char* name, colider_type col_type, GameObject* parent, Application* app, bool active) : Component(name, parent, app, active)
{
	isTrigger = false;
	type = col_type;
}

ColliderComponent::~ColliderComponent()
{

}

bool ColliderComponent::Update(float dt)
{

	return true;
}


void ColliderComponent::DisplayComponentMenu()
{
}
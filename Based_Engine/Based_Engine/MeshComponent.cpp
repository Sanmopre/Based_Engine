#include "Components.h"

#include "Application.h"
#include "Renderer3D.h"
#include "Simp.h"
#include "Mesh.h"

#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

MeshComponent::MeshComponent(const char* path, GameObject* parent, Application* app, bool active) : Component(parent, app, active)
{
	mesh = Simp::LoadFile(path);

	if (active)
		App->renderer3D->AddMesh(&mesh);
}

MeshComponent::~MeshComponent()
{
}

bool MeshComponent::Update(float dt)
{
	if (active != toActivate)
	{
		if (toActivate)
			Activate();
		else
			Deactivate();
	}
	return true;
}

void MeshComponent::Activate()
{
	if (!active)
	{
		active = true;
		App->renderer3D->AddMesh(&mesh);
	}
}

void MeshComponent::Deactivate()
{
	if (active)
	{
		active = false;
		App->renderer3D->DeleteMesh(&mesh);
	}
}

void MeshComponent::DisplayComponentMenu()
{
	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{

	}
	ImGui::Checkbox("active", &toActivate);
}

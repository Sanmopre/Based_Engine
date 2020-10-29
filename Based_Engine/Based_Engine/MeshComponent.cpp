#include "Components.h"

#include "Application.h"
#include "Renderer3D.h"
#include "Simp.h"
#include "Mesh.h"


MeshComponent::MeshComponent(const char* path, Application* app, bool active) : Component(app, active)
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

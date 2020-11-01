#include "Components.h"

#include "Application.h"
#include "Renderer3D.h"
#include "Simp.h"
#include "Mesh.h"
#include "GameObject.h"

#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h" 

MeshComponent::MeshComponent(char* name, const char* path, GameObject* parent, Application* app, bool active) : Component(name, parent, app, active)
{
	mesh = Simp::LoadFile(path);
	mesh_buffer = mesh;

	this->path = path;
	path_buffer = path;

	if (active)
		App->renderer3D->AddMesh(&mesh_buffer);
}

MeshComponent::~MeshComponent()
{
	App->renderer3D->DeleteMesh(&mesh_buffer);
}

bool MeshComponent::Update(float dt)
{
	if (parent->last_transform != parent->transform)
	{
		mesh_buffer = mesh;
		for (uint m = 0; m < mesh_buffer.size(); m++)
		{
			mesh_buffer[m].UpdateScale(parent->transform.scale.x, parent->transform.scale.y, parent->transform.scale.z);
			mesh_buffer[m].UpdatePosition(parent->transform.position.x, parent->transform.position.y, parent->transform.position.z);
		}
	}

	if (active != to_activate)
	{
		if (to_activate)
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
		App->renderer3D->AddMesh(&mesh_buffer);
	}
}

void MeshComponent::Deactivate()
{
	if (active)
	{
		active = false;
		App->renderer3D->DeleteMesh(&mesh_buffer);
	}
}

void MeshComponent::DisplayComponentMenu()
{
	char str[24];
	sprintf_s(str, "Mesh Component (%s)", name.c_str());

	if (ImGui::CollapsingHeader(str, ImGuiTreeNodeFlags_DefaultOpen))
	{
		sprintf_s(str, "active (%s)", name.c_str());
		ImGui::Checkbox(str, &to_activate);

		ImGui::SameLine();
		sprintf_s(str, "delete (%s)", name.c_str());
		if(ImGui::Button(str))
			to_delete = true;

		sprintf_s(str, "name (%s)", name.c_str());
		if (ImGui::InputText(str, &name_buffer, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			bool same = false;
			for (uint i = 0; i < parent->components.size(); i++)
			{
				if (parent->components[i]->name == name_buffer)
				{
					same = true;
					break;
				}
			}
			if (!same)
				name = name_buffer;
			else
				name_buffer = name;
		}

		sprintf_s(str, "mesh (%s)", name.c_str());
		if (ImGui::InputText(str, &path_buffer, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			MESH m = Simp::LoadFile(path_buffer.c_str());

			if (m.size() != 0)
			{
				mesh = m;
				path = path_buffer;
			}
			else
				path_buffer = path;
		}
	}
}

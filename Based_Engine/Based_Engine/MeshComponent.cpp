#include "Components.h"

#include "Application.h"
#include "Renderer3D.h"
#include "Simp.h"
#include "Mesh.h"
#include "TextureLoader.h"
#include "GameObject.h"
#include "ObjectManager.h"
#include "Transform.h"
#include "Input.h"

#include "GL/glew.h"

#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h" 

MeshComponent::MeshComponent(char* name, const char* path, const char* texture_path, GameObject* parent, Application* app, bool active) : Component(name, parent, app, active)
{
	std::vector<Mesh> meshes = Simp::LoadMeshFile(path);

	if (meshes.size() == 0)
		return;

	mesh = *meshes.begin();
	if (meshes.size() > 1)
	{
		for (int i = 1; i < meshes.size(); i++)
		{
			char n[64];
			memset(n, 0, 64);
			sprintf(n, "%s%d", parent->name.c_str(), i);
			GameObject* obj = App->objects->AddObject(n, parent);
			obj->AddMeshComponent(meshes[i], texture_path);
		}
	}

	this->path = path;
	path_buffer = path;

	to_draw_normals = false;
	to_draw_AABB = false;

	texture = NULL;
	AddTexture(texture_path);

	this->parent->transform->Reparent(mesh.transform);

	if (active)
		App->renderer3D->AddMesh(&mesh);
}

MeshComponent::MeshComponent(char* name, Mesh mesh, const char* texture_path, GameObject* parent, Application* app, bool active) : Component(name, parent, app, active)
{

	this->mesh = mesh;

	this->path = path;
	path_buffer = path;

	to_draw_normals = false;

	texture = NULL;
	AddTexture(texture_path);


	this->parent->transform->Reparent(mesh.transform);

	if (active)
		App->renderer3D->AddMesh(&this->mesh);
}

MeshComponent::~MeshComponent()
{
	App->renderer3D->DeleteMesh(&mesh);
}

bool MeshComponent::Update(float dt)
{	
	//TEMP//
	mesh.transform = this->parent->transform->global_transformation;
	this->parent->transform->RecalculateTransform();
	//mesh.RecalculateAABB_OBB(this->parent->transform->local_transformation);
	////////

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
	char str[24];
	sprintf_s(str, "Mesh Component (%s)", name.c_str());

	if (ImGui::CollapsingHeader(str, ImGuiTreeNodeFlags_DefaultOpen))
	{
		sprintf_s(str, "active (%s)", name.c_str());
		ImGui::Checkbox(str, &to_activate);
		ImGui::SameLine();

		sprintf_s(str, "normals (%s)", name.c_str());
		if(ImGui::Checkbox(str, &to_draw_normals))
			mesh.drawnormals = to_draw_normals;
		ImGui::SameLine();

		sprintf_s(str, "AABB (%s)", name.c_str());
		if (ImGui::Checkbox(str, &to_draw_AABB))
			mesh.show_bounding_box = to_draw_AABB;


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

		//sprintf_s(str, "mesh (%s)", name.c_str());
		//if (ImGui::InputText(str, &path_buffer, ImGuiInputTextFlags_EnterReturnsTrue))
		//{
		//	MESH m = Simp::LoadFile(path_buffer.c_str());
		//
		//	if (m.size() != 0)
		//	{
		//		mesh = m;
		//		path = path_buffer;
		//	}
		//	else
		//		path_buffer = path;
		//}
		sprintf_s(str, "texture (%s)", name.c_str());
		if (ImGui::InputText(str, &text_path_buffer, ImGuiInputTextFlags_EnterReturnsTrue))
			if (text_path != text_path_buffer)
			{
				bool valid = AddTexture(text_path_buffer.c_str());

				if (valid)
					text_path = text_path_buffer;
				else
					text_path = text_path_buffer = "";
			}

		if (texture != NULL)
		{
			ImTextureID my_tex_id = (ImTextureID)(GLuint)texture;
			float tex_w = 300;
			float tex_h = 300;

			ImGui::Image(my_tex_id, ImVec2(tex_w, tex_h));
		}
	}
}

bool MeshComponent::AddTexture(const char* path)
{
	if (path)
	{
		this->text_path = path;
		text_path_buffer = path;

		texture = TextureLoader::Load(path).id;
		PushTexture(texture);

		if (texture == NULL)
			return false;
	}

	return true;
}

void MeshComponent::PushTexture(uint texture)
{
	mesh.tex_id = texture;
}

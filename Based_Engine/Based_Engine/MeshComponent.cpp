#include "MeshComponent.h"

#include "Application.h"
#include "Renderer3D.h"
#include "Simp.h"
#include "TextureLoader.h"
#include "GameObject.h"
#include "ObjectManager.h"
#include "ResourceManager.h"
#include "Transform.h"
#include "Input.h"

#include "MeshResource.h"
#include "TextureResource.h"

#include "GL/glew.h"

#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h" 

MeshComponent::MeshComponent(char* name, const char* path, const char* texture_path, GameObject* parent, Application* app, bool active) : Component(name, parent, app, active)
{
	if (path)
		PushMesh(path, texture_path);
	else
		this->path = " ";

	to_draw_normals = false;
	to_draw_AABB = false;

	texResource = NULL;
	AddTexture(texture_path);

	if(mesh.vertices)
		this->parent->transform->Reparent(mesh.transform);
}

MeshComponent::MeshComponent(char* name, Mesh mesh, const char* path, const char* texture_path, GameObject* parent, Application* app, bool active) : Component(name, parent, app, active)
{
	resource = App->resources->Find(path);

	this->mesh = mesh;

	this->path = path;

	to_draw_normals = false;

	texResource = NULL;
	AddTexture(texture_path);


	this->parent->transform->Reparent(mesh.transform);

	if (active)
		App->renderer3D->AddMesh(&this->mesh);
}

MeshComponent::~MeshComponent()
{
	App->renderer3D->DeleteMesh(&mesh);

	App->resources->ReleaseResource(resource);
	App->resources->ReleaseResource(texResource);
}

bool MeshComponent::Update(float dt)
{	
	if (path != " ")
	{
		mesh.transform = this->parent->transform->global_transformation;
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
	if (ImGui::CollapsingHeader("Mesh Component", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("active", &to_activate);
		ImGui::SameLine();

		if(ImGui::Checkbox("normals", &to_draw_normals))
			mesh.drawnormals = to_draw_normals;
		ImGui::SameLine();

		if (ImGui::Checkbox("AABB", &to_draw_AABB))
			mesh.show_bounding_box = to_draw_AABB;

		if(ImGui::Button("aelete"))
			to_delete = true;

		if (ImGui::InputText("name", &name_buffer, ImGuiInputTextFlags_EnterReturnsTrue))
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

		std::string meshName = path;
		if (resource != 0)
		{
			MeshResource* mRes = (MeshResource*)App->resources->RequestResource(resource);
			meshName = mRes->GetAssetFile();
		}
		if (ImGui::BeginCombo("mesh", meshName.c_str()))
		{
			const Folder* const lMeshes = App->resources->GetFolder("LMeshes");
			for (uint i = 0; i < lMeshes->entries.size(); i++)
			{
				if (ImGui::Selectable(lMeshes->entries[i]->name.c_str()))
				{
					if (path != " ")
						App->renderer3D->DeleteMesh(&mesh);
					if (resource != 0)
						App->resources->ReleaseResource(resource);

					PushMesh(lMeshes->entries[i]->GetDirectory().c_str(), nullptr, true);
				}
			}
			ImGui::EndCombo();
		}

		std::string texName = text_path;
		TextureResource* tRes = nullptr;
		if (texResource != 0)
		{
			TextureResource* tRes = (TextureResource*)App->resources->RequestResource(texResource);
			texName = tRes->GetAssetFile();
		}
		if (ImGui::BeginCombo("texture", texName.c_str()))
		{
			const Folder* const lMaterials = App->resources->GetFolder("LMaterials");
			for (uint i = 0; i < lMaterials->entries.size(); i++)
			{
				if (ImGui::Selectable(lMaterials->entries[i]->name.c_str()))
				{
					if (texResource != 0)
						App->resources->ReleaseResource(texResource);

					AddTexture(lMaterials->entries[i]->GetDirectory().c_str(), true);
				}
			}
			ImGui::EndCombo();
		}

		if (texResource != NULL)
		{
			tRes = (TextureResource*)App->resources->RequestResource(texResource);
			if (tRes->GetTexture().id != NULL)
			{
				ImTextureID my_tex_id = (ImTextureID)(GLuint)tRes->GetTexture().id;
				float tex_w = 300;
				float tex_h = 300;
		
				ImGui::Image(my_tex_id, ImVec2(tex_w, tex_h));
			}
		}
	}
}

bool MeshComponent::AddTexture(const char* path, bool library)
{
	if (path)
	{
		text_path = path;

		if(!library)
			texResource = App->resources->Find(path);
		else
			texResource = App->resources->FindFromLibrary(path);

		if (texResource == NULL)
			return false;

		TextureResource* tRes = (TextureResource*)App->resources->RequestResource(texResource);
		PushTexture(tRes->GetTexture().id);
	}

	return true;
}

void MeshComponent::PushMesh(const char* path, const char* texture_path, bool library)
{
	if(!library)
		resource = App->resources->Find(path);
	else
		resource = App->resources->FindFromLibrary(path);

	MeshResource* mRes = (MeshResource*)App->resources->RequestResource(resource);
	std::vector<Mesh> meshes = *mRes->GetMeshes();

	if (meshes.size() == 0)
		return;
	if (meshes.size() == 1)
	{
		mesh = *meshes.begin();
		this->path = path;

		if (active)
			App->renderer3D->AddMesh(&mesh);
	}
	else
	{
		for (int i = 0; i < meshes.size(); i++)
		{
			GameObject* obj = App->objects->AddObject(meshes[i].name.c_str(), parent);
			obj->AddMeshComponent(meshes[i], path, texture_path);
		}

		this->path = " ";
		App->resources->ReleaseResource(resource);
		resource = 0;
		to_delete = true;
	}
}

void MeshComponent::PushTexture(uint texture)
{
	mesh.tex_id = texture;
}

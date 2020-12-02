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

	if (active)
		App->renderer3D->AddMesh(&this->mesh);
}

MeshComponent::~MeshComponent()
{
	App->renderer3D->DeleteMesh(&mesh);
}

bool MeshComponent::Update(float dt)
{	
	if (*parent->last_transform != *parent->transform)
	{
		if (parent->transform->scale.x * parent->transform->scale.y * parent->transform->scale.z == 0)
		{
			parent->transform->scale = parent->last_transform->scale;
		}
		mesh.UpdateScale(parent->transform->scale, parent->last_transform->scale);
		mesh.UpdatePosition(parent->transform->position, parent->last_transform->position);
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

AABB MeshComponent::GenerateAABB()
{	
	local_aabb.SetNegativeInfinity();
	local_aabb.Enclose((float3*)mesh.vertices, mesh.buffersLength[Mesh::vertex] * 3);

	return local_aabb;
}

void MeshComponent::RecalculateAABB_OBB()
{
	float3x3 matrix = { { 	parent->transform->position[0], 	parent->transform->position[1], 	parent->transform->position[2] },
						 { 	parent->transform->rotation[0], 	parent->transform->rotation[1], 	parent->transform->rotation[2] },
						 { 	parent->transform->scale[0], 	parent->transform->scale[1], 	parent->transform->scale[2] }
	};

	obb = GenerateAABB();
	obb.Transform(matrix);

	global_aabb.SetNegativeInfinity();
	global_aabb.Enclose(obb);
}

const AABB MeshComponent::GetGlobalAABB() const
{
	return global_aabb;
}

const OBB MeshComponent::GetOBB() const
{
	return obb;
}

void MeshComponent::DrawGlobalAABB()
{
//	glColor3f(200, 10, 10);
	glLineWidth(1);
	glBegin(GL_LINES);

	glVertex3f(global_aabb.minPoint.x, global_aabb.minPoint.y, global_aabb.minPoint.z);
	glVertex3f(global_aabb.maxPoint.x, global_aabb.minPoint.y, global_aabb.minPoint.z);

	glVertex3f(global_aabb.minPoint.x, global_aabb.minPoint.y, global_aabb.minPoint.z);
	glVertex3f(global_aabb.minPoint.x, global_aabb.minPoint.y, global_aabb.maxPoint.z);

	glVertex3f(global_aabb.minPoint.x, global_aabb.minPoint.y, global_aabb.minPoint.z);
	glVertex3f(global_aabb.minPoint.x, global_aabb.maxPoint.y, global_aabb.minPoint.z);


	glVertex3f(global_aabb.maxPoint.x, global_aabb.minPoint.y, global_aabb.minPoint.z);
	glVertex3f(global_aabb.maxPoint.x, global_aabb.maxPoint.y, global_aabb.minPoint.z);

	glVertex3f(global_aabb.maxPoint.x, global_aabb.minPoint.y, global_aabb.minPoint.z);
	glVertex3f(global_aabb.maxPoint.x, global_aabb.minPoint.y, global_aabb.maxPoint.z);

	glVertex3f(global_aabb.minPoint.x, global_aabb.maxPoint.y, global_aabb.minPoint.z);
	glVertex3f(global_aabb.minPoint.x, global_aabb.maxPoint.y, global_aabb.maxPoint.z);

	glVertex3f(global_aabb.minPoint.x, global_aabb.maxPoint.y, global_aabb.minPoint.z);
	glVertex3f(global_aabb.maxPoint.x, global_aabb.maxPoint.y, global_aabb.minPoint.z);

	glVertex3f(global_aabb.maxPoint.x, global_aabb.maxPoint.y, global_aabb.minPoint.z);
	glVertex3f(global_aabb.maxPoint.x, global_aabb.maxPoint.y, global_aabb.maxPoint.z);

	glVertex3f(global_aabb.maxPoint.x, global_aabb.minPoint.y, global_aabb.maxPoint.z);
	glVertex3f(global_aabb.maxPoint.x, global_aabb.maxPoint.y, global_aabb.maxPoint.z);

	glVertex3f(global_aabb.minPoint.x, global_aabb.minPoint.y, global_aabb.maxPoint.z);
	glVertex3f(global_aabb.maxPoint.x, global_aabb.minPoint.y, global_aabb.maxPoint.z);

	glVertex3f(global_aabb.minPoint.x, global_aabb.minPoint.y, global_aabb.maxPoint.z);
	glVertex3f(global_aabb.minPoint.x, global_aabb.maxPoint.y, global_aabb.maxPoint.z);

	glVertex3f(global_aabb.minPoint.x, global_aabb.maxPoint.y, global_aabb.maxPoint.z);
	glVertex3f(global_aabb.maxPoint.x, global_aabb.maxPoint.y, global_aabb.maxPoint.z);

	glLineWidth(1);
	glEnd();
}

void MeshComponent::DrawOBB()
{
//	glColor3f(100,100,100);
	glLineWidth(1);
	float3* obb_points = nullptr;
	obb.GetCornerPoints(obb_points);

	glBegin(GL_LINES);

	glVertex3f(obb.CornerPoint(0).x, obb.CornerPoint(0).y, obb.CornerPoint(0).z);
	glVertex3f(obb.CornerPoint(1).x, obb.CornerPoint(1).y, obb.CornerPoint(1).z);

	glVertex3f(obb.CornerPoint(0).x, obb.CornerPoint(0).y, obb.CornerPoint(0).z);
	glVertex3f(obb.CornerPoint(4).x, obb.CornerPoint(4).y, obb.CornerPoint(4).z);

	glVertex3f(obb.CornerPoint(0).x, obb.CornerPoint(0).y, obb.CornerPoint(0).z);
	glVertex3f(obb.CornerPoint(2).x, obb.CornerPoint(2).y, obb.CornerPoint(2).z);

	glVertex3f(obb.CornerPoint(2).x, obb.CornerPoint(2).y, obb.CornerPoint(2).z);
	glVertex3f(obb.CornerPoint(3).x, obb.CornerPoint(3).y, obb.CornerPoint(3).z);

	glVertex3f(obb.CornerPoint(1).x, obb.CornerPoint(1).y, obb.CornerPoint(1).z);
	glVertex3f(obb.CornerPoint(5).x, obb.CornerPoint(5).y, obb.CornerPoint(5).z);

	glVertex3f(obb.CornerPoint(1).x, obb.CornerPoint(1).y, obb.CornerPoint(1).z);
	glVertex3f(obb.CornerPoint(3).x, obb.CornerPoint(3).y, obb.CornerPoint(3).z);

	glVertex3f(obb.CornerPoint(4).x, obb.CornerPoint(4).y, obb.CornerPoint(4).z);
	glVertex3f(obb.CornerPoint(5).x, obb.CornerPoint(5).y, obb.CornerPoint(5).z);

	glVertex3f(obb.CornerPoint(4).x, obb.CornerPoint(4).y, obb.CornerPoint(4).z);
	glVertex3f(obb.CornerPoint(6).x, obb.CornerPoint(6).y, obb.CornerPoint(6).z);

	glVertex3f(obb.CornerPoint(2).x, obb.CornerPoint(2).y, obb.CornerPoint(2).z);
	glVertex3f(obb.CornerPoint(6).x, obb.CornerPoint(6).y, obb.CornerPoint(6).z);

	glVertex3f(obb.CornerPoint(5).x, obb.CornerPoint(5).y, obb.CornerPoint(5).z);
	glVertex3f(obb.CornerPoint(7).x, obb.CornerPoint(7).y, obb.CornerPoint(7).z);

	glVertex3f(obb.CornerPoint(6).x, obb.CornerPoint(6).y, obb.CornerPoint(6).z);
	glVertex3f(obb.CornerPoint(7).x, obb.CornerPoint(7).y, obb.CornerPoint(7).z);

	glVertex3f(obb.CornerPoint(3).x, obb.CornerPoint(3).y, obb.CornerPoint(3).z);
	glVertex3f(obb.CornerPoint(7).x, obb.CornerPoint(7).y, obb.CornerPoint(7).z);

	glLineWidth(1);
	glEnd();
}


void MeshComponent::PushTexture(uint texture)
{
	mesh.tex_id = texture;
}

#include "UI_GameObject.h"
#include "Application.h"
#include "ObjectManager.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"

#include "ColliderComponent.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h" 

GameObject_Inspector::GameObject_Inspector(bool isActive, Application* application) : UI("GameObjects", isActive, App)
{
	App = application;
	mesh_path = "";
	name_buffer = "";
}

GameObject_Inspector::~GameObject_Inspector()
{
}

void GameObject_Inspector::Update(float dt)
{
	object = App->objects->selected;
	if (last_object != object)
	{
		mesh_path = "";
		if (object)
			name_buffer = object->name;
		else
			name_buffer = "";
	}

	if (ImGui::Begin("Object Inspector")) 
		if (object)
		{
			if (ImGui::CollapsingHeader("GameObject", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Text("name:");
				ImGui::SameLine();
				char str[64];
				sprintf_s(str, " ", object->name.c_str());
				if (ImGui::InputText(str, &name_buffer, ImGuiInputTextFlags_EnterReturnsTrue))
				{
					bool same = false;
					uint i = 0;
					for (i; i < object->parent->children.size(); i++)
					{
						if (object->parent->children[i]->name == name_buffer)
						{
							same = true;
							break;
						}
					}
					if (!same)
						object->name = name_buffer;
					else
						name_buffer = object->name;
				}
				ImGui::SameLine();
				if (ImGui::Button("delete"))
				{
					object->to_delete = true;
					App->objects->selected = nullptr;
				}
			}
			ImGui::Separator();
			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::InputFloat3("position", object->transform->local_position.ptr());
				ImGui::InputFloat3("rotation", object->transform->euler_rotation.ptr());
				ImGui::InputFloat3("scale", object->transform->local_scale.ptr());
			}
			ImGui::Separator();
			for (uint i = 0; i < object->components.size(); i++)
			{
				object->components[i]->DisplayComponentMenu();
				ImGui::Separator();
			}

			if (ImGui::BeginCombo("  ", "Add New Component"))
			{
				if (ImGui::Selectable("Camera Component")) 
				{
					object->AddCameraComponent("Camera");
				}
				if (ImGui::Selectable("Mesh Component"))
				{
					object->AddMeshComponent(nullptr);
				}
				if (ImGui::Selectable("RigidBody Component"))
				{
					object->AddRigidBodyComponent();
				}
				if (ImGui::Selectable("Box Colider"))
				{
					object->AddColliderComponent(colider_type::BOX);
				}
				if (ImGui::Selectable("Sphere Colider"))
				{
					object->AddColliderComponent(colider_type::SPHERE);
				}
				if (ImGui::Selectable("Capsule Colider"))
				{
					object->AddColliderComponent(colider_type::CAPSULE);
				}
				if (ImGui::Selectable("Mesh Colider"))
				{

				}
				if (ImGui::Selectable("Constraint"))
				{
					object->AddConstraintComponent("Constraint");
				}

				ImGui::EndCombo();
			}

		}
		else
		{
			mesh_path = "";
			name_buffer = "";
		}

	ImGui::End();

	last_object = object;
}

void GameObject_Inspector::AddComponentUI(Component_ui* component)
{
}

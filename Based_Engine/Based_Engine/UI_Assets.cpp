#include "UI_Assets.h"

#include "Application.h"
#include "ResourceManager.h"

#include "FileSystem.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

Assets::Assets(bool isActive, Application* application) : UI("Console", isActive, App)
{
	App = application;
}

Assets::~Assets()
{
}

void Assets::Update(float dt)
{
	std::vector<std::string> files = FileSystem::GetFiles(App->resources->currentFolder.c_str());

	std::string name = "Assets";
	if (App->resources->currentFolder != "")
		name += "/" + App->resources->currentFolder;

	if (ImGui::Begin("Assets", nullptr))
	{
		ImGui::Columns(2);

		IterateFolder(App->resources->assets);

		ImGui::NextColumn();

		ImGui::Text(name.c_str());
		ImGui::SameLine();
		if (ImGui::Button("Back"))
		{
			if (App->resources->currentFolder != "")
			{
				bool found = false;
				for (std::string::iterator itr = App->resources->currentFolder.end() - 1; itr != App->resources->currentFolder.begin(); itr--)
					if (*itr == '/')
					{
						found = true;
						App->resources->currentFolder.erase(itr, App->resources->currentFolder.end());
						break;
					}
				if (!found)
					App->resources->currentFolder.erase(App->resources->currentFolder.begin(), App->resources->currentFolder.end());
			}
		}

		for (uint i = 0; i < files.size(); i++)
		{
			if (files[i] == "Library")
				continue;

			bool isFolder = FileSystem::IsAFolder(files[i]);
			if (isFolder)
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.5f, 0.5f, 0.2f, 1.0f });

			if (ImGui::Button(files[i].c_str()))
			{
				if (isFolder)
				{
					if (App->resources->currentFolder != "")
						App->resources->currentFolder += "/";
					App->resources->currentFolder += files[i];
				}
				else
				{

				}
			}

			if (isFolder)
				ImGui::PopStyleColor();
		}
	}
	ImGui::End();
}

bool Assets::IterateFolder(Folder* folder)
{
	bool output = true;

	if (folder->name == "Library")
		return true;
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;
	if (folder->GetDirectory() == App->resources->currentFolder)
		flags |= ImGuiTreeNodeFlags_Selected;
	if (folder->HasFolders() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf;

	if (ImGui::TreeNodeEx(folder->name.c_str(), flags))
	{
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			//if(ImGui::GetIO().MouseDoubleClicked[ImGuiMouseButton_Left])
			App->resources->currentFolder = folder->GetDirectory();
		}

		for (std::vector<Entry*>::iterator ent = folder->entries.begin(); ent != folder->entries.end(); ent++)
		{
			Entry* entry = *ent;
			if (entry->type == Entry::Type::FOLDER)
				if (!IterateFolder((Folder*)entry))
				{
					ImGui::TreePop();
					return false;
				}
		}
		ImGui::TreePop();
	}
	else
	{
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			App->resources->currentFolder = folder->GetDirectory();
	}

	return output;
}

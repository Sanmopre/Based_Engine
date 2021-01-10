#include "ObjectManager.h"
#include "Application.h"
#include "GameObject.h"
#include "Transform.h"
#include "Input.h"

#include "ColliderComponent.h"
#include "RigidBodyComponent.h"
#include "PlayerController.h"

ObjectManager::ObjectManager(Application* app, bool active) : Module(app, active)
{
	go_id = 0;
}

ObjectManager::~ObjectManager()
{
}

bool ObjectManager::Start()
{
	parent = new GameObject("BASEDObject", nullptr, App, true);

	GameObject* street = AddObject("street");
	street->AddMeshComponent("Assets/Meshes/Street environment_V01.FBX");
	
	for (uint i = 0; i < street->children.size(); i++)
	{
		if(street->children[i]->name != "Plane001")
		{
			street->children[i]->transform->AddPosition(float3(0, 1, 0));
			street->children[i]->AddRigidBodyComponent();
			float3 offset = float3(0, 0, 0);
			float3 size = float3(10, 10, 10);
			street->children[i]->AddColliderComponent(colider_type::BOX, offset, size);
		}
		else
		{
			float3 offset = float3(0, -1.5, 0);
			float3 size = float3(97, 1, 97);
			street->children[i]->AddColliderComponent(colider_type::BOX, offset, size);
		}
	}

	GameObject* ball = AddObject("ball");
	ball->AddMeshComponent("Assets/Meshes/Primitives/sphere.fbx");
	ball->transform->AddPosition(float3(0, 5, 0));
	ball->AddRigidBodyComponent();
	ball->AddColliderComponent(colider_type::SPHERE);
	ball->AddPlayerController();
	ball->rigidbody->ChangeMassAndDensity(1000, 100);
	ball->controller->SetAccAndDec(50000, 30000);

	return true;
}

update_status ObjectManager::Update(float dt)
{
	parent->Update(dt);

	return UPDATE_CONTINUE;
}

update_status ObjectManager::PostUpdate()
{
	if (selected && App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN)
	{
		selected->to_delete = true;
		selected = nullptr;
	}

	return UPDATE_CONTINUE;
}

bool ObjectManager::CleanUp()
{
	delete parent;
	return true;
}

GameObject* ObjectManager::AddObject(char* name, GameObject* parent, bool active, const char* type)
{
	if (name == nullptr)
	{
		char str[64];
		sprintf_s(str, "%s%d", type, go_id);
		go_id++;
		name = str;
	}

	if (!parent)
		parent = this->parent;
	GameObject* output = new GameObject(name, parent, App, active);

	parent->children.push_back(output);
	
	return output;
}
GameObject* ObjectManager::AddObject(const char* name, GameObject* parent, bool active, const char* type)
{
	if (name == nullptr)
	{
		char str[64];
		sprintf_s(str, "%s%d", type, go_id);
		go_id++;
		name = str;
	}

	if (!parent)
		parent = this->parent;
	GameObject* output = new GameObject(name, parent, App, active);

	parent->children.push_back(output);

	return output;
}

bool ObjectManager::ChildGameObject(GameObject* newChild, GameObject* newParent)
{
	if (newChild->parent == newParent)
		return false;

	GameObject* posPar = newParent->parent;
	if (posPar)
	{
		bool out = false;
		while (!out)
		{
			if (posPar == newChild)
			{
				ChildGameObject(newParent, newChild->parent);
				out = true;
				break;
			}
			posPar = posPar->parent;
			if (!posPar)
				out = true;
		}
	}

	for (std::vector<GameObject*>::iterator g = newChild->parent->children.begin(); g != newChild->parent->children.end(); g++)
		if (*g == newChild)
		{
			newChild->parent->children.erase(g);
			break;
		}
	newChild->parent = newParent;
	newParent->children.push_back(newChild);

	return true;
}

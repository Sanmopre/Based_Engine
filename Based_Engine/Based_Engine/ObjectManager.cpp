#include "ObjectManager.h"
#include "Application.h"
#include "GameObject.h"
#include "Transform.h"
#include "Input.h"

#include "ColliderComponent.h"
#include "RigidBodyComponent.h"

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

	//GameObject* street = AddObject("street");
	//street->AddMeshComponent("Assets/Meshes/Street environment_V01.FBX");
	//street->AddRigidBodyComponent();
	//street->AddColliderComponent("sm");

	GameObject* ground = AddObject("ground");
	ground->AddMeshComponent("Assets/Meshes/Primitives/cube.fbx");
	ground->transform->AddScale(float3(99, 0, 99));
	ground->AddColliderComponent(colider_type::BOX);

	GameObject* wall = AddObject("wall");
	wall->AddMeshComponent("Assets/Meshes/Primitives/cube.fbx");
	wall->transform->AddPosition(float3(15, 10, 0));
	wall->transform->AddScale(float3(0, 15, 1));
	wall->AddColliderComponent(colider_type::BOX);

	GameObject* wall2 = AddObject("wall2");
	wall2->AddMeshComponent("Assets/Meshes/Primitives/cube.fbx");
	wall2->transform->AddPosition(float3(-15, 10, 0));
	wall2->transform->AddScale(float3(0, 15, 1));
	wall2->AddColliderComponent(colider_type::BOX);
	
	box = AddObject("box");
	box->AddMeshComponent("Assets/Meshes/Primitives/cube.fbx");
	box->transform->AddPosition(float3(10, 20, 0));	
	box->AddRigidBodyComponent();
	box->AddColliderComponent(colider_type::BOX);


	box2 = AddObject("box2");
	box2->AddMeshComponent("Assets/Meshes/Primitives/cube.fbx");
	box2->transform->AddPosition(float3(-10, 20, 0));
	box2->AddRigidBodyComponent();
	box2->AddColliderComponent(colider_type::BOX);


	return true;
}

update_status ObjectManager::Update(float dt)
{
	if(!App->paused)
		if (selected && selected->rigidbody)
		{
			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
			{
				selected->rigidbody->rigidBody->addForce(physx::PxVec3(0.0f, 0, 1000.0f));
				box2->rigidbody->rigidBody->addForce(physx::PxVec3(-1000.0f, 0, 0));
				box->rigidbody->rigidBody->addForce(physx::PxVec3(1000.0f, 0, 0));
			}
			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
				selected->rigidbody->rigidBody->addForce(physx::PxVec3(0.0f, 0, -1000.0f));
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
				selected->rigidbody->rigidBody->addForce(physx::PxVec3(1000.0f, 0, 0));
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
				selected->rigidbody->rigidBody->addForce(physx::PxVec3(-1000.0f, 0, 0));
		}

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

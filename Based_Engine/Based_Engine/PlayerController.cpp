#include "Application.h"
#include "Input.h"
#include "ObjectManager.h"
#include "PlayerController.h"

#include "GameObject.h"
#include "RigidBodyComponent.h"

#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h" 

#include "PxPhysicsAPI.h"

PlayerController::PlayerController(char* name, GameObject* parent, Application* app, bool active)
	: Component(name, parent, app, active)
{
}

PlayerController::~PlayerController()
{
	parent->controller = nullptr;
}

bool PlayerController::Update(float dt)
{
	if (!App->paused)
		if (parent->rigidbody)
		{
			physx::PxVec3 vel = parent->rigidbody->rigidBody->getLinearVelocity();

			bool w = false;
			bool s = false;
			bool d = false;
			bool a = false;
			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
				w = true;
			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
				s = true;
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
				d = true;
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
				a = true;

			if (s)
				parent->rigidbody->rigidBody->addForce(physx::PxVec3(0, 0, acceleration));
			else if (!w)
				if (vel.z > 0)
					parent->rigidbody->rigidBody->addForce(physx::PxVec3(0, 0, -deceleration));

			if (w)
				parent->rigidbody->rigidBody->addForce(physx::PxVec3(0, 0, -acceleration));
			else if (!s)
				if (vel.z < 0)
					parent->rigidbody->rigidBody->addForce(physx::PxVec3(0, 0, deceleration));

			if (a)
				parent->rigidbody->rigidBody->addForce(physx::PxVec3(acceleration, 0, 0));
			else if (!d)
				if (vel.x > 0)
					parent->rigidbody->rigidBody->addForce(physx::PxVec3(-deceleration, 0, 0));

			if (d)
				parent->rigidbody->rigidBody->addForce(physx::PxVec3(-acceleration, 0, 0));
			else if (!a)
				if (vel.x < 0)
					parent->rigidbody->rigidBody->addForce(physx::PxVec3(deceleration, 0, 0));

			bool changed = false;
			for (int i = 0; i < 3; i++)
				if (vel[i] > maxVelocity)
				{
					vel[i] = maxVelocity;
					changed = true;
				}
				else if (vel[i] < -maxVelocity)
				{
					vel[i] = -maxVelocity;
					changed = true;
				}
			if (changed)
				parent->rigidbody->rigidBody->setLinearVelocity(vel);
		}
		else
		{
			bool pressed = false;
			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
				pressed = true;
			else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
				pressed = true;
			else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
				pressed = true;
			else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
				pressed = true;
			if (pressed)
				LOG("Player controller error! No RigidBody found!");
		}

	return true;
}

void PlayerController::DisplayComponentMenu()
{
	if (ImGui::CollapsingHeader("Player Controller", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("active controller", &active);
		ImGui::SameLine();

		if (ImGui::Button("delete controller"))
			to_delete = true;

		ImGui::InputFloat("max speed", &maxVelocity);
		ImGui::InputFloat("acceleration", &acceleration);
		ImGui::InputFloat("deceleration", &deceleration);
	}
}

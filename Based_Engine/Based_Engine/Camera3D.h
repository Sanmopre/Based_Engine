#pragma once
#include "Module.h"
#include "glmath.h"

#include "PxRigidActor.h"
#include "MathGeoLib.h"
#include "PxPhysicsAPI.h"

class Camera3D : public Module
{
public:
	Camera3D(Application* app, bool start_enabled = true);
	~Camera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();

	bool CameraMovement();

private:

	void CalculateViewMatrix();

public:
	
	vec3 X, Y, Z, Position, Reference;

	float speed;
	float speed_shift = 0.85;
	float speed_normal = 0.25;

private:

	physx::PxRigidDynamic* rigidBody = nullptr;	
	physx::PxShape* shape = nullptr;
	float radius = 2.0f;

	mat4x4 ViewMatrix, ViewMatrixInverse;

};
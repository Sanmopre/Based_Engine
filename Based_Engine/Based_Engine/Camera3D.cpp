#include "Application.h"
#include "Camera3D.h"
#include "Input.h"
#include "PhysicsEngine.h"
#include "PxPhysicsAPI.h"

Camera3D::Camera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 10.0f, 0.0f);
	Reference = vec3(0.0f, 10.0f, 0.0f);
}

Camera3D::~Camera3D()
{}

// -----------------------------------------------------------------
bool Camera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	physx::PxVec3 p = physx::PxVec3(Position.x, Position.y , Position.z);
	rigidBody = App->physics->physics->createRigidDynamic(physx::PxTransform(p));

	physx::PxSphereGeometry SphereGeometry(radius * 1.5);
	shape = App->physics->physics->createShape(SphereGeometry, *App->physics->material);

	rigidBody->attachShape(*shape);
	App->physics->AddActor(rigidBody);

	rigidBody->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
	rigidBody->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);
	rigidBody->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);

	rigidBody->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
	return ret;
}

// -----------------------------------------------------------------
bool Camera3D::CleanUp()
{
	LOG("Cleaning camera");
	App->physics->DeleteActor(rigidBody);
	rigidBody->detachShape(*shape);
	shape->release();
	rigidBody->release();
	return true;
}

// -----------------------------------------------------------------
update_status Camera3D::Update(float dt)
{
	CalculateViewMatrix();

	if (CameraMovement()) 
	{
		if (App->paused)
		{
			App->physics->DeleteActor(rigidBody);

			physx::PxVec3 newPosition = physx::PxVec3(Position.x, Position.y, Position.z);
			physx::PxTransform transform = physx::PxTransform(newPosition);
			rigidBody->setGlobalPose(transform, true);

			App->physics->AddActor(rigidBody);
		}
		else
		{
			physx::PxTransform transform = rigidBody->getGlobalPose();
			float3 position = float3(transform.p.x, transform.p.y, transform.p.z);
			Position.x = transform.p.x;
			Position.y = transform.p.y;
			Position.z = transform.p.z;
		}
	}
	else 
	{	
		rigidBody->setLinearVelocity(physx::PxVec3(0, 0, 0));
		physx::PxTransform transform = rigidBody->getGlobalPose();
		float3 position = float3(transform.p.x, transform.p.y, transform.p.z);
		Position.x = transform.p.x;
		Position.y = transform.p.y;
		Position.z = transform.p.z;
	}

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void Camera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void Camera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void Camera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* Camera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

bool Camera3D::CameraMovement()
{
	bool output = false;

	vec3 newPos(0, 0, 0);

	float multiplier = 1.0f;

	if (!App->paused)
		multiplier = 100.0f;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	{
		speed = speed_shift * multiplier;
	}
	else {
		speed = speed_normal * multiplier;
	}

	
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		if (!App->paused)
			rigidBody->setLinearVelocity(physx::PxVec3(-Z.x * speed, -Z.y * speed, -Z.z * speed));
		else
			newPos -= Z * speed;

		output = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) 
	{
		if (!App->paused)
			rigidBody->setLinearVelocity(physx::PxVec3(Z.x * speed, Z.y * speed, Z.z * speed));
		else
			newPos += Z * speed;
		output = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if (!App->paused)
			rigidBody->setLinearVelocity(physx::PxVec3(-X.x * speed, -X.y * speed, -X.z * speed));
		else
			newPos -= X * speed;
		output = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if (!App->paused)
			rigidBody->setLinearVelocity(physx::PxVec3(X.x * speed, X.y * speed, X.z * speed));
		else
			newPos += X * speed;
		output = true;
	}

	Position += newPos;
	Reference += newPos;

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		Position -= Reference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if (Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

		Position = Reference + Z * length(Position);
	}
	return output;
}

// -----------------------------------------------------------------
void Camera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

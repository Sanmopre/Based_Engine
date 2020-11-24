#pragma once

#include "Globals.h"
#include "Color.h"
#include "MathGeoLib.h"
#include "MathBuildConfig.h"
#include "Mesh.h"

class Application;
class Mesh;
class GameObject;
struct Transform;

class Component
{
public:

	Component(char* name, GameObject* parent, Application* app, bool active = true);
	virtual ~Component();

	virtual bool Update(float dt);

	virtual void Activate();
	virtual void Deactivate();
	bool IsActive();

	virtual void DisplayComponentMenu() = 0;

	virtual bool AddTexture(const char* path) { return false; }

	bool to_delete;


	std::string name;


protected:

	bool to_activate;

	Application* App;
	bool active;

	std::string name_buffer;

	GameObject* parent;
};

class MeshComponent : public Component
{
public:

	MeshComponent(char* name, const char* path, const char* texture_path, GameObject* parent, Application* app, bool active = true);
	MeshComponent(char* name, Mesh mesh, const char* texture_path, GameObject* parent, Application* app, bool active = true);
	virtual ~MeshComponent();

	bool Update(float dt);

	void Activate();
	void Deactivate();

	void DisplayComponentMenu();
	bool AddTexture(const char* path);

	void GenerateAABB();

public:

	AABB bbox;
	OBB obb;

private:

	bool to_draw_normals;

	void PushTexture(uint texture);

	std::string path;
	std::string path_buffer;

	std::string text_path;
	std::string text_path_buffer;

	Mesh mesh;
	uint texture;


};

class CameraComponent : public Component
{
public:

	CameraComponent(char* name, const char* path, const char* texture_path, GameObject* parent, Application* app, bool active = true);
	virtual ~CameraComponent();
	void Look(const float3& position_to_look);

	float* GetProjectionMatrix() const;

	float* GetViewMatrix() const;

	void SetVerticalFov(const float& vertical_fov);
	float GetVerticalFov() const;
	void SetHorizontalFov(const float& horitzontal_fov);
	float GetHorizontalFov() const;

	void SetFarPlane(const float& far_plane);
	void SetNearPlane(const float& near_plane);
	float GetFarPlane() const;
	float GetNearPlane() const;

	void SetCameraPosition(const float3& position);
	float3 GetCameraPosition() const;

	void DisplayComponentMenu();

private:
	void AspectRatio(int width_ratio, int height_ratio, bool fov_type = 0);

	void DrawFrustum();

	void SetComponent(Component* component);

private:


	float vertical_fov = 60.0f;
	float horizontal_fov = 91.0f;

	bool projection_changed = false;

	float far_plane = 200.f;

	float near_plane = 0.1f;

	int is_fov_horizontal = 0;

//	float4x4 ViewMatrix = float4x4::identity();
//	float4x4 ViewMatrixInverse = float4x4::identity();

public:	
	Frustum frustum;
	Color camera_color_background{ 0.0f, 0.0f, 0.0f, 1.0f };
};
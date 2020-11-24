#include "Components.h"

#include "Application.h"
#include "Renderer3D.h"
#include "Simp.h"
#include "Mesh.h"
#include "TextureLoader.h"
#include "GameObject.h"
#include "Input.h"
#include "imgui.h"
#include "MathGeoLib.h"
#include "MathBuildConfig.h"

#include "GL/glew.h"


CameraComponent::CameraComponent(char* name, const char* path, const char* texture_path, GameObject* parent, Application* app, bool active) : Component(name, parent, app, active)
{
	frustum.type = FrustumType::PerspectiveFrustum;

	//frustum.pos = float3::zero();
	frustum.pos = { 0,0,0 };
	//frustum.front = float3::unitZ();
	frustum.front = { 0,0,1 };
	//frustum.up = float3::unitY();
	frustum.up = { 0,1,0 };

	frustum.nearPlaneDistance = near_plane;
	frustum.farPlaneDistance = far_plane;
	frustum.verticalFov = DEGTORAD * vertical_fov;
	AspectRatio(16, 9);
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::DisplayComponentMenu()
{

}


void CameraComponent::Look(const float3& position_to_look)
{
	if (position_to_look.IsFinite()) {
		float3 direction = position_to_look - frustum.pos;

		float3x3 matrix = float3x3::LookAt(frustum.front, direction.Normalized(), frustum.up, /*float3::unitY()*/{0,1,0});

		frustum.front = matrix.MulDir(frustum.front).Normalized();
		frustum.up = matrix.MulDir(frustum.up).Normalized();
	}
}

float* CameraComponent::GetProjectionMatrix() const
{
	return (float*)frustum.ProjectionMatrix().Transposed().v;
}

float* CameraComponent::GetViewMatrix() const
{
	return (float*)static_cast<float4x4>(frustum.ViewMatrix()).Transposed().v;
}

void CameraComponent::SetVerticalFov(const float& vertical_fov)
{
	this->vertical_fov = vertical_fov;
	frustum.verticalFov = DEGTORAD * vertical_fov;
	AspectRatio(16, 9);
}

float CameraComponent::GetVerticalFov() const
{
	return vertical_fov;
}

void CameraComponent::SetHorizontalFov(const float& horitzontal_fov)
{
	this->horizontal_fov = horizontal_fov;
	frustum.horizontalFov = DEGTORAD * horizontal_fov;
	AspectRatio(16, 9, true);
}

float CameraComponent::GetHorizontalFov() const
{
	return horizontal_fov;
}

void CameraComponent::SetFarPlane(const float& far_plane)
{
	this->far_plane = far_plane;
	frustum.farPlaneDistance = far_plane;
}

void CameraComponent::SetNearPlane(const float& near_plane)
{
	this->near_plane = near_plane;
	frustum.nearPlaneDistance = near_plane;
}

float CameraComponent::GetFarPlane() const
{
	return far_plane;
}

float CameraComponent::GetNearPlane() const
{
	return near_plane;
}

void CameraComponent::SetCameraPosition(const float3& position)
{
	frustum.pos = position;
}

float3 CameraComponent::GetCameraPosition() const
{
	return frustum.pos;
}

void CameraComponent::AspectRatio(int width_ratio, int height_ratio, bool fov_type)
{
	if (fov_type == 0)
	{
		frustum.horizontalFov = (2.f * atanf(tanf(frustum.verticalFov * 0.5f) * ((float)width_ratio / (float)height_ratio)));
	}
	else
	{
		frustum.verticalFov = (2.f * atanf(tanf(frustum.horizontalFov * 0.5f) * ((float)height_ratio) / (float)width_ratio));
	}
}


void CameraComponent::DrawFrustum()
{
	static float3 points[8];
	frustum.GetCornerPoints(points);

	glLineWidth(1);
	//glColor3f(100, 100, 100);
	glBegin(GL_LINES);

	glVertex3f(points[0].x, points[0].y, points[0].z);
	glVertex3f(points[1].x, points[1].y, points[1].z);

	glVertex3f(points[0].x, points[0].y, points[0].z);
	glVertex3f(points[4].x, points[4].y, points[4].z);

	glVertex3f(points[4].x, points[4].y, points[4].z);
	glVertex3f(points[5].x, points[5].y, points[5].z);

	glVertex3f(points[0].x, points[0].y, points[0].z);
	glVertex3f(points[2].x, points[2].y, points[2].z);

	glVertex3f(points[2].x, points[2].y, points[2].z);
	glVertex3f(points[3].x, points[3].y, points[3].z);

	glVertex3f(points[1].x, points[1].y, points[1].z);
	glVertex3f(points[3].x, points[3].y, points[3].z);

	glVertex3f(points[1].x, points[1].y, points[1].z);
	glVertex3f(points[5].x, points[5].y, points[5].z);

	glVertex3f(points[4].x, points[4].y, points[4].z);
	glVertex3f(points[6].x, points[6].y, points[6].z);

	glVertex3f(points[2].x, points[2].y, points[2].z);
	glVertex3f(points[6].x, points[6].y, points[6].z);

	glVertex3f(points[6].x, points[6].y, points[6].z);
	glVertex3f(points[7].x, points[7].y, points[7].z);

	glVertex3f(points[5].x, points[5].y, points[5].z);
	glVertex3f(points[7].x, points[7].y, points[7].z);

	glVertex3f(points[3].x, points[3].y, points[3].z);
	glVertex3f(points[7].x, points[7].y, points[7].z);

	glEnd();
	glLineWidth(1);
}


void CameraComponent::SetComponent(Component* component)
{
	camera_color_background = { 0.05f, 0.05f, 0.05f, 1.0f };

	near_plane = 0.1f;
	far_plane = 200.f;
	frustum.nearPlaneDistance = near_plane;
	frustum.farPlaneDistance = far_plane;

	vertical_fov = 60.0f;
	frustum.verticalFov = DEGTORAD * vertical_fov;
	AspectRatio(16, 9);
	horizontal_fov = frustum.horizontalFov * RADTODEG;
}

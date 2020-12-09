#include "Components.h"

#include "Application.h"
#include "Renderer3D.h"
#include "Simp.h"
#include "Mesh.h"
#include "TextureLoader.h"
#include "GameObject.h"
#include "Input.h"
#include "imgui.h"
#include "Math/float3.h"

#include "GL/glew.h"


CameraComponent::CameraComponent(char* name, const char* path, const char* texture_path, GameObject* parent, Application* app, bool active) : Component(name, parent, app, active)
{
	/*
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetPos(float3(0, 0, 0));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	frustum.SetViewPlaneDistances(0.1f, 10.0f);
	frustum.SetPerspective(1.0f, 1.0f);

	UpdatePlanes();
	corners = new vec[8];
	*/
}

CameraComponent::~CameraComponent()
{
}

bool CameraComponent::Update(float dt)
{
	DrawFrustum();
	return false;
}

void CameraComponent::Setposition(float3 pos)
{
	//frustum.SetPos(pos);
	UpdatePlanes();
}

void CameraComponent::SetNearPlane(float distance)
{
	//frustum.SetViewPlaneDistances(distance, GetFarPlaneDistance());
	UpdatePlanes();
}

void CameraComponent::SetFarPlane(float distance)
{
	//frustum.SetViewPlaneDistances(GetNearPlaneDistance(), distance);
	UpdatePlanes();
}

void CameraComponent::SetVerticalFov(float verticalFov)
{
	//frustum.SetVerticalFovAndAspectRatio(verticalFov, frustum.AspectRatio());
}

void CameraComponent::SetHorizontalFov(float horizontalFov)
{
	//frustum.SetHorizontalFovAndAspectRatio(horizontalFov, frustum.AspectRatio());
}

float CameraComponent::ComputeAspectRatio(float verticalFov, float horizontalFov)
{
	return (Tan(verticalFov / 2) / Tan(horizontalFov / 2));
}

float* CameraComponent::GetViewMatrix()
{
	float4x4 mat = frustum.ViewMatrix();

	return (float*)mat.Transposed().v; 
}

void CameraComponent::UpdatePlanes()
{
	frustum.GetPlanes(planes);
}

Frustum CameraComponent::GetFrustum() const
{
	return frustum;
}

float3 CameraComponent::GetPos() const
{
//	return frustum.Pos();
}

float CameraComponent::GetNearPlaneDistance() const
{
//	return frustum.NearPlaneDistance();
}

float CameraComponent::GetFarPlaneDistance() const
{
//	return frustum.FarPlaneDistance();
}

float CameraComponent::GetVerticalFov() const
{
//	return frustum.VerticalFov();
}

float CameraComponent::GetHorizontalFov() const
{
//	return frustum.HorizontalFov();
}

void CameraComponent::DrawFrustum()
{
	static float3 points[8];
	frustum.GetCornerPoints(points);

	glLineWidth(1);
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



#ifndef __CAMERACOMPONENT_H__
#define __CAMERACOMPONENT_H__

#include "Component.h"

#include "Math/float3.h"
#include "Geometry/Frustum.h"
#include "Geometry/Plane.h"

class Mesh;

class CameraComponent : public Component
{
public:

	CameraComponent(char* name, GameObject* parent, Application* app, bool active = true);
	virtual ~CameraComponent();

	bool Update(float dt);

	void Setposition(float3 pos);

	void SetNearPlane(float distance);
	void SetFarPlane(float distance);

	void SetVerticalFov(float verticalFov);
	void SetHorizontalFov(float horizontalFov);

	float ComputeAspectRatio(float verticalFov, float horizontalFov);

	float* GetViewMatrix();
	void UpdatePlanes();

	Frustum GetFrustum()const;
	float3 GetPos()const;
	float GetNearPlaneDistance()const;
	float GetFarPlaneDistance()const;
	float GetVerticalFov()const;
	float GetHorizontalFov()const;

	void DisplayComponentMenu();
	void DrawFrustum();

	bool IsObjectInFrustum(Mesh* mesh);

public:

	Frustum frustum;
	bool cull = true;
	Plane planes[6];
};

#endif //__CAMERACOMPONENT_H__
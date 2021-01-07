#include "Globals.h"

#include "Math/float3.h"
#include "Math/float4x4.h"
#include "Math/Quat.h"

class GameObject;

class Transform
{
public:

	Transform(GameObject* game_object);
	Transform(GameObject* game_object, float4x4 transform);
	virtual ~Transform();

	GameObject* object;

	float4x4 global_transformation = float4x4::identity;
	float4x4 local_transformation = float4x4::identity;

	// position
	float3 local_position = { 0,0,0 };
	// sacale
	float3 local_scale = { 0,0,0 };
	// rotation
	Quat local_rotation = { 0,0,0,0 };

	float3 euler_rotation = { 0,0,0 };

	void RecalculateTransform();

	void AddPosition(const float3 pos);
	void AddScale(const float3 scale);
	void AddRotation(const float3 rot);

	void SetTransform(float4x4 transform);

	void Reparent(const float4x4& transform);
	void Reset();

	void SetLocalPosition(const float3& new_local_pos);
	void SetLocalPosition(const float& x, const float& y, const float& z);

	const float3 GetLocalPosition() const;
	const float3 GetGlobalPosition() const;

	void SetLocalScale(const float3& new_local_scale);
	void SetLocalScale(const float& x, const float& y, const float& z);

	const float3 GetLocalScale() const;
	const float3 GetGlobalScale() const;

	void SetLocalRotation(const Quat& new_local_rotation);
	void SetLocalRotation(const float& x, const float& y, const float& z, const float& angle);

	const Quat GetLocalRotation() const;
	const Quat GetGlobalRotation() const;

	bool ToUdate();

private:

	float4x4 last_transformation = float4x4::identity;
};
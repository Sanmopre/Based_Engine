#include "Globals.h"
#include "Math/float3.h"
#include "MathGeoLib.h"

class GameObject;

class Transform
{
public:
	Transform(GameObject* game_object);
	virtual ~Transform();

	float3 position, rotation, scale;

	float3 forward = { 0,0,0 };
	float3 up = { 0,0,0 };
	float3 right = { 0,0,0 };

	GameObject* object;

	float4x4 global_transformation;
	float4x4 local_transformation;

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

	void Reparent(const float4x4& transform);
	void Reset();

public:

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



	//Operators
	bool operator==(Transform t)
	{
		if (position.x == t.position.x && position.y == t.position.y && position.z == t.position.z &&
			rotation.x == t.rotation.x && rotation.y == t.rotation.y && rotation.z == t.rotation.z &&
			scale.x == t.scale.x && scale.y == t.scale.y && scale.z == t.scale.z)
			return true;
		return false;
	}
	bool operator!=(Transform t)
	{
		if (position.x == t.position.x && position.y == t.position.y && position.z == t.position.z &&
			rotation.x == t.rotation.x && rotation.y == t.rotation.y && rotation.z == t.rotation.z &&
			scale.x == t.scale.x && scale.y == t.scale.y && scale.z == t.scale.z)
			return false;
		return true;
	}
};
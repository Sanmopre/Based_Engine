#include "Globals.h"
#include "Math/float3.h"
#include "MathGeoLib.h"

class GameObject;

class Transform
{
public:
	Transform();
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
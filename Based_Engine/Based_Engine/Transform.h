#include "Globals.h"
#include "Math/float3.h"


struct Transform
{
	Transform();
	virtual ~Transform();

	float3 position, rotation, scale;

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
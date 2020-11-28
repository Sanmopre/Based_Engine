#include "Transform.h"
#include "GameObject.h"

Transform::Transform() 
{
	position = { 0,0,0 };
	rotation = { 0,0,0 };
	scale = { 1,1,1 };

	local_position = { 0,0,0 };
	local_rotation = { 0,0,0 };
	local_scale = { 0,0,0 };

	euler_rotation = local_rotation.ToEulerXYX();
	euler_rotation.x = RadToDeg(euler_rotation.x);
	euler_rotation.y = RadToDeg(euler_rotation.y);
	euler_rotation.z = RadToDeg(euler_rotation.z);
	local_transformation = float4x4::FromTRS(local_position, local_rotation, local_scale);
}

Transform::~Transform()
{

}



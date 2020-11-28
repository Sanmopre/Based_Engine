#include "Transform.h"
#include "GameObject.h"

Transform::Transform(GameObject* game_object)
{
	//temp//
	position = { 0,0,0 };
	rotation = { 0,0,0 };
	scale = { 1,1,1 };
	//   //

	object = game_object;

	local_position = { 0,0,0 };
	local_rotation = { 0,0,0 };
	local_scale = { 0,0,0 };
	euler_rotation = local_rotation.ToEulerXYX();
	euler_rotation.x = RadToDeg(euler_rotation.x);
	euler_rotation.y = RadToDeg(euler_rotation.y);
	euler_rotation.z = RadToDeg(euler_rotation.z);
	local_transformation = float4x4::FromTRS(local_position, local_rotation, local_scale);

	if (object->parent != nullptr) 
	{
		global_transformation = object->parent->transform->global_transformation * local_transformation;
	}
	else 
		global_transformation = local_transformation;

	up = { 2 * (local_rotation.x * local_rotation.y - local_rotation.w * local_rotation.z),
		1 - 2 * (local_rotation.x * local_rotation.x + local_rotation.z * local_rotation.z),
		2 * (local_rotation.y * local_rotation.z + local_rotation.w * local_rotation.x) };
	forward = { 2 * (local_rotation.x * local_rotation.z + local_rotation.w * local_rotation.y),
			2 * (local_rotation.y * local_rotation.z - local_rotation.w * local_rotation.x),
			1 - 2 * (local_rotation.x * local_rotation.x + local_rotation.y * local_rotation.y) };
	right = { 1 - 2 * (local_rotation.y * local_rotation.y + local_rotation.z * local_rotation.z),
		 2 * (local_rotation.x * local_rotation.y + local_rotation.w * local_rotation.z),
		 2 * (local_rotation.x * local_rotation.z + local_rotation.w * local_rotation.y) };
}

Transform::~Transform()
{

}

void Transform::RecalculateTransform()
{
	local_transformation = float4x4::FromTRS(local_position, local_rotation, local_scale);
	
	if (object->parent != nullptr) {
		if (object->parent->transform != nullptr)
			global_transformation = object->parent->transform->global_transformation * local_transformation;
		else
			global_transformation = local_transformation;
	}
	else {
		global_transformation = local_transformation;
	}
	up = { 2 * (local_rotation.x * local_rotation.y - local_rotation.w * local_rotation.z),
			1 - 2 * (local_rotation.x * local_rotation.x + local_rotation.z * local_rotation.z),
			2 * (local_rotation.y * local_rotation.z + local_rotation.w * local_rotation.x) };
	forward = { 2 * (local_rotation.x * local_rotation.z + local_rotation.w * local_rotation.y),
				2 * (local_rotation.y * local_rotation.z - local_rotation.w * local_rotation.x),
				1 - 2 * (local_rotation.x * local_rotation.x + local_rotation.y * local_rotation.y) };
	right = { 1 - 2 * (local_rotation.y * local_rotation.y + local_rotation.z * local_rotation.z),
			 2 * (local_rotation.x * local_rotation.y + local_rotation.w * local_rotation.z),
			 2 * (local_rotation.x * local_rotation.z + local_rotation.w * local_rotation.y) };

	std::vector<GameObject*>::iterator item = object->children.begin();
	for (; item != object->children.end(); ++item) {
		if (*item != nullptr) {
			(*item)->transform->RecalculateTransform();
		}
	}


	//recalculate		mesh->RecalculateAABB_OBB();

}

void Transform::SetLocalPosition(const float3& new_local_pos)
{
	local_position = new_local_pos;

	RecalculateTransform();
}

void Transform::SetLocalPosition(const float& x, const float& y, const float& z)
{
	local_position.x = x;
	local_position.y = y;
	local_position.z = z;

	RecalculateTransform();
}

const float3 Transform::GetLocalPosition() const
{
	return local_position;
}

const float3 Transform::GetGlobalPosition() const
{
	float3 pos, scale;
	Quat rot;

	global_transformation.Decompose(pos, rot, scale);

	return pos;
}

void Transform::SetLocalScale(const float3& new_local_scale)
{
	local_scale = new_local_scale;

	RecalculateTransform();
}

void Transform::SetLocalScale(const float& x, const float& y, const float& z)
{
	local_scale.x = x;
	local_scale.y = y;
	local_scale.z = z;

	RecalculateTransform();
}

const float3 Transform::GetLocalScale() const
{
	return local_scale;
}

const float3 Transform::GetGlobalScale() const
{
	float3 pos, scale;
	Quat rot;

	global_transformation.Decompose(pos, rot, scale);

	return scale;
}

void Transform::SetLocalRotation(const Quat& new_local_rotation)
{
	local_rotation.Set(new_local_rotation.x, new_local_rotation.y, new_local_rotation.z, new_local_rotation.w);

	euler_rotation = local_rotation.ToEulerXYZ();
	euler_rotation.x = RadToDeg(euler_rotation.x);
	euler_rotation.y = RadToDeg(euler_rotation.y);
	euler_rotation.z = RadToDeg(euler_rotation.z);

	RecalculateTransform();
}

void Transform::SetLocalRotation(const float& x, const float& y, const float& z, const float& angle)
{
	local_rotation.x = x;
	local_rotation.y = y;
	local_rotation.z = z;
	local_rotation.w = angle;

	euler_rotation = local_rotation.ToEulerXYZ();
	euler_rotation.x = RadToDeg(euler_rotation.x);
	euler_rotation.y = RadToDeg(euler_rotation.y);
	euler_rotation.z = RadToDeg(euler_rotation.z);

	RecalculateTransform();
}

const Quat Transform::GetLocalRotation() const
{
	return local_rotation;
}

const Quat Transform::GetGlobalRotation() const
{
	float3 pos, scale;
	Quat rot;

	global_transformation.Decompose(pos, rot, scale);

	return rot;
}



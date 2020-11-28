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



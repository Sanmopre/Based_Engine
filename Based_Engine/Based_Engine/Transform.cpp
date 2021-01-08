#include "Transform.h"
#include "GameObject.h"
#include "MeshComponent.h"
#include "Mesh.h"

Transform::Transform(GameObject* game_object)
{
	object = game_object;

	local_position = { 0,0,0 };
	local_rotation = {0,0,0,0 };
	local_scale = { 1,1,1 };
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

	last_transformation = global_transformation;
}

Transform::Transform(GameObject* game_object, float4x4 transform)
{
	Reparent(transform);
	local_transformation = float4x4::FromTRS(local_position, local_rotation, local_scale);

	if (object->parent != nullptr)
	{
		global_transformation = object->parent->transform->global_transformation * local_transformation;
	}
	else
		global_transformation = local_transformation;

}

Transform::~Transform()
{

}

void Transform::RecalculateTransform()
{
	local_rotation = Quat::FromEulerXYZ(DegToRad(euler_rotation.x), DegToRad(euler_rotation.y), DegToRad(euler_rotation.z));
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

	std::vector<GameObject*>::iterator item = object->children.begin();
	for (; item != object->children.end(); ++item) {
		if (*item != nullptr) {
			(*item)->transform->RecalculateTransform();
		}
	}

	if (object->meshComp != nullptr) {
		object->meshComp->mesh.RecalculateAABB_OBB(global_transformation);
	}
}

void Transform::AddPosition(const float3 pos)
{
	local_position += pos;
	RecalculateTransform();
}

void Transform::AddScale(const float3 scale)
{
	local_scale += scale;
	RecalculateTransform();
}

void Transform::AddRotation(const float3 rot)
{
	euler_rotation += rot;
	float3 aux = euler_rotation;
	aux.x = DegToRad(euler_rotation.x);
	aux.y = DegToRad(euler_rotation.y);
	aux.z = DegToRad(euler_rotation.z);
	local_rotation = Quat::FromEulerXYZ(aux.x, aux.y, aux.z);

	RecalculateTransform();
}

void Transform::SetTransform(float4x4 transform)
{
	global_transformation = transform;
	
	float3 position, scale;
	Quat rotation;
	transform.Decompose(position, rotation, scale);
	this->local_scale = scale;
	this->local_position = position;
	this->local_rotation = rotation;
	euler_rotation = local_rotation.ToEulerXYZ();
	euler_rotation.x = RadToDeg(euler_rotation.x);
	euler_rotation.y = RadToDeg(euler_rotation.y);
	euler_rotation.z = RadToDeg(euler_rotation.z);

	last_transformation = global_transformation;
}

void Transform::Reparent(const float4x4& transform)
{
	float3 position, scale;
	Quat rotation;
	transform.Decompose(position, rotation, scale);

	this->local_scale = scale;
	this->local_position = position;
	this->local_rotation = rotation;
	euler_rotation = local_rotation.ToEulerXYZ();
	euler_rotation.x = RadToDeg(euler_rotation.x);
	euler_rotation.y = RadToDeg(euler_rotation.y);
	euler_rotation.z = RadToDeg(euler_rotation.z);

	RecalculateTransform();
}

void Transform::Reset()
{
	local_scale = { 1,1,1 };
	local_position = { 0,0,0 };
	local_rotation = {0,0,0,0 };

	euler_rotation = local_rotation.ToEulerXYZ();
	euler_rotation.x = RadToDeg(euler_rotation.x);
	euler_rotation.y = RadToDeg(euler_rotation.y);
	euler_rotation.z = RadToDeg(euler_rotation.z);

	RecalculateTransform();
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
	return 	global_transformation.GetScale();
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

bool Transform::ToUdate()
{
	Quat last_rotation = Quat::FromEulerXYZ(DegToRad(euler_rotation.x), DegToRad(euler_rotation.y), DegToRad(euler_rotation.z));
	float4x4 last_transformation = float4x4::FromTRS(local_position, last_rotation, local_scale);

	if (object->parent && object->parent->transform)
		last_transformation = object->parent->transform->global_transformation * last_transformation;


	bool output = !global_transformation.Equals(last_transformation);

	return output;
}



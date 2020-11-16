#pragma once

#include "Globals.h"

class Application;
class Mesh;
class GameObject;
struct Transform;

class Component
{
public:

	Component(char* name, GameObject* parent, Application* app, bool active = true);
	virtual ~Component();

	virtual bool Update(float dt);

	virtual void Activate();
	virtual void Deactivate();
	bool IsActive();

	virtual void DisplayComponentMenu() = 0;

	virtual bool AddTexture(const char* path) { return false; }

	bool to_delete;


	std::string name;


protected:

	bool to_activate;

	Application* App;
	bool active;

	std::string name_buffer;

	GameObject* parent;
};

class MeshComponent : public Component
{
public:

	MeshComponent(char* name, const char* path, const char* texture_path, GameObject* parent, Application* app, bool active = true);
	virtual ~MeshComponent();

	bool Update(float dt);

	void Activate();
	void Deactivate();

	void DisplayComponentMenu();
	bool AddTexture(const char* path);

private:

	bool to_draw_normals;

	void PushTexture(uint texture);

	std::string path;
	std::string path_buffer;

	std::string text_path;
	std::string text_path_buffer;

	MESH mesh;
	uint texture;
};
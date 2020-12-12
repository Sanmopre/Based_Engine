#ifndef __MESHCOMPONENT_H__
#define __MESHCOMPONENT_H__

#include "Component.h"
#include "Mesh.h"

class MeshComponent : public Component
{
public:

	MeshComponent(char* name, const char* path, const char* texture_path, GameObject* parent, Application* app, bool active = true);
	MeshComponent(char* name, Mesh mesh, const char* texture_path, GameObject* parent, Application* app, bool active = true);
	virtual ~MeshComponent();

	bool Update(float dt);

	void Activate();
	void Deactivate();

	void DisplayComponentMenu();
	bool AddTexture(const char* path);

	Mesh mesh;
private:

	bool to_draw_normals;
	bool to_draw_AABB;

	void PushTexture(uint texture);

	std::string path;
	std::string path_buffer;

	std::string text_path;
	std::string text_path_buffer;


	uint texture;
};

#endif //__MESHCOMPONENT_H__

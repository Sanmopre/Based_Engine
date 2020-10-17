#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"


#define MAX_LIGHTS 8

struct Mesh;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();

	bool Start();
	update_status PreUpdate();
	update_status Update(float dt);
	update_status PostUpdate();
	bool CleanUp();

	void OnResize(int width, int height);
	void DrawMesh(Mesh* mesh);

public:

	float vertices[24] = { -3.4494 ,0.0000 ,7.2961 ,-3.4494 ,0.0000,-14.5765,9.4740 ,0.0000 ,-14.5765 ,9.4740 ,0.0000 ,7.2961 ,-3.4494 ,11.0152 ,7.2961 ,9.4740 ,11.0152 ,7.2961 ,9.4740 ,11.0152 ,-14.5765 ,-3.4494 ,11.0152 ,-14.5765 };
	int indices[36] = { 1, 2, 3, 3, 4, 1, 5, 6, 7, 7, 8, 5, 1, 4, 6, 6, 5, 1, 4, 3, 7, 7, 6, 4, 3, 2, 8, 8, 7, 3, 2, 1, 5, 5, 8, 2 };

	uint my_id = 0;
	uint my_indices = 0;
	int num_vertices = 8;
	int num_indices = 12;
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
};
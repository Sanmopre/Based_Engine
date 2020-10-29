#pragma once
#include "Module.h"

#include "Light.h"

#define MAX_LIGHTS 8

struct PhysBody3D;

class Mesh;
class B_Plane;

class Renderer3D : public Module
{
public:
	Renderer3D(Application* app, bool start_enabled = true);
	~Renderer3D();

	bool Init();

	bool Start();
	update_status PreUpdate();
	update_status Update(float dt);
	update_status PostUpdate();
	bool CleanUp();

	void OnResize(int width, int height);

	void GenerateFrameBuffers(int width, int height);

	void BeginDebugMode();
	void EndDebugMode();

	void BeginDrawMode();
	void EndDrawMode();
	void ActivateMeshNormals(bool c);

	void AddMesh(MESH*);
	void DeleteMesh(MESH*);

	update_status Draw();
	update_status WireframeDraw();

public:

	std::vector<MESH*> meshes;

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	
	uint frameBuffer;
	uint texColorBuffer;
	uint depthBuffer;

	bool wireframe_mode = true;
	bool show_normals = false;

private:

	B_Plane* plane;
};
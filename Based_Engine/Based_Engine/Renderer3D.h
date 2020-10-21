#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
struct PhysBody3D;


#define MAX_LIGHTS 8

struct Mesh;
class Plane;

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

	update_status Draw();
	update_status WireframeDraw();

public:

	std::vector<Mesh> meshes;

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

};
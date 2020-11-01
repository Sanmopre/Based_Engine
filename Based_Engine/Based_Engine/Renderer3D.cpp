#include <GL/glew.h>

#include "Application.h"
#include "Renderer3D.h"
#include "Window.h"
#include "Camera3D.h"
#include "SDL_opengl.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "Simp.h"
#include "Mesh.h"

#include "scene.h"
#include "Primitive.h"
#include "postprocess.h"
#include "TextureLoader.h"

#include <gl/GL.h>
#include <gl/GLU.h>

Renderer3D::Renderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

// Destructor
Renderer3D::~Renderer3D()
{}

// Called before render is available
bool Renderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;

	//Create context
	context = SDL_GL_CreateContext(App->window->window);

	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	//Initialize assimp debbuger
	Simp::InitializeDebugger();

	TextureLoader::Init();

	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}

	return ret;
}

bool Renderer3D::Start()
{
	//Main scene plane
	plane = new B_Plane(0, 1, 0, 0);
	plane->axis = true;
	return true;
}

// PreUpdate: clear buffer
update_status Renderer3D::PreUpdate()
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_FRAMEBUFFER | GL_RENDERBUFFER);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

update_status Renderer3D::Update(float dt)
{
	BeginDrawMode();
	if (wireframe_mode) 
	{
		BeginDebugMode();	
		WireframeDraw();		
		plane->Render();
		
		EndDebugMode();
	}
	else
	{
		plane->Render();
		Draw();
	}
	EndDrawMode();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status Renderer3D::PostUpdate()
{
	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool Renderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);
	Simp::CleanDebugger();
	meshes.clear();
	return true;
}



void Renderer3D::OnResize(int width, int height)
{
	if (width == 0 || height == 0)
		return;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, width / height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glViewport(0, 0, width, height);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	GenerateFrameBuffers(width, height);
}

void Renderer3D::GenerateFrameBuffers(int width, int height)
{
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	//Generate the depth buffer
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer3D::BeginDebugMode()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Renderer3D::EndDebugMode()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer3D::BeginDrawMode()
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);


}

void Renderer3D::EndDrawMode()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer3D::ActivateMeshNormals(bool c)
{
	for (uint i = 0; i < meshes.size(); i++)
		for (uint n = 0; n < meshes[i]->size(); n++)
			(*meshes[i])[n].drawnormals = c;
}

void Renderer3D::AddMesh(MESH* mesh)
{
	meshes.push_back(mesh);
}

void Renderer3D::DeleteMesh(MESH* mesh)
{
	for (std::vector<MESH*>::iterator itr = meshes.begin(); itr != meshes.end(); itr++)
		if (*itr == mesh)
		{
			meshes.erase(itr);
			break;
		}
}

update_status Renderer3D::Draw()
{
	for (uint i = 0; i < meshes.size(); i++)
		for (uint n = 0; n < meshes[i]->size(); n++)
			(*meshes[i])[n].Render(true);

	return UPDATE_CONTINUE;
}

update_status Renderer3D::WireframeDraw()
{
	for (uint i = 0; i < meshes.size(); i++)
		for (uint n = 0; n < meshes[i]->size(); n++)
			(*meshes[i])[n].Render(true);
	
	return UPDATE_CONTINUE;
}



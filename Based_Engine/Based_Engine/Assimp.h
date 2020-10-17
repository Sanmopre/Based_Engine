#include "Module.h"
#include "cimport.h"
#include "scene.h"
#include "postprocess.h"
#include "Globals.h"
#include "MeshClass.h"
#include "Application.h"
#include "ModuleRenderer3D.h"


namespace Simp {

    std::vector<Mesh*> mesh_vec;

	void InitializeDebugger() {
		struct aiLogStream stream;
		stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
		aiAttachLogStream(&stream);
	}

	void CleanDebugger() {
		aiDetachAllLogStreams();
	}

	void LoadFile(char* file_path) {
		const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
		if (scene != nullptr && scene->HasMeshes()) {

            for (int i = 0; i < scene->mNumMeshes; i++)
            {
                Mesh* mesh = new Mesh();

                mesh->buffersId[Mesh::vertex] = 0;
                mesh->buffersId[Mesh::index] = 0;


                mesh->buffersSize[Mesh::vertex] = scene->mMeshes[i]->mNumVertices;
                mesh->vertices = new float[mesh->buffersSize[Mesh::vertex] * 3];
                memcpy(mesh->vertices, scene->mMeshes[i]->mVertices, sizeof(float) * mesh->buffersSize[Mesh::vertex] * 3);
                LOG("New mesh with %d vertices", mesh->buffersSize[Mesh::vertex]);

                if (scene->mMeshes[i]->HasFaces())
                {
                    mesh->buffersSize[Mesh::index] = scene->mMeshes[i]->mNumFaces * 3;
                    mesh->indices = new uint[mesh->buffersSize[Mesh::index]];
                    for (uint f = 0; f < scene->mMeshes[i]->mNumFaces; ++f)
                    {
                        if (scene->mMeshes[i]->mFaces[f].mNumIndices != 3)
                        {
                            LOG("WARNING, geometery face with != 3 indices!");
                        }
                        else
                        {
                            memcpy(&mesh->indices[f * 3], scene->mMeshes[i]->mFaces[f].mIndices, 3 * sizeof(uint));
                        }

                    }
                }
                
                //VERTEX BUFFER
                glGenBuffers(1, (GLuint*)&(mesh->buffersId[Mesh::vertex]));
                glBindBuffer(GL_ARRAY_BUFFER, mesh->buffersId[Mesh::vertex]);
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->buffersSize[Mesh::vertex] * 3, mesh->vertices, GL_STATIC_DRAW);
                glBindBuffer(GL_ARRAY_BUFFER, 0);

                //INDEX BUFFER
                if (mesh->indices != nullptr)
                {
                    
                    glGenBuffers(1, (GLuint*)&(mesh->buffersId[Mesh::index]));
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->buffersId[Mesh::vertex]);
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->buffersSize[Mesh::index], mesh->indices, GL_STATIC_DRAW);
                    glBindBuffer(GL_ARRAY_BUFFER, 0);
                }

                //MESH VECTOR
               mesh_vec.push_back(mesh);
            }

			aiReleaseImport(scene);
		}
	}

}
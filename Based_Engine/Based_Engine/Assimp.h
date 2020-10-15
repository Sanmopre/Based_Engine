#include "cimport.h"
#include "scene.h"
#include "postprocess.h"
#include "Globals.h"
#include "MeshClass.h"

namespace Simp {

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
			// Use scene->mNumMeshes to iterate on scene->mMeshes array  

            // Use scene->mNumMeshes to iterate on scene->mMeshes array

            for (int i = 0; i < scene->mNumMeshes; i++)
            {
               // Mesh* new_mesh = new Mesh();
            }

			aiReleaseImport(scene);
		}
	}

}
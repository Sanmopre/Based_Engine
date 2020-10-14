#include "cimport.h"
#include "scene.h"
#include "postprocess.h"
#include "Globals.h"

namespace Simp {

	struct VertexData {
		uint id_index = 0; 
		uint num_index = 0;
		uint* index = nullptr;
		uint id_vertex = 0; 
		uint num_vertex = 0;
		float* vertex = nullptr;
	};

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
			aiReleaseImport(scene);
		}
	}

}
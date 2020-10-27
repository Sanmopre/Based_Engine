#include <GL/glew.h>
#include "Globals.h"
#include "Mesh.h"
#include "Application.h"
#include "Renderer3D.h"
#include "Module.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment( lib, "Based_Engine/External/DevIL_Windows_SDK/libx/x86/ReleaseLib/DevIL.lib" )
#pragma comment( lib, "Based_Engine/External/DevIL_Windows_SDK/libx/x86/ReleaseLib/ILU.lib" )
#pragma comment( lib, "Based_Engine/External/DevIL_Windows_SDK/libx/x86/ReleaseLib/ILUT.lib" )

namespace Texture {

	bool Init() {

	//	ilInit();
	//	iluInit();
	//	ilutInit();
		return true;
	}
}
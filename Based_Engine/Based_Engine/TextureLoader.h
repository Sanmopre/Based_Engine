#include <GL/glew.h>
#include "Globals.h"
#include "Mesh.h"
#include "Application.h"
#include "Renderer3D.h"
#include "Module.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "il.h"
#include "ilu.h"
#include "ilut.h"

namespace Texture {

	bool Init() {

		ilInit();
		iluInit();
		ilutInit();
		return true;
	}
}
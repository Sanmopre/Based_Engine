/*
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
#include "TextureLoader.h"

void TextureLod::Init()
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

Texture* TextureLod::Load(const char* path)
{
	Texture* newTexture = nullptr;
	
	uint i;

	ilGenImages(1, &i);
	ilBindImage(i);

	if (ilLoadImage(path))
	{
		ILinfo ImgInfo;
		iluGetImageInfo(&ImgInfo);

		if (ImgInfo.Origin == IL_ORIGIN_UPPER_LEFT)
			iluFlipImage();

		if (ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
		{
			uint id;

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
			glBindTexture(GL_TEXTURE_2D, 0);
			newTexture.id = id;

			newTexture.height = ilGetInteger(IL_IMAGE_HEIGHT);
			newTexture.width = ilGetInteger(IL_IMAGE_WIDTH);
		}

	}
	else
	{
		LOG("Error loading Image %s", path);
	}
	return newTexture;
}*/
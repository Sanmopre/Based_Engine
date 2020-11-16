#include "Globals.h"
#include "ilut.h"
#include "ilu.h"
#include "il.h"
#include "TextureLoader.h"

void TextureLoader::Init()
{
	ilutInit();
	iluInit();
	ilInit();
	ilutRenderer(ILUT_OPENGL);
	LOG("Initialize TextureLoader DevIL")
}

Texture TextureLoader::Load(const char* path)
{
	Texture newTexture = { NULL,NULL,NULL };
	
	uint i = NULL;

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
		ilDeleteImages(1, &i);
	}
	else
	{
		LOG("Error loading Image %s", path);
	}

	LOG("Invalid Texture Path -> %s", path)
	return newTexture;
}

Texture TextureLoader::CheckerTexture()
{
	Texture newTexture;

	GLubyte checkerImage[64][64][4];
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;

			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &newTexture.id);
	glBindTexture(GL_TEXTURE_2D, newTexture.id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);

	glBindTexture(GL_TEXTURE_2D, 0);

	return newTexture;
}

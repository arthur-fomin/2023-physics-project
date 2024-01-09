#pragma once

#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include <ndNewton.h>

// endian conversion
ndUnsigned32 SWAP_INT32(ndUnsigned32 x);
ndUnsigned16 SWAP_INT16(ndUnsigned16 x);
ndUnsigned16 ndIndian16(ndUnsigned16 x);
ndUnsigned32 ndIndian32(ndUnsigned32 x);


void dGetWorkingFileName (const char* const name, char* const outPathName);


enum TextureImageFormat
{
	m_rgb,
	m_rgba,
	m_luminace,
};

void TextureCacheCleanUp();
GLuint GetDefaultTexture();
GLuint LoadTexture(const char* const filename);

GLuint LoadCubeMapTexture(
	const char* const filename_x0, const char* const filename_x1,
	const char* const filename_y0, const char* const filename_y1,
	const char* const filename_z0, const char* const filename_z1);

GLuint AddTextureRef (GLuint texture);
void ReleaseTexture (GLuint texture);

const char* FindTextureById (GLuint textureID);




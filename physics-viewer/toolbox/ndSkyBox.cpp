#include "ndCommon.h"
#include "ndSkyBox.h"
#include "ndLabCamera.h"
#include "ndTargaToOpenGl.h"

ndSkyBox::ndSkyBox(GLuint shader)
	:ndBaseEntity(ndGetIdentityMatrix(), nullptr)
	,m_textureMatrix(ndGetIdentityMatrix())
	,m_shader(shader)
	,m_indexBuffer(0)
	,m_vertexBuffer(0)
	,m_texturecubemap(0)
	,m_vertextArrayBuffer(0)
	,m_matrixUniformLocation(0)
	,m_textureMatrixLocation(0)
{
	GLfloat size = 200.0f;
	GLfloat vertices[] =
	{
		 size, size, size,  -size, size, size,  -size,-size, size,  size,-size, size, // v0,v1,v2,v3 (front)
		 size, size, size,   size,-size, size,   size,-size,-size,  size, size,-size, // v0,v3,v4,v5 (right)
		 size, size, size,   size, size,-size,  -size, size,-size, -size, size, size, // v0,v5,v6,v1 (top)
		-size, size, size,  -size, size,-size,  -size,-size,-size, -size,-size, size, // v1,v6,v7,v2 (left)
		-size,-size,-size,   size,-size,-size,   size,-size, size, -size,-size, size, // v7,v4,v3,v2 (bottom)
		 size,-size,-size,  -size,-size,-size,  -size, size,-size,  size, size,-size  // v4,v7,v6,v5 (back)
	};

	// index array for glDrawElements()
	// A cube requires 36 indices = 6 sides * 2 tris * 3 verts
	ndInt32 indices[] =
	{
		0, 2, 1,   2, 0, 3,     // v0-v1-v2, v2-v3-v0 (front)
		4, 6, 5,   6, 4, 7,    // v0-v3-v4, v4-v5-v0 (right)
		8, 10,9,   10, 8,11,    // v0-v5-v6, v6-v1-v0 (top)
		12,14,13,  14,12,15,    // v1-v6-v7, v7-v2-v1 (left)
		16,18,17,  18,16,19,    // v7-v4-v3, v3-v2-v7 (bottom)
		20,22,21,  22,20,23     // v4-v7-v6, v6-v5-v4 (back)
	};

	ndMatrix texMatrix(ndGetIdentityMatrix());
	texMatrix[1][1] = -1.0f;
	texMatrix[1][3] = size;
	m_textureMatrix = glMatrix(texMatrix);
		
	//SetupCubeMap();
	m_texturecubemap = LoadCubeMapTexture(
		"NewtonSky0003.tga", "NewtonSky0001.tga",
		"NewtonSky0006.tga", "NewtonSky0005.tga",
		"NewtonSky0002.tga", "NewtonSky0004.tga");

	glGenVertexArrays(1, &m_vertextArrayBuffer);
	glBindVertexArray(m_vertextArrayBuffer);
	
	glGenBuffers(1, &m_vertexBuffer); //m_vbo
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	
	// for some reason, this crash in debug mode in window 32 bit 
	// seems to be a problem with glatter
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	
	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);
	
	glBindVertexArray(0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(m_shader);
	m_textureMatrixLocation = glGetUniformLocation(m_shader, "textureMatrix");
	m_matrixUniformLocation = glGetUniformLocation(m_shader, "projectionViewModelMatrix");
	glUseProgram(0);
}

ndSkyBox::~ndSkyBox()
{
	if (m_texturecubemap)
	{
		//glDeleteTextures(1, &m_texturecubemap);
		ReleaseTexture(m_texturecubemap);
	}

	if (m_indexBuffer)
	{
		glDeleteBuffers(1, &m_indexBuffer);
	}
	
	if (m_vertexBuffer)
	{
		glDeleteBuffers(1, &m_vertexBuffer);
	}

	if (m_vertextArrayBuffer)
	{
		glDeleteVertexArrays(1, &m_vertextArrayBuffer);
	}
}

void ndSkyBox::Render(ndFloat32, ndPhysicsViewer* const scene, const ndMatrix&) const
{
	glDepthMask(GL_FALSE);

	ndLabCamera* const camera = scene->GetCamera();
	
	ndMatrix skyMatrix(ndGetIdentityMatrix());
	ndMatrix viewMatrix(camera->GetViewMatrix());
	skyMatrix.m_posit = viewMatrix.UntransformVector(ndVector(0.0f, 0.25f, 0.0f, 1.0f));
	const glMatrix projectionViewModelMatrix(skyMatrix * viewMatrix * camera->GetProjectionMatrix());
	
	glUseProgram(m_shader);
	glUniformMatrix4fv(m_textureMatrixLocation, 1, false, &m_textureMatrix[0][0]);
	glUniformMatrix4fv(m_matrixUniformLocation, 1, false, &projectionViewModelMatrix[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texturecubemap);
	glBindVertexArray(m_vertextArrayBuffer);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	glUseProgram(0);
	glDepthMask(GL_TRUE);
}

#pragma once
#include "ndBaseEntity.h"

class ndPhysicsViewer;

class ndSkyBox: public ndBaseEntity
{
	public:
	ndSkyBox(GLuint shader);
	~ndSkyBox();

	virtual void Render(ndFloat32 timeStep, ndPhysicsViewer* const scene, const ndMatrix& matrix) const;

	private:
	glMatrix m_textureMatrix;
	GLuint m_shader;
	GLuint m_indexBuffer;
	GLuint m_vertexBuffer;
	GLuint m_texturecubemap;
	GLuint m_vertextArrayBuffer;
	GLint m_matrixUniformLocation;
	GLint m_textureMatrixLocation;
};



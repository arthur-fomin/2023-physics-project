#pragma once
#include "ndDemoMeshInterface.h"

//class ndDemoMesh;
//class ndBaseEntity;
class ndShaderCache;
//class ndPhysicsViewer;


class ndFlatShadedDebugMesh: public ndDemoMeshInterface
{
	public:
	ndFlatShadedDebugMesh(const ndShaderCache& shaderCache, const ndShapeInstance* const collision);
	~ndFlatShadedDebugMesh();

	void SetColor(const ndVector& color)
	{
		m_color = color;
	}
	void Render(ndPhysicsViewer* const scene, const ndMatrix& modelMatrix);

	ndVector m_color;
	ndInt32 m_indexCount;
	ndInt32 m_shadeColorLocation;
	ndInt32 m_normalMatrixLocation;
	ndInt32 m_projectMatrixLocation;
	ndInt32 m_viewModelMatrixLocation;

	GLuint m_shader;
	GLuint m_vertexBuffer;
	GLuint m_vertextArrayBuffer;
	GLuint m_triangleIndexBuffer;
};

class ndWireFrameDebugMesh: public ndDemoMeshInterface
{
	public:
	ndWireFrameDebugMesh(const ndShaderCache& shaderCache, const ndShapeInstance* const collision, ndShapeDebugNotify::ndEdgeType edgeType = ndShapeDebugNotify::ndEdgeType::m_shared);
	~ndWireFrameDebugMesh();

	void SetColor(const ndVector& color)
	{
		m_color = color;
	}

	void Render(ndPhysicsViewer* const scene, const ndMatrix& modelMatrix);

	ndVector m_color;
	ndInt32 m_indexCount;
	ndInt32 m_shadeColorLocation;
	ndInt32 m_projectionViewModelMatrixLocation;

	GLuint m_shader;
	GLuint m_vertexBuffer;
	GLuint m_vertextArrayBuffer;
	GLuint m_lineIndexBuffer;
};




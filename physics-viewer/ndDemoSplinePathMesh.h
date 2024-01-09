#include "ndDemoMeshInterface.h"

class ndDemoMesh;
class ndBaseEntity;
class ndShaderCache;
class ndPhysicsViewer;

class ndDemoSplinePathMesh: public ndDemoMeshInterface
{
	public:
	ndDemoSplinePathMesh(const ndBezierSpline& curve, const ndShaderCache& shaderCache, ndInt32 resolution);
	~ndDemoSplinePathMesh();

	void SetColor(const ndVector& color);
	ndInt32 GetRenderResolution() const;
	void SetRenderResolution(ndInt32 breaks);

	virtual void Render(ndPhysicsViewer* const scene, const ndMatrix& modelMatrix);
	
	ndBezierSpline m_curve;
	ndVector m_color;
	ndInt32 m_renderResolution;
	GLuint m_shader;
	GLuint m_vertexBuffer;
	GLuint m_vertextArrayBuffer;
	GLuint m_shadeColorLocation;
	GLuint m_projectionViewModelMatrixLocation;
};




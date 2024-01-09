#pragma once
#include "ndDemoMeshInterface.h"

class ndDemoMesh;
class ndBaseEntity;
class ndShaderCache;
class ndPhysicsViewer;

class ndDemoMesh: public ndDemoMeshInterface, public ndList<ndDemoSubMesh>
{
	public:
	ndDemoMesh(const char* const name);
	ndDemoMesh(const ndDemoMesh& mesh, const ndShaderCache& shaderCache);
	ndDemoMesh(const char* const name, ndMeshEffect* const meshNode, const ndShaderCache& shaderCache);
	ndDemoMesh(const char* const name, const ndShaderCache& shaderCache, const ndShapeInstance* const collision, const char* const texture0, const char* const texture1, const char* const texture2, ndFloat32 opacity = 1.0f, const ndMatrix& uvMatrix = ndGetIdentityMatrix(), bool stretchMaping = true);

	ndDemoSubMesh* AddSubMesh();
	virtual const char* GetTextureName (const ndDemoSubMesh* const subMesh) const;

	virtual void RenderNormals();
	virtual void Render (ndPhysicsViewer* const scene, const ndMatrix& modelMatrix);
	virtual void RenderTransparency(ndPhysicsViewer* const scene, const ndMatrix& modelMatrix);
	void OptimizeForRender(const glPositionNormalUV* const points, ndInt32 pointCount,
						   const ndInt32* const indices, ndInt32 indexCount);
	void GetVertexArray(ndArray<ndVector>& points) const;
	void GetIndexArray(ndArray<ndInt32>& indexList) const;

	protected:
	virtual ~ndDemoMesh();
	void ResetOptimization();
	void RenderGeometry(ndPhysicsViewer* const scene, const ndMatrix& modelMatrix);

	ndInt32 m_indexCount;
	ndInt32 m_vertexCount;
	ndInt32 m_textureLocation;
	ndInt32 m_transparencyLocation;
	ndInt32 m_normalMatrixLocation;
	ndInt32 m_projectMatrixLocation;
	ndInt32 m_viewModelMatrixLocation;
	ndInt32 m_directionalLightDirLocation;

	ndInt32 m_materialAmbientLocation;
	ndInt32 m_materialDiffuseLocation;
	ndInt32 m_materialSpecularLocation;

	GLuint m_shader;
	GLuint m_indexBuffer;
	GLuint m_vertexBuffer;
	GLuint m_vertextArrayBuffer;
	bool m_hasTransparency;

	friend class ndBaseEntity;
	friend class ndDemoSkinMesh;
	friend class ndDemoDebrisRootEntity;
};



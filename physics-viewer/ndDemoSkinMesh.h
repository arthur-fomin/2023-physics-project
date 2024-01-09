#pragma once
#include "ndDemoMesh.h"

class ndDemoMesh;
class ndBaseEntity;
class glSkinVertex;
class ndShaderCache;
class ndPhysicsViewer;

//class ndDemoSkinMesh: public ndDemoMesh
class ndDemoSkinMesh: public ndDemoMeshInterface
{
	public:
	struct dWeightBoneIndex
	{
		ndInt32 m_boneIndex[4];
	};

	ndDemoSkinMesh(const ndDemoSkinMesh& source, ndBaseEntity* const owner);
	ndDemoSkinMesh(ndBaseEntity* const owner, ndMeshEffect* const meshNode, const ndShaderCache& shaderCache);
	~ndDemoSkinMesh();

	protected: 
	virtual ndDemoMeshInterface* Clone(ndBaseEntity* const owner);
	void Render(ndPhysicsViewer* const scene, const ndMatrix& modelMatrix);
	void CreateRenderMesh(
		const glSkinVertex* const points, ndInt32 pointCount,
		const ndInt32* const indices, ndInt32 indexCount);

	ndInt32 CalculateMatrixPalette(ndMatrix* const bindMatrix) const;

	ndSharedPtr<ndDemoMeshInterface> m_shareMesh;
	ndBaseEntity* m_ownerEntity; 
	ndArray<ndMatrix> m_bindingMatrixArray;
	GLuint m_shader;
	ndInt32 m_nodeCount; 
	ndInt32 m_matrixPalette;
};




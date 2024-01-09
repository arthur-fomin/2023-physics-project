#pragma once 
#include "ndDemoMesh.h"
#include "ndBaseEntity.h"

class ndDemoMeshIntance : public ndDemoMesh
{
	public:
	ndDemoMeshIntance(const char* const name, const ndShaderCache& shaderCache, const ndShapeInstance* const collision, const char* const texture0, const char* const texture1, const char* const texture2, ndFloat32 opacity = 1.0f, const ndMatrix& uvMatrix = ndGetIdentityMatrix());
	~ndDemoMeshIntance();

	virtual void Render(ndPhysicsViewer* const scene, const ndMatrix& modelMatrix);
	void SetTransforms(ndInt32 count, const ndMatrix* const matrixArray);

	private:
	void RenderBatch(ndInt32 start, ndPhysicsViewer* const scene, const ndMatrix& modelMatrix);

	const ndMatrix* m_offsets;
	ndInt32 m_instanceCount;
	ndInt32 m_maxInstanceCount;
	GLuint m_matrixOffsetBuffer;
};

class ndDemoInstanceEntity: public ndBaseEntity
{
	public:
	ndDemoInstanceEntity(ndSharedPtr<ndDemoMeshIntance> instanceMesh);
	ndDemoInstanceEntity(const ndDemoInstanceEntity& copyFrom);
	virtual ~ndDemoInstanceEntity(void);

	virtual void Render(ndFloat32 timeStep, ndPhysicsViewer* const scene, const ndMatrix& matrix) const;

	static ndArray<ndMatrix>& GetMatrixStack();

	//ndDemoMeshIntance* m_instanceMesh;
	ndSharedPtr<ndDemoMeshIntance> m_instanceMesh;
};

#pragma once 
#include "ndDemoMesh.h"
#include "ndBaseEntity.h"


class glDebrisPoint
{
	public:
	glVector4 m_posit;
	glVector3 m_normal;
	glUV m_uv;
};

class ndDemoDebrisRootEntity;

class ndDemoDebrisMesh : public ndDemoMesh
{
	public:
	ndDemoDebrisMesh(ndDemoDebrisMesh* const srcMesh, const ndArray<glDebrisPoint>& vertexArray);
	ndDemoDebrisMesh(const char* const name, ndMeshEffect* const meshNode, const ndShaderCache& shaderCache, ndInt32 offsetBase, ndArray<glDebrisPoint>& vertexArray);
	~ndDemoDebrisMesh();

	void Render(ndPhysicsViewer* const scene, const ndMatrix& modelMatrix);

	private:
	ndDemoSubMeshMaterial m_material[2];
	ndInt32 m_textureLocation1;

	friend class ndDemoDebrisRootEntity;
};

class ndDemoDebrisRootEntity: public ndBaseEntity
{
	public:
	ndDemoDebrisRootEntity();
	ndDemoDebrisRootEntity(const ndDemoDebrisRootEntity& copyFrom);
	virtual ~ndDemoDebrisRootEntity(void);

	void FinalizeConstruction(const ndArray<glDebrisPoint>& vertexArray);

	virtual void Render(ndFloat32 timeStep, ndPhysicsViewer* const scene, const ndMatrix& matrix) const;

	//ndInt32 m_vertexCount;
	//ndInt32 m_buffRefCount;
	//GLuint m_vertexBuffer;
	//GLuint m_vertextArrayBuffer;
};

class ndDemoDebrisEntity : public ndBaseEntity
{
	public:
	ndDemoDebrisEntity(ndMeshEffect* const meshNode, ndArray<glDebrisPoint>& vertexArray, ndDemoDebrisRootEntity* const parent, const ndShaderCache& shaderCache);
	ndDemoDebrisEntity(const ndDemoDebrisEntity& copyFrom);
	virtual ~ndDemoDebrisEntity();
	ndBaseEntity* CreateClone() const;

	virtual void Render(ndFloat32 timeStep, ndPhysicsViewer* const scene, const ndMatrix& matrix) const;

	//ndInt32 m_vertexOffsetBase;
};


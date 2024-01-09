#pragma once

#include "ndOpenGlUtil.h"

class ndBaseEntity;
class ndPhysicsViewer;

class ndDemoSubMeshMaterial
{
	public:
	ndDemoSubMeshMaterial();
	~ndDemoSubMeshMaterial();

	GLint GetTexture() const;
	void SetTexture(GLint textureHandle);

	const char* GetTextureName() const;
	void SetTextureName(const char* const name);
	
	glVector4 m_ambient;
	glVector4 m_diffuse;
	glVector4 m_specular;
	GLfloat m_opacity;
	GLfloat m_shiness;
	protected:
	GLint m_textureHandle;
	char  m_textureName[32];
};

class ndDemoSubMesh
{
	public:
	ndDemoSubMesh();
	~ndDemoSubMesh();
	void SetOpacity(ndFloat32 opacity);

	ndDemoSubMeshMaterial m_material;
	ndInt32 m_indexCount;
	ndInt32 m_segmentStart;
	bool m_hasTranparency;
};

class ndDemoMeshInterface : public ndClassAlloc
{
	public:
	ndDemoMeshInterface();
	virtual ~ndDemoMeshInterface();
	const ndString& GetName () const;
	
	bool GetVisible () const;
	void SetVisible (bool visibilityFlag);

	virtual ndDemoMeshInterface* Clone(ndBaseEntity* const) { ndAssert(0); return nullptr; }

	virtual void Render (ndPhysicsViewer* const scene, const ndMatrix& modelMatrix) = 0;
	virtual void RenderTransparency(ndPhysicsViewer* const, const ndMatrix&) {}

	ndString m_name;
	bool m_isVisible;
};





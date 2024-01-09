#pragma once 
#include "ndPhysicsViewer.h"
#include "ndPhysicsUtils.h"

class ndBaseEntity;
class ndAnimKeyframe;
class ndShaderCache;
class ndDemoMeshInterface;

class ndBaseEntity: public ndNodeHierarchy<ndBaseEntity>
{
	public:
	ndBaseEntity(const ndBaseEntity& copyFrom);
	ndBaseEntity(const ndMatrix& matrix, ndBaseEntity* const parent);
	ndBaseEntity(ndPhysicsViewer* const scene, ndMeshEffectNode* const meshEffect);
	virtual ~ndBaseEntity(void);

	static ndBaseEntity* LoadFbx (const char* const filename, ndPhysicsViewer* const scene);

	ndSharedPtr<ndDemoMeshInterface> GetMesh();
	void SetMesh(ndSharedPtr<ndDemoMeshInterface> mesh, const ndMatrix& meshMatrix = ndGetIdentityMatrix());

	const ndMatrix& GetMeshMatrix() const;  
	void SetMeshMatrix(const ndMatrix& matrix);  

	ndBaseEntity* CreateClone () const;

	const ndMatrix& GetRenderMatrix () const;
	ndMatrix CalculateGlobalMatrix (const ndBaseEntity* const root = nullptr) const;

	ndMatrix GetNextMatrix () const;
	ndMatrix GetCurrentMatrix () const;
	ndAnimKeyframe GetCurrentTransform() const;
	virtual void SetMatrix(const ndQuaternion& rotation, const ndVector& position);
	virtual void SetNextMatrix (const ndQuaternion& rotation, const ndVector& position);
	virtual void ResetMatrix(const ndMatrix& matrix);
	virtual void InterpolateMatrix (ndFloat32 param);
	ndMatrix CalculateInterpolatedGlobalMatrix (const ndBaseEntity* const root = nullptr) const;

	void RenderBone(ndPhysicsViewer* const scene, const ndMatrix& nodeMatrix) const;

	ndShapeInstance* CreateCollisionFromChildren() const;
	ndShapeInstance* CreateCompoundFromMesh(bool lowDetail = false);

	void RenderSkeleton(ndPhysicsViewer* const scene, const ndMatrix& matrix) const;
	virtual void Render(ndFloat32 timeStep, ndPhysicsViewer* const scene, const ndMatrix& matrix) const;

	ndBaseEntity* Find(const char* const name) const;
	ndBaseEntity* FindBySubString(const char* const subString) const;

	const ndString& GetName() const;
	void SetName(const ndString& name);

	protected:
	mutable ndMatrix m_matrix;			// interpolated matrix
	ndVector m_curPosition;				// position one physics simulation step in the future
	ndVector m_nextPosition;             // position at the current physics simulation step
	ndQuaternion m_curRotation;          // rotation one physics simulation step in the future  
	ndQuaternion m_nextRotation;         // rotation at the current physics simulation step  

	ndMatrix m_meshMatrix;
	ndSharedPtr<ndDemoMeshInterface> m_mesh;
	ndList <ndBaseEntity*>::ndNode* m_rootNode;
	ndString m_name;
	ndSpinLock m_lock;
	bool m_isDead;
	bool m_isVisible;

	friend class ndPhysicsWorld;
	friend class ndBaseEntityNotify;
	friend class ndPhysicsViewer;
};


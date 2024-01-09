#pragma once 
#include "ndPhysicsViewer.h"
#include "ndPhysicsUtils.h"

class ndBaseEntity;
class ndAnimKeyframe;
class ndShaderCache;
class ndDemoMeshInterface;

class ndBaseEntityNotify: public ndBodyNotify
{
	public:
	D_CLASS_REFLECTION(ndBaseEntityNotify);
	ndBaseEntityNotify(const ndLoadSaveBase::ndLoadDescriptor& desc);
	ndBaseEntityNotify(ndPhysicsViewer* const manager, ndBaseEntity* const entity, ndBodyKinematic* const parentBody = nullptr, ndFloat32 gravity = DEMO_GRAVITY);
	virtual ~ndBaseEntityNotify();

	void* GetUserData() const
	{
		return m_entity;
	}

	virtual void OnObjectPick() const;
	virtual void OnTransform(ndInt32 threadIndex, const ndMatrix& matrix);
	virtual void OnApplyExternalForce(ndInt32 threadIndex, ndFloat32 timestep);

	virtual void Save(const ndLoadSaveBase::ndSaveDescriptor& desc) const;

	void RemoveBody();
	bool CheckInWorld(const ndMatrix& matrix) const
	{
		return matrix.m_posit.m_y > -100.0f;
	}

	ndBaseEntity* m_entity;
	ndBodyKinematic* m_parentBody;
	ndPhysicsViewer* m_manager;
};

class ndBindingRagdollEntityNotify : public ndBaseEntityNotify
{
	public:
	D_CLASS_REFLECTION(ndBindingRagdollEntityNotify);
	ndBindingRagdollEntityNotify(const ndLoadSaveBase::ndLoadDescriptor& desc);
	ndBindingRagdollEntityNotify(ndPhysicsViewer* const manager, ndBaseEntity* const entity, ndBodyDynamic* const parentBody, ndFloat32 campSpeed);

	void OnTransform(ndInt32, const ndMatrix& matrix);
	void OnApplyExternalForce(ndInt32 thread, ndFloat32 timestep);

	virtual void Save(const ndLoadSaveBase::ndSaveDescriptor& desc) const;

	ndMatrix m_bindMatrix;
	ndFloat32 m_capSpeed;
};

#pragma once
#include "ndBaseEntity.h"

class ndLabCamera;
class ndLabCameraPickBodyJoint;

class ndLabCameraManager: public ndClassAlloc
{
	public:
	ndLabCameraManager(ndPhysicsViewer* const scene);
	~ndLabCameraManager();

	ndLabCamera* GetCamera() const 
	{
		return m_camera;
	}

	void SetCameraMatrix (const ndQuaternion& rotation, const ndVector& position);

	void SetCameraMouseLock (bool loockState);

	void SetPickMode(bool mode) {m_pickingMode = mode;}

	void RenderPickedTarget () const;
	void InterpolateMatrices (ndPhysicsViewer* const scene, ndFloat32 timeStepFraction);

	//virtual void FixUpdate(ndPhysicsViewer* const scene, ndFloat32 timestep);
	void FixUpdate(ndPhysicsViewer* const scene, ndFloat32 timestep);

	void ResetPickBody();

	private:
	//virtual void OnBodyDestroy (NewtonBody* const body);
	void UpdatePickBody (ndPhysicsViewer* const scene, bool mouseState, const ndVector& camPos0, const ndVector& camPos1, ndFloat32 timestep); 

	ndVector m_pickedBodyTargetPosition;
	ndVector m_pickedBodyLocalAtachmentPoint;
	ndVector m_pickedBodyLocalAtachmentNormal;

	ndLabCamera* m_camera;
	ndBodyKinematic* m_targetPicked;
	ndSharedPtr<ndJointBilateralConstraint> m_pickJoint;
	ndFloat32 m_mousePosX;
	ndFloat32 m_mousePosY;
	ndFloat32 m_yaw;
	ndFloat32 m_pitch;
	ndFloat32 m_yawRate;
	ndFloat32 m_pitchRate;
	ndFloat32 m_frontSpeed;
	ndFloat32 m_sidewaysSpeed;
	ndFloat32 m_pickedBodyParam;
	
	bool m_prevMouseState;	
	bool m_mouseLockState;
	bool m_pickingMode;

	friend class ndLabCamera;
};



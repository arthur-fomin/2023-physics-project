#pragma once 
#include "ndBaseEntity.h"

class ndLabCamera: public ndBaseEntity
{
	public:
	ndLabCamera();
	~ndLabCamera();

	ndFloat32 GetYawAngle() const;
	ndFloat32 GetPichAngle() const;

	const ndMatrix& GetViewMatrix() const;
	const ndMatrix& GetProjectionMatrix() const;

	void SetMatrix (const ndQuaternion& rotation, const ndVector& position);
	void SetViewMatrix (ndInt32 width, ndInt32 height);

	virtual void Render(ndFloat32 timeStep, ndPhysicsViewer* const scene, const ndMatrix& matrix) const;

	ndVector ScreenToWorld (const ndVector& screenPoint) const;
	ndVector WorldToScreen (const ndVector& worldPoint) const;

	
	private:
	ndMatrix CreateLookAtMatrix(const ndVector& eye, const ndVector& center, const ndVector& normUp);
	ndMatrix CreateMatrixFromFrustum(ndFloat32 Left, ndFloat32 Right, ndFloat32 Bottom, ndFloat32 Top, ndFloat32 ZNear, ndFloat32 ZFar);
	ndMatrix CreatePerspectiveMatrix(ndFloat32 FOV, ndFloat32 Aspect, ndFloat32 ZNear, ndFloat32 ZFar);

	ndMatrix m_viewMatrix;
	ndMatrix m_projectionMatrix;

	ndFloat32 m_fov;
	ndFloat32 m_backPlane;
	ndFloat32 m_frontPlane;
	ndFloat32 m_cameraYaw;
	ndFloat32 m_cameraPitch;

	GLint m_viewport[4]; 
	// friend class ndBaseEntity;
};

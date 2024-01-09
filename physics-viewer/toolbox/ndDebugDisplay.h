#pragma once

class ndPhysicsViewer;

class ndDebugNotify : public ndShapeDebugNotify
{
	public:
	ndDebugNotify(ndPhysicsViewer* const manager = nullptr, ndBodyKinematic* const body = nullptr)
		:ndShapeDebugNotify()
		,m_body(body)
		,m_manager(manager)
	{
	}

	void DrawPolygon(ndInt32, const ndVector* const, const ndEdgeType* const)
	{
	}

	ndBodyKinematic* m_body;
	ndPhysicsViewer* m_manager;
	
};

void RenderParticles(ndPhysicsViewer* const scene);
void RenderWorldScene(ndPhysicsViewer* const scene);
void RenderBodiesAABB(ndPhysicsViewer* const scene);
void RenderBodyFrame(ndPhysicsViewer* const scene);
void RenderCenterOfMass(ndPhysicsViewer* const scene);
void RenderContactPoints(ndPhysicsViewer* const scene);
void RenderJointsDebugInfo(ndPhysicsViewer* const scene);
void RenderModelsDebugInfo(ndPhysicsViewer* const scene);
void RenderNormalForces(ndPhysicsViewer* const scene, ndFloat32 scale = 0.005f);
void RenderPolygon(ndPhysicsViewer* const scene, const ndVector* const points, ndInt32 count, const ndVector& color);



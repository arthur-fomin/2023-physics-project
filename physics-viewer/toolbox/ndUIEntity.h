#pragma once
#include "ndBaseEntity.h"

class ndPhysicsViewer;

class ndUIEntity: public ndBaseEntity
{
	public:
	ndUIEntity(ndPhysicsViewer* const scene);
	virtual ~ndUIEntity();

	virtual void RenderUI() = 0;
	virtual void RenderHelp() = 0;

	ndPhysicsViewer* m_scene;
};
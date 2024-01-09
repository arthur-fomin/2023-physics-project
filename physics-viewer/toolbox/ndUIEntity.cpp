#include "ndNewton.h"
#include "ndUIEntity.h"

ndUIEntity::ndUIEntity(ndPhysicsViewer* const scene)
	:ndBaseEntity(ndGetIdentityMatrix(), nullptr)
	,m_scene(scene)
{
}

ndUIEntity::~ndUIEntity()
{
}

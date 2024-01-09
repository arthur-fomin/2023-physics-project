#include "ndDemoMesh.h"
#include "ndLabCamera.h"
#include "ndPhysicsWorld.h"
#include "ndContactCallback.h"
#include "ndBaseEntityNotify.h"
#include "ndPhysicsViewer.h"
#include "ndLabCameraManager.h"
#include "ndDemoMeshInterface.h"

#define MAX_PHYSICS_STEPS			1
#define MAX_PHYSICS_FPS				60.0f
//#define MAX_PHYSICS_RECOVER_STEPS	2

class ndPhysicsWorldSettings : public ndWordSettings
{
	public:
	D_CLASS_REFLECTION(ndPhysicsWorldSettings);

	ndPhysicsWorldSettings(ndPhysicsWorld* const world)
		:ndWordSettings()
		,m_cameraMatrix(ndGetIdentityMatrix())
		,m_world(world)
	{
	}

	ndPhysicsWorldSettings(const ndLoadSaveBase::ndLoadDescriptor& desc)
		:ndWordSettings(ndLoadSaveBase::ndLoadDescriptor(desc))
		,m_world(nullptr)
	{
	}

	virtual void Load(const ndLoadSaveBase::ndLoadDescriptor& desc)
	{
		ndLoadSaveBase::ndLoadDescriptor childDesc(desc);
		ndWordSettings::Load(childDesc);
		
		// load application specific settings here
		m_cameraMatrix = xmlGetMatrix(desc.m_rootNode, "cameraMatrix");
	}

	virtual void Save(const ndLoadSaveBase::ndSaveDescriptor& desc) const
	{
		nd::TiXmlElement* const childNode = new nd::TiXmlElement(ClassName());
		desc.m_rootNode->LinkEndChild(childNode);
		ndWordSettings::Save(ndLoadSaveBase::ndSaveDescriptor(desc, childNode));

		ndPhysicsViewer* const manager = m_world->GetManager();
		ndLabCamera* const camera = manager->GetCamera();

		ndMatrix cameraMatrix (camera->GetCurrentMatrix());
		xmlSaveParam(childNode, "description", "string", "this scene was saved from Newton 4.0 sandbox demos");
		xmlSaveParam(childNode, "cameraMatrix", cameraMatrix);
	}
	
	ndMatrix m_cameraMatrix;
	ndPhysicsWorld* m_world;
};

D_CLASS_REFLECTION_IMPLEMENT_LOADER(ndPhysicsWorldSettings);

ndPhysicsWorld::ndDefferentDeleteEntities::ndDefferentDeleteEntities(ndPhysicsViewer* const manager)
	:ndArray<ndBaseEntity*>()
	,m_manager(manager)
	,m_renderThreadId(std::this_thread::get_id())
{
}

void ndPhysicsWorld::ndDefferentDeleteEntities::Update()
{
	for (ndInt32 i = 0; i < GetCount(); ++i)
	{
		RemoveEntity((*this)[i]);
	}
	SetCount(0);
}

void ndPhysicsWorld::ndDefferentDeleteEntities::RemoveEntity(ndBaseEntity* const entity)
{
	ndAssert(entity->m_rootNode);
	if (m_renderThreadId == std::this_thread::get_id())
	{
		m_manager->RemoveEntity(entity);
		delete entity;
	}
	else
	{
		ndScopeSpinLock lock(entity->m_lock);
		if (!entity->m_isDead)
		{
			entity->m_isDead = true;
			PushBack(entity);
		}
	}
}

ndPhysicsWorld::ndPhysicsWorld(ndPhysicsViewer* const manager)
	:ndWorld()
	,m_manager(manager)
	// ,m_soundManager(new ndSoundManager(manager))
	,m_timeAccumulator(0.0f)
	,m_deadEntities(manager)
{
	ClearCache();
	SetContactNotify(new ndContactCallback);
}

ndPhysicsWorld::~ndPhysicsWorld()
{
	CleanUp();
}

void ndPhysicsWorld::CleanUp()
{
	ndWorld::CleanUp();
}

void ndPhysicsWorld::RemoveEntity(ndBaseEntity* const entity)
{
	ndAssert(entity->m_rootNode);
	m_deadEntities.RemoveEntity(entity);
}

ndPhysicsViewer* ndPhysicsWorld::GetManager() const
{
	return m_manager;
}

void ndPhysicsWorld::OnPostUpdate(ndFloat32 timestep)
{
	m_manager->m_cameraManager->FixUpdate(m_manager, timestep);
	if (m_manager->m_updateCamera)
	{
		m_manager->m_updateCamera(m_manager, m_manager->m_updateCameraContext, timestep);
	}
}

void ndPhysicsWorld::SaveScene(const char* const path)
{
	ndLoadSave loadScene;
	ndPhysicsWorldSettings setting(this);
	
	loadScene.SaveScene(path, this, &setting);
}

void ndPhysicsWorld::SaveSceneModel(const char* const path)
{
	ndLoadSave loadScene;
	if (m_manager->m_selectedModel)
	{
		loadScene.SaveModel(path, m_manager->m_selectedModel);
	}
}

bool ndPhysicsWorld::LoadScene(const char* const path)
{
	ndAssert(0);
	return true;
}

void ndPhysicsWorld::AdvanceTime(ndFloat32 timestep)
{
	D_TRACKTIME();
	const ndFloat32 descreteStep = (1.0f / MAX_PHYSICS_FPS);

	ndInt32 maxSteps = MAX_PHYSICS_STEPS;
	m_timeAccumulator += timestep;

	// if the time step is more than max timestep par frame, throw away the extra steps.
	if (m_timeAccumulator > descreteStep * (ndFloat32)maxSteps)
	{
		ndFloat32 steps = ndFloor(m_timeAccumulator / descreteStep) - (ndFloat32)maxSteps;
		ndAssert(steps >= 0.0f);
		m_timeAccumulator -= descreteStep * steps;
	}

	while (m_timeAccumulator > descreteStep)
	{
		Update(descreteStep);
		m_timeAccumulator -= descreteStep;
	}
	if (m_manager->m_synchronousPhysicsUpdate)
	{
		Sync();
	}

	m_deadEntities.Update();
}
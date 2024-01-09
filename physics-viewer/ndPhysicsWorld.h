#pragma once
#define MAX_PHYSICS_FPS			60.0f

class ndSoundManager;
class ndPhysicsViewer;

class ndPhysicsWorld: public ndWorld
{
	public:
	class ndDefferentDeleteEntities : public ndArray<ndBaseEntity*>
	{
		public:
		ndDefferentDeleteEntities(ndPhysicsViewer* const manager);

		void Update();
		void RemoveEntity(ndBaseEntity* const entity);

		ndPhysicsViewer* m_manager;
		std::thread::id m_renderThreadId;
	};

	ndPhysicsWorld(ndPhysicsViewer* const manager);
	virtual ~ndPhysicsWorld();
	virtual void CleanUp();

	void AdvanceTime(ndFloat32 timestep);
	ndPhysicsViewer* GetManager() const;
	ndSoundManager* GetSoundManager() const;

	bool LoadScene(const char* const path);
	void SaveScene(const char* const path);
	void SaveSceneModel(const char* const path);

	void RemoveEntity(ndBaseEntity* const entity);

	private:
	void OnPostUpdate(ndFloat32 timestep);

	ndPhysicsViewer* m_manager;
	ndSoundManager* m_soundManager;
	ndFloat32 m_timeAccumulator;
	ndDefferentDeleteEntities m_deadEntities;
};


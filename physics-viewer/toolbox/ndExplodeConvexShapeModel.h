#pragma once

class ndDemoMesh;
class ndDemoDebrisEntity;
class ndPhysicsViewer;
class ndDemoDebriEntityRoot;
class ndDemoDebrisRootEntity;

class ndExplodeConvexShapeModel: public ndModel
{
	class ndAtom
	{
		public:
		ndAtom();
		ndAtom(const ndAtom& atom);
		~ndAtom();

		ndVector m_centerOfMass;
		ndVector m_momentOfInertia;
		ndDemoDebrisEntity* m_debriEnt;
		ndShapeInstance* m_collision;
		ndFloat32 m_massFraction;
	};

	public:
	class ndDesc
	{
		public:
		ndDesc()
			:m_pointCloud()
			,m_shape(nullptr)
			,m_outTexture(nullptr)
			,m_innerTexture(nullptr)
			,m_breakImpactSpeed(10.0f)
		{
		}

		ndArray<ndVector> m_pointCloud;
		ndShapeInstance* m_shape;
		const char* m_outTexture;
		const char* m_innerTexture; 
		ndFloat32 m_breakImpactSpeed;
	};

	class ndEffect : public ndList<ndAtom>
	{
		public:
		ndEffect(ndExplodeConvexShapeModel* const manager, const ndDesc& desc);
		ndEffect(const ndEffect& effect);
		~ndEffect();

		private:
		ndSharedPtr<ndBody> m_body;
		ndShapeInstance* m_shape;
		ndSharedPtr<ndDemoMeshInterface> m_visualMesh;
		ndDemoDebrisRootEntity* m_debrisRootEnt;
		ndFloat32 m_breakImpactSpeed;

		friend ndExplodeConvexShapeModel;
	};

	public:
	ndExplodeConvexShapeModel(ndPhysicsViewer* const scene);
	~ndExplodeConvexShapeModel();

	void AddEffect(const ndEffect& effect, ndFloat32 mass, const ndMatrix& location);

	virtual void Update(ndWorld* const world, ndFloat32 timestep);
	virtual void PostUpdate(ndWorld* const world, ndFloat32 timestep);

	void UpdateEffect(ndWorld* const world, ndEffect& effect);

	ndList<ndEffect> m_effectList;
	ndList<ndEffect> m_pendingEffect;
	ndPhysicsViewer* m_scene;
	ndSpinLock m_lock;
};


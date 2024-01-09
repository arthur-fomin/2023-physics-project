#pragma once 
#define DEMO_GRAVITY  ndFloat32(-10.0f)
//#define DEMO_GRAVITY  ndFloat32(0.0f)

class ndPhysicsViewer;

ndBodyKinematic* AddSphere(ndPhysicsViewer* const scene, const ndMatrix& origin, ndFloat32 mass, ndFloat32 radius, const char* const textName = "wood_0.tga");
ndBodyKinematic* AddBox(ndPhysicsViewer* const scene, const ndMatrix& origin, ndFloat32 mass, ndFloat32 sizex, ndFloat32 sizey, ndFloat32 sizez, const char* const textName = "wood_0.tga");
ndBodyKinematic* AddCapsule(ndPhysicsViewer* const scene, const ndMatrix& origin, ndFloat32 mass, ndFloat32 radius0, ndFloat32 radius1, ndFloat32 high, const char* const textName = "wood_0.tga");
ndBodyKinematic* AddConvexHull(ndPhysicsViewer* const scene, const ndMatrix& origin, ndFloat32 mass, ndFloat32 radius, ndFloat32 high, ndInt32 segments, const char* const textName = "wood_0.tga");
ndBodyKinematic* CreateBody(ndPhysicsViewer* const scene, const ndShapeInstance& shape, const ndMatrix& origin, ndFloat32 mass, const char* const textName = "wood_0.tga");

void AddPlanks(ndPhysicsViewer* const scene, const ndMatrix& origin, ndFloat32 mass, ndInt32 count);
void AddCapsulesStacks(ndPhysicsViewer* const scene, const ndMatrix& origin, ndFloat32 mass, ndFloat32 radius0, ndFloat32 radius1, ndFloat32 high, ndInt32 rows_x, ndInt32 rows_z, ndInt32 columHigh);

ndVector FindFloor(const ndWorld& world, const ndVector& origin, ndFloat32 dist);
ndBodyKinematic* MousePickBody(ndWorld* const nWorld, const ndVector& origin, const ndVector& end, ndFloat32& paramter, ndVector& positionOut, ndVector& normalOut);

class ndParamMapper
{
	public:
	ndParamMapper()
		:m_x0(0.0f)
		,m_scale(0.0f)
	{
	}

	ndParamMapper(ndFloat32 x0, ndFloat32 x1)
		:m_x0(x0 + (x1 - x0) * 0.5f)
		,m_scale((x1 - x0) * 0.5f)
	{
	}

	ndFloat32 Interpolate(const ndFloat32 t)
	{
		return m_x0 + m_scale * t;
	}

	ndFloat32 CalculateParam(const ndFloat32 value) const
	{
		return (value - m_x0) / m_scale;
	}

	ndFloat32 m_x0;
	ndFloat32 m_scale;
};

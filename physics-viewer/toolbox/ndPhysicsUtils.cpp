#include <ndNewton.h>
#include "ndDemoMesh.h"
#include "ndBaseEntity.h"
#include "ndPhysicsUtils.h"
#include "ndPhysicsViewer.h"
#include "ndOpenGlUtil.h"
#include "ndDebugDisplay.h"
#include "ndPhysicsWorld.h"
#include "ndBaseEntityNotify.h"
#include "ndPhysicsViewer.h"
#include "ndDemoInstanceEntity.h"
#include "ndHighResolutionTimer.h"

ndVector FindFloor(const ndWorld& world, const ndVector& origin, ndFloat32 dist)
{
	ndVector p0(origin);
	ndVector p1(origin - ndVector(0.0f, ndAbs(dist), 0.0f, 0.0f));

	ndRayCastClosestHitCallback rayCaster;
	if (world.RayCast(rayCaster, p0, p1))
	{
		return rayCaster.m_contact.m_point;
	}
	return p1;
}

ndBodyKinematic* MousePickBody(ndWorld* const world, const ndVector& origin, const ndVector& end, ndFloat32& paramterOut, ndVector& positionOut, ndVector& normalOut)
{
	class ndRayPickingCallback: public ndRayCastClosestHitCallback
	{
		public: 
		ndRayPickingCallback()
			:ndRayCastClosestHitCallback()
		{
		}

		ndFloat32 OnRayCastAction(const ndContactPoint& contact, ndFloat32 intersetParam)
		{
			if (contact.m_body0->GetInvMass() == ndFloat32(0.0f)) 
			{
				return 1.2f;
			}
			return ndRayCastClosestHitCallback::OnRayCastAction(contact, intersetParam);
		}
	};

	ndRayPickingCallback rayCaster;
	if (world->RayCast(rayCaster, origin, end))
	{
		paramterOut = rayCaster.m_param;
		positionOut = rayCaster.m_contact.m_point;
		normalOut = rayCaster.m_contact.m_normal;
		return (ndBodyKinematic* )rayCaster.m_contact.m_body0;
	}

	return nullptr;
}

void AddPlanks(ndPhysicsViewer* const scene, const ndMatrix& location, ndFloat32 mass, ndInt32 count)
{
	ndMatrix matrix(location);
	for (ndInt32 i = 0; i < count; ++i)
	{
		for (ndInt32 j = 0; j < count; ++j)
		{
			matrix.m_posit = location.m_posit + ndVector(((ndFloat32)i - 2)* 5.0f, 0.0f, ((ndFloat32)j - 2) * 5.0f, 0.0f);
			AddBox(scene, matrix, mass, 4.0f, 0.25f, 3.0f);
		}
	}
}

static void AddShape(ndPhysicsViewer* const scene,
	ndDemoInstanceEntity* const rootEntity, const ndShapeInstance& shape,
	ndFloat32 mass, const ndMatrix& location, const ndFloat32 high, ndInt32 count)
{
	ndMatrix matrix(location);
	ndPhysicsWorld* const world = scene->GetWorld();

	ndVector floor(FindFloor(*world, matrix.m_posit + ndVector(0.0f, 100.0f, 0.0f, 0.0f), 200.0f));
	matrix.m_posit.m_y = floor.m_y + high + 7.0f;

	for (ndInt32 i = 0; i < count; ++i)
	{
		ndBodyKinematic* const body = new ndBodyDynamic();
		ndBaseEntity* const entity = new ndBaseEntity(matrix, rootEntity);

		body->SetNotifyCallback(new ndBaseEntityNotify(scene, entity));
		body->SetMatrix(matrix);
		body->SetCollisionShape(shape);
		body->SetMassMatrix(mass, shape);
		body->SetAngularDamping(ndVector(ndFloat32(0.5f)));
		
		ndSharedPtr<ndBody> bodyPtr(body);
		world->AddBody(bodyPtr);
		matrix.m_posit.m_y += high * 2.5f;
	}
}

void AddCapsulesStacks(ndPhysicsViewer* const scene, const ndMatrix& location, ndFloat32 mass, ndFloat32 radius0, ndFloat32 radius1, ndFloat32 high, ndInt32 rows_x, ndInt32 rows_z, ndInt32 columHigh)
{
	ndShapeInstance shape(new ndShapeCapsule(radius0, radius1, high));
	ndSharedPtr<ndDemoMeshIntance> instanceMesh (new ndDemoMeshIntance("shape", scene->GetShaderCache(), &shape, "marble.tga", "marble.tga", "marble.tga"));
	
	ndDemoInstanceEntity* const rootEntity = new ndDemoInstanceEntity(instanceMesh);
	scene->AddEntity(rootEntity);
	
	ndFloat32 spacing = 2.0f;
	ndMatrix matrix(ndRollMatrix(90.0f * ndDegreeToRad));
	for (ndInt32 z = 0; z < rows_z; ++z)
	{
		for (ndInt32 x = 0; x < rows_x; ++x)
		{
			matrix.m_posit = location.m_posit + ndVector ((ndFloat32)(x - rows_x / 2) * spacing, 0.0f, (ndFloat32)(z - rows_z / 2) * spacing, 0.0f);
			AddShape(scene, rootEntity, shape, mass, matrix, high, columHigh);
		}
	}
}

ndBodyKinematic* CreateBody(ndPhysicsViewer* const scene, const ndShapeInstance& shape, const ndMatrix& location, ndFloat32 mass, const char* const textName)
{
	ndPhysicsWorld* const world = scene->GetWorld();

	ndMatrix matrix(location);
	ndVector floor(FindFloor(*world, matrix.m_posit + ndVector(0.0f, 500.0f, 0.0f, 0.0f), 1000.0f));
	matrix.m_posit.m_y = ndMax (floor.m_y + 1.0f, matrix.m_posit.m_y);
	ndSharedPtr<ndDemoMeshInterface> mesh (new ndDemoMesh("shape", scene->GetShaderCache(), &shape, textName, textName, textName));

	ndBodyKinematic* const body = new ndBodyDynamic();
	ndBaseEntity* const entity = new ndBaseEntity(matrix, nullptr);
	entity->SetMesh(mesh);
	body->SetNotifyCallback(new ndBaseEntityNotify(scene, entity));

	body->SetMatrix(matrix);
	body->SetCollisionShape(shape);
	body->SetMassMatrix(mass, shape);

	ndSharedPtr<ndBody> bodyPtr(body);
	world->AddBody(bodyPtr);
	scene->AddEntity(entity);
	return body;
}

ndBodyKinematic* AddBox(ndPhysicsViewer* const scene, const ndMatrix& location, ndFloat32 mass, ndFloat32 sizex, ndFloat32 sizey, ndFloat32 sizez, const char* const textName)
{
	ndShapeInstance shape(new ndShapeBox(sizex, sizey, sizez));
	ndBodyKinematic* const body = CreateBody(scene, shape, location, mass, textName);
	return body;
}

ndBodyKinematic* AddSphere(ndPhysicsViewer* const scene, const ndMatrix& location, ndFloat32 mass, ndFloat32 radius, const char* const textName)
{
	ndShapeInstance shape(new ndShapeSphere(radius));
	ndBodyKinematic* const body = CreateBody(scene, shape, location, mass, textName);
	return body;
}

ndBodyKinematic* AddCapsule(ndPhysicsViewer* const scene, const ndMatrix& location, ndFloat32 mass, ndFloat32 radius0, ndFloat32 radius1, ndFloat32 high, const char* const textName)
{
	ndShapeInstance shape(new ndShapeCapsule(radius0, radius1, high));
	ndBodyKinematic* const body = CreateBody(scene, shape, location, mass, textName);
	return body;
}

ndBodyKinematic* AddConvexHull(ndPhysicsViewer* const scene, const ndMatrix& location, ndFloat32 mass, ndFloat32 radius, ndFloat32 high, ndInt32 segments, const char* const textName)
{
	ndFixSizeArray<ndVector, 1024 * 8> points;
	ndFloat32 den = ndFloat32(segments);
	for (ndInt32 i = 0; i < segments; ++i)
	{
		ndFloat32 angle = ndFloat32(2.0f) * ndPi * ndFloat32(i) / den;
		ndFloat32 x = radius * ndCos(angle);
		ndFloat32 z = radius * ndSin(angle);
		points.PushBack(ndVector(0.7f * x, -high * 0.5f, 0.7f * z, 0.0f));
		points.PushBack(ndVector(0.7f * x,  high * 0.5f, 0.7f * z, 0.0f));
		points.PushBack(ndVector(x, -high * 0.25f, z, 0.0f));
		points.PushBack(ndVector(x, high * 0.25f, z, 0.0f));
	}

	ndShapeInstance shape(new ndShapeConvexHull(points.GetCount(), sizeof(ndVector), 0.0f, &points[0].m_x));
	ndBodyKinematic* const body = CreateBody(scene, shape, location, mass, textName);
	return body;
}

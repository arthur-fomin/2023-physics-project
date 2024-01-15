#include "ndSkyBox.h"
#include "ndDemoMesh.h"
#include "ndLabCamera.h"
#include "ndPhysicsUtils.h"
#include "ndPhysicsWorld.h"
#include "ndMakeStaticMap.h"
#include "ndBaseEntityNotify.h"
#include "ndPhysicsViewer.h"
#include "ndLabCameraManager.h"

class ndAsymetricInertiaBody: public ndBodyDynamic
{
	public:
	ndAsymetricInertiaBody()
		:ndBodyDynamic()
		,m_principalAxis(ndGetIdentityMatrix())
	{
	}
	
	virtual void SetMassMatrix(ndFloat32 mass, const ndMatrix& inertia)
	{
		m_principalAxis = inertia;
		ndVector eigenValues(m_principalAxis.EigenVectors());
		ndMatrix massMatrix(ndGetIdentityMatrix());
		massMatrix[0][0] = eigenValues[0];
		massMatrix[1][1] = eigenValues[1];
		massMatrix[2][2] = eigenValues[2];
		ndBodyDynamic::SetMassMatrix(mass, massMatrix);
	}

	virtual ndMatrix CalculateInvInertiaMatrix() const
	{
		ndMatrix matrix(m_principalAxis * m_matrix);
		matrix.m_posit = ndVector::m_wOne;
		ndMatrix diagonal(ndGetIdentityMatrix());
		diagonal[0][0] = m_invMass[0];
		diagonal[1][1] = m_invMass[1];
		diagonal[2][2] = m_invMass[2];
		return matrix * diagonal * matrix.Inverse();
	}
	
	ndMatrix m_principalAxis;
};

static void RattleBack3(ndPhysicsViewer* const scene, ndFloat32 mass, 
// ndFloat32 angularSpeed, 
const ndVector& origin)
{
	ndMatrix matrix(ndPitchMatrix(15.0f * ndDegreeToRad));
	matrix.m_posit = origin;
	matrix.m_posit.m_w = 1.0f;

	ndPhysicsWorld* const world = scene->GetWorld();
	matrix.m_posit.m_y += 5.0f;

    ndMatrix shapeMatrix(ndYawMatrix(5.0f * ndDegreeToRad));
	ndShapeInstance shape(new ndShapeSphere(1.0f));
	shape.SetLocalMatrix(shapeMatrix);
	shape.SetScale(ndVector(0.3f, 0.25f, 1.0f, 0.0f));

	// ndShapeInstance shape(new ndShapeBox(2.0f, 0.5f, 1.0));

	ndSharedPtr<ndDemoMeshInterface> mesh(new ndDemoMesh("shape", scene->GetShaderCache(), &shape, 
                                                            "camo.tga", "marble.tga", "camo.tga"));

	// // ndSharedPtr<ndDemoMeshInterface> mesh(new ndDemoMesh("shape", scene->GetShaderCache(), &shape, 
    //                                                         "camo.tga", "marble.tga", "camo.tga"));


	// ndVector omega(0.1f, 0.0f, angularSpeed, 0.0f);
	ndBodyKinematic* const body = new ndBodyDynamic();
	ndBaseEntity* const entity = new ndBaseEntity(matrix, nullptr);
	entity->SetMesh(mesh);
    body->SetNotifyCallback(new ndBaseEntityNotify(scene, entity));

	// body->SetNotifyCallback(new ndBaseEntityNotify(scene, entity, nullptr, 0.0f));

	// body->SetOmega(omega);
	body->SetMatrix(matrix);
	body->SetCollisionShape(shape);
	body->SetMassMatrix(mass, shape);

	ndSharedPtr<ndBody> bodyPtr(body);
	world->AddBody(bodyPtr);
	scene->AddEntity(entity);
}



static void RattleBack(ndPhysicsViewer* const scene, ndFloat32 mass, const ndVector& origin)
{
	ndMatrix matrix(ndPitchMatrix(15.0f * ndDegreeToRad));
	matrix.m_posit = origin;
	matrix.m_posit.m_w = 1.0f;

	ndPhysicsWorld* const world = scene->GetWorld();

	ndVector floor(FindFloor(*world, matrix.m_posit + ndVector(0.0f, 100.0f, 0.0f, 0.0f), 200.0f));
	matrix.m_posit.m_y += floor.m_y + 0.4f;

	ndMatrix shapeMatrix(ndYawMatrix(5.0f * ndDegreeToRad));

	ndShapeInstance shape(new ndShapeSphere(1.0f));
	shape.SetLocalMatrix(shapeMatrix);
	shape.SetScale(ndVector(0.3f, 0.25f, 1.0f, 0.0f));

	ndSharedPtr<ndDemoMeshInterface> mesh(new ndDemoMesh("shape", scene->GetShaderCache(), &shape, 
                                                            "marble.tga", "marble.tga", "marble.tga"));

	ndBodyKinematic* const body = new ndBodyDynamic();
	ndBaseEntity* const entity = new ndBaseEntity(matrix, nullptr);
	entity->SetMesh(mesh);
	body->SetNotifyCallback(new ndBaseEntityNotify(scene, entity));

	body->SetMatrix(matrix);
	body->SetCollisionShape(shape);
	body->SetMassMatrix(mass, shape);
	body->SetCentreOfMass(ndVector(0.0f, -0.1f, 0.0f, 0.0f));

	ndSharedPtr<ndBody> bodyPtr(body);
	world->AddBody(bodyPtr);
	scene->AddEntity(entity);
}


static void RattleBackRight(ndPhysicsViewer* const scene, ndFloat32 mass, const ndVector& origin)
{
	ndMatrix matrix(ndPitchMatrix(15.0f * ndDegreeToRad));
	matrix.m_posit = origin;
	matrix.m_posit.m_w = 1.0f;

	ndPhysicsWorld* const world = scene->GetWorld();

	ndVector floor(FindFloor(*world, matrix.m_posit + ndVector(0.0f, 100.0f, 0.0f, 0.0f), 200.0f));
	matrix.m_posit.m_y += floor.m_y + 1.4f;

	ndMatrix shapeMatrix(ndYawMatrix(15.0f * ndDegreeToRad));

	ndShapeInstance shape(new ndShapeSphere(1.0f));
	shape.SetLocalMatrix(shapeMatrix);
	shape.SetScale(ndVector(2*0.3f, 2*0.25f, 2*1.0f, 0.0f));

	ndSharedPtr<ndDemoMeshInterface> mesh(new ndDemoMesh("shape", scene->GetShaderCache(), &shape, 
                                                            "camo.tga", "camo.tga", "camo.tga"));

	ndBodyKinematic* const body = new ndBodyDynamic();
	ndBaseEntity* const entity = new ndBaseEntity(matrix, nullptr);
	entity->SetMesh(mesh);
	body->SetNotifyCallback(new ndBaseEntityNotify(scene, entity));

	body->SetMatrix(matrix);
	body->SetCollisionShape(shape);
	body->SetMassMatrix(mass, shape);
	body->SetCentreOfMass(ndVector(0.0f, -0.5f, 0.0f, 0.0f));

	ndSharedPtr<ndBody> bodyPtr(body);
	world->AddBody(bodyPtr);
	scene->AddEntity(entity);
}


static void RattleBackNoGravity(ndPhysicsViewer* const scene, ndFloat32 mass, const ndVector& origin)
{
	ndMatrix matrix(ndPitchMatrix(15.0f * ndDegreeToRad));
	matrix.m_posit = origin;
	matrix.m_posit.m_w = 1.0f;

	ndPhysicsWorld* const world = scene->GetWorld();

	ndVector floor(FindFloor(*world, matrix.m_posit + ndVector(0.0f, 100.0f, 0.0f, 0.0f), 200.0f));
	matrix.m_posit.m_y += floor.m_y + 4.0f;

	ndMatrix shapeMatrix(ndYawMatrix(5.0f * ndDegreeToRad));

	ndShapeInstance shape(new ndShapeSphere(1.0f));

	shape.SetLocalMatrix(shapeMatrix);
	shape.SetScale(ndVector(0.3f, 0.25f, 1.0f, 0.0f));

	ndSharedPtr<ndDemoMeshInterface> mesh(new ndDemoMesh("shape", scene->GetShaderCache(), &shape, 
                                                            "marble.tga", "marble.tga", "marble.tga"));

	ndBodyKinematic* const body = new ndBodyDynamic();

	ndVector omega(0.0f, 0.0f, 2.0f, 0.0f);
    body->SetOmega(omega);
	ndBaseEntity* const entity = new ndBaseEntity(matrix, nullptr);
	entity->SetMesh(mesh);

    // Отключаем гравитацию.
    body->SetNotifyCallback(new ndBaseEntityNotify(scene, entity, nullptr, 0.0f));

	body->SetMatrix(matrix);
	body->SetCollisionShape(shape);
	body->SetMassMatrix(mass, shape);
	body->SetCentreOfMass(ndVector(0.0f, -0.1f, 0.0f, 0.0f));

	ndSharedPtr<ndBody> bodyPtr(body);
	world->AddBody(bodyPtr);
	scene->AddEntity(entity);
}




static void RattleBack2(ndPhysicsViewer* const scene, ndFloat32 mass, const ndVector& origin)
{
	ndMatrix matrix(ndPitchMatrix(15.0f * ndDegreeToRad));
	matrix.m_posit = origin;
	matrix.m_posit.m_w = 1.0f;

	ndPhysicsWorld* const world = scene->GetWorld();
	matrix.m_posit.m_y += 5.0f;

	// ndVector floor(FindFloor(*world, matrix.m_posit + ndVector(0.0f, 100.0f, 0.0f, 0.0f), 200.0f));
	// matrix.m_posit.m_y += floor.m_y + 3.4f;

	ndMatrix shapeMatrix(ndYawMatrix(5.0f * ndDegreeToRad));

	ndShapeInstance shape(new ndShapeSphere(1.0f));
	shape.SetLocalMatrix(shapeMatrix);
	shape.SetScale(ndVector(0.3f, 0.25f, 1.0f, 0.0f));

	ndSharedPtr<ndDemoMeshInterface> mesh(new ndDemoMesh("shape", scene->GetShaderCache(), &shape, 
                                                            "camo.tga", "marble.tga", "camo.tga"));

	ndBodyKinematic* const body = new ndBodyDynamic();
	ndBaseEntity* const entity = new ndBaseEntity(matrix, nullptr);
	entity->SetMesh(mesh);
	body->SetNotifyCallback(new ndBaseEntityNotify(scene, entity));

	body->SetMatrix(matrix);
	body->SetCollisionShape(shape);
	body->SetMassMatrix(mass, shape);
	// body->SetCentreOfMass(ndVector(0.0f, -0.1f, 0.0f, 0.0f));

	ndSharedPtr<ndBody> bodyPtr(body);
	world->AddBody(bodyPtr);
	scene->AddEntity(entity);
}




void ndBasicLab(ndPhysicsViewer* const scene)
{
	// build a floor
	BuildFloorBox(scene, ndGetIdentityMatrix()); 

	RattleBack(scene, 10.0f, ndVector(15.0f, 0.0f, -4.0f, 0.0f));

    RattleBackNoGravity(scene, 10.0f, ndVector(10.0f, 0.0f, -4.0f, 0.0f));

	RattleBackRight(scene, 10.0f, ndVector(20.0f, 0.0f, -4.0f, 0.0f));

	scene->GetCameraManager()->SetPickMode(true);

	ndQuaternion rot;
	ndVector origin(-10.0f, 5.0f, 0.5f, 1.0f);
	scene->SetCameraMatrix(rot, origin);
}

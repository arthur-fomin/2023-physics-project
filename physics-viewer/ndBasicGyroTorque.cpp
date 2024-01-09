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

static void DzhanibekovEffect(ndPhysicsViewer* const scene, ndFloat32 mass, ndFloat32 angularSpeed, const ndVector& origin)
{
	ndMatrix matrix(ndGetIdentityMatrix());
	matrix.m_posit = origin;
	matrix.m_posit.m_w = 1.0f;

	ndPhysicsWorld* const world = scene->GetWorld();
	matrix.m_posit.m_y += 5.0f;

	ndShapeInstance shape(new ndShapeBox(2.0f, 0.5f, 1.0));
	ndSharedPtr<ndDemoMeshInterface> mesh (new ndDemoMesh("shape", scene->GetShaderCache(), &shape, "marble.tga", "marble.tga", "marble.tga"));

	ndVector omega(0.1f, 0.0f, angularSpeed, 0.0f);
	ndBodyKinematic* const body = new ndBodyDynamic();
	ndBaseEntity* const entity = new ndBaseEntity(matrix, nullptr);
	entity->SetMesh(mesh);
	body->SetNotifyCallback(new ndBaseEntityNotify(scene, entity, nullptr, 0.0f));

	body->SetOmega(omega);
	body->SetMatrix(matrix);
	body->SetCollisionShape(shape);
	body->SetMassMatrix(mass, shape);

	ndSharedPtr<ndBody> bodyPtr(body);
	world->AddBody(bodyPtr);
	scene->AddEntity(entity);
}

static void Phitop(ndPhysicsViewer* const scene, ndFloat32 mass, ndFloat32 angularSpeed, const ndVector& origin)
{
	ndMatrix matrix(ndPitchMatrix(15.0f * ndDegreeToRad));
	matrix.m_posit = origin;
	matrix.m_posit.m_w = 1.0f;

	ndPhysicsWorld* const world = scene->GetWorld();
	matrix.m_posit.m_y += 0.5f;

	ndShapeInstance shape(new ndShapeSphere(1.0f));
	shape.SetScale(ndVector (0.5f, 0.5f, 1.0f, 0.0f));

	ndSharedPtr<ndDemoMeshInterface> mesh(new ndDemoMesh("shape", scene->GetShaderCache(), &shape, "marble.tga", "marble.tga", "marble.tga"));

	ndVector omega(0.0f, angularSpeed, 0.0f, 0.0f);
	ndBodyKinematic* const body = new ndBodyDynamic();
	ndBaseEntity* const entity = new ndBaseEntity(matrix, nullptr);
	entity->SetMesh(mesh);
	body->SetNotifyCallback(new ndBaseEntityNotify(scene, entity));

	body->SetOmega(omega);
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

static void PrecessingTop(ndPhysicsViewer* const scene, const ndVector& origin)
{
	ndPhysicsWorld* const world = scene->GetWorld();

	ndShapeInstance shape(new ndShapeCone(0.7f, 1.0f));
	shape.SetLocalMatrix(ndRollMatrix(-90.0f * ndDegreeToRad));

	ndMatrix matrix(ndPitchMatrix(15.0f * ndDegreeToRad));
	matrix.m_posit = origin;
	matrix.m_posit.m_w = 1.0f;

	ndSharedPtr<ndDemoMeshInterface> mesh(new ndDemoMesh("shape", scene->GetShaderCache(), &shape, "marble.tga", "marble.tga", "marble.tga"));

	ndBaseEntity* const entity = new ndBaseEntity(matrix, nullptr);
	entity->SetMesh(mesh);
	matrix.m_posit.m_y += 1.0f;

	const ndFloat32 mass = 1.0f;
	ndBodyKinematic* const body = new ndBodyDynamic();
	body->SetNotifyCallback(new ndBaseEntityNotify(scene, entity));
	body->SetMatrix(matrix);
	body->SetCollisionShape(shape);
	body->SetMassMatrix(mass, shape);
	body->SetOmega(matrix.m_up.Scale(40.0f));

	ndSharedPtr<ndBody> bodyPtr(body);
	world->AddBody(bodyPtr);
	scene->AddEntity(entity);
}

static void CreateFlyWheel(ndPhysicsViewer* const scene, const ndVector& origin, ndFloat32 mass, ndFloat32 speed, ndFloat32 radius, ndFloat32 lenght, ndFloat32 tiltAnsgle)
{
	ndPhysicsWorld* const world = scene->GetWorld();
	
	ndFloat32 smallRadius = 0.0625f;
	ndShapeInstance rod(new ndShapeCapsule(smallRadius * 0.5f, smallRadius * 0.5f, lenght));
	ndShapeInstance wheel(new ndShapeCylinder(radius, radius, 0.125f));
	
	ndMatrix offset(ndGetIdentityMatrix());
	offset.m_posit.m_x = lenght * 0.5f;
	wheel.SetLocalMatrix(offset);
	
	ndShapeInstance flyWheelShape(new ndShapeCompound());
	ndShapeCompound* const compound = flyWheelShape.GetShape()->GetAsShapeCompound();
	compound->BeginAddRemove();
	compound->AddCollision(&rod);
	compound->AddCollision(&wheel);
	compound->EndAddRemove();
	
	ndMatrix matrix(ndRollMatrix(tiltAnsgle * ndDegreeToRad));
	matrix.m_posit = origin;
	matrix.m_posit.m_y += 5.0f;
	matrix.m_posit.m_w = 1.0f;
	
	ndBaseEntity* const entity = new ndBaseEntity(ndGetIdentityMatrix(), nullptr);
	
	ndSharedPtr<ndDemoMeshInterface> mesh(new ndDemoMesh("primitive", scene->GetShaderCache(), &flyWheelShape, "smilli.tga", "smilli.tga", "smilli.tga"));
	entity->SetMesh(mesh);
	
	ndBodyKinematic* const body = new ndBodyDynamic();
	body->SetNotifyCallback(new ndBaseEntityNotify(scene, entity));
	body->SetMatrix(matrix);
	body->SetCollisionShape(flyWheelShape);
	body->SetMassMatrix(mass, flyWheelShape);
	
	ndVector omega(matrix.m_front.Scale (speed));
	body->SetOmega(omega);
	
	matrix.m_posit -= matrix.m_front.Scale(lenght * 0.5f);
	//ndJointSpherical* const joint = new ndJointSpherical(matrix, *body, world->GetSentinelBody());
	ndSharedPtr<ndJointBilateralConstraint> joint (new ndJointSpherical(matrix, body, world->GetSentinelBody()));
	
	ndSharedPtr<ndBody> bodyPtr(body);
	world->AddBody(bodyPtr);
	world->AddJoint(joint);
	scene->AddEntity(entity);
}

void ndBasicAngularMomentum (ndPhysicsViewer* const scene)
{
	// build a floor
	BuildFloorBox(scene, ndGetIdentityMatrix()); 

	// should spins very slowly, with a tilt angle of 30 degrees
	// CreateFlyWheel(scene, ndVector(15.0f, 0.0f, -12.0f, 0.0f), 10.0f, 50.0f, 0.6f, 0.5f, 30.0f);
	// CreateFlyWheel(scene, ndVector(15.0f, 0.0f, -10.0f, 0.0f), 10.0f, 100.0f, 0.6f, 0.5f, 0.0f);
	// CreateFlyWheel(scene, ndVector(15.0f, 0.0f,  -8.0f, 0.0f), 10.0f, -30.0f, 0.6f, 0.5f, 0.0f);
	
	// DzhanibekovEffect(scene, 10.0f, 5.0f, ndVector(15.0f, 0.0f, -4.0f, 0.0f));
	// DzhanibekovEffect(scene, 10.0f, -5.0f, ndVector(15.0f, 0.0f, 0.0f, 0.0f));
	// DzhanibekovEffect(scene, 10.0f, 10.0f, ndVector(15.0f, 0.0f, 4.0f, 0.0f));
	
	// Phitop(scene, 10.0f,  25.0f, ndVector(10.0f, 0.0f, -6.0f, 0.0f));
	// Phitop(scene, 10.0f, -25.0f, ndVector(10.0f, 0.0f, 0.0f, 0.0f));
	// Phitop(scene, 10.0f,  35.0f, ndVector(10.0f, 0.0f, 6.0f, 0.0f));
	
	// PrecessingTop(scene, ndVector(5.0f, 0.0f, -4.0f, 0.0f));
	// PrecessingTop(scene, ndVector(5.0f, 0.0f, 0.0f, 0.0f));
	PrecessingTop(scene, ndVector(5.0f, 0.0f, 4.0f, 0.0f));
	
	RattleBack(scene, 10.0f, ndVector(0.0f, 0.0f, -4.0f, 0.0f));
	RattleBack(scene, 10.0f, ndVector(0.0f, 0.0f, 0.0f, 0.0f));
	RattleBack(scene, 10.0f, ndVector(0.0f, 0.0f,  4.0f, 0.0f));
	
	scene->GetCameraManager()->SetPickMode(true);

	ndQuaternion rot;
	ndVector origin(-15.0f, 5.0f, 0.0f, 1.0f);
	scene->SetCameraMatrix(rot, origin);
}

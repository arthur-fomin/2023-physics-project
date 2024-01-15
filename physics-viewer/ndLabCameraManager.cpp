#include "ndLabCamera.h"
#include "ndOpenGlUtil.h"
#include "ndPhysicsWorld.h"
#include "ndPhysicsUtils.h"
#include "ndBaseEntityNotify.h"
#include "ndLabCameraManager.h"

//#define D_ENABLE_CAMERA_REPLAY
#ifdef D_ENABLE_CAMERA_REPLAY
	//#define D_RECORD_CAMERA
#endif

class ndLabCameraPickBodyJoint: public ndJointKinematicController
{
	public:
	ndLabCameraPickBodyJoint(ndBodyKinematic* const childBody, ndBodyKinematic* const worldBody, const ndVector& attachmentPointInGlobalSpace, ndLabCameraManager* const camera)
		:ndJointKinematicController(childBody, worldBody, attachmentPointInGlobalSpace)
		,m_manager (camera)
	{
	}
	
	~ndLabCameraPickBodyJoint()
	{
		if (m_manager) 
		{
			m_manager->ResetPickBody();
		}
	}
		
	ndLabCameraManager* m_manager;
};

ndLabCameraManager::ndLabCameraManager(ndPhysicsViewer* const)
	:ndClassAlloc()
	,m_pickedBodyTargetPosition(ndVector::m_wOne)
	,m_pickedBodyLocalAtachmentPoint(ndVector::m_wOne)
	,m_pickedBodyLocalAtachmentNormal(ndVector::m_zero)
	,m_camera(new ndLabCamera())
	,m_targetPicked(nullptr)
	,m_pickJoint(nullptr)
	,m_mousePosX(0)
	,m_mousePosY(0)
	,m_yaw (m_camera->GetYawAngle())
	,m_pitch (m_camera->GetPichAngle())
	,m_yawRate (0.04f)
	,m_pitchRate (0.02f)
	,m_frontSpeed(15.0f)
	,m_sidewaysSpeed(10.0f)
	,m_pickedBodyParam(0.0f)
	,m_prevMouseState(false)
	,m_mouseLockState(false)
	,m_pickingMode(false)
{
}

ndLabCameraManager::~ndLabCameraManager()
{
	if (m_targetPicked) 
	{
		ResetPickBody();
	}
	delete m_camera;
}

void ndLabCameraManager::SetCameraMatrix(const ndQuaternion& rotation, const ndVector& position)
{
	// m_camera->SetMatrix(rotation, position);
	m_camera->SetMatrix(rotation, position);
	m_yaw = m_camera->GetYawAngle();
	m_pitch = m_camera->GetPichAngle();
}

void ndLabCameraManager::FixUpdate (ndPhysicsViewer* const scene, ndFloat32 timestep)
{
	// update the camera;
	ndMatrix targetMatrix (m_camera->GetNextMatrix());

	ndFloat32 mouseX;
	ndFloat32 mouseY;
	scene->GetMousePosition (mouseX, mouseY);

	// slow down the Camera if we have a Body
	ndFloat32 slowDownFactor = scene->IsShiftKeyDown() ? 0.5f/10.0f : 0.5f;

	// do camera translation
	if (scene->GetKeyState ('W')) 
	{
		targetMatrix.m_posit += targetMatrix.m_front.Scale(m_frontSpeed * timestep * slowDownFactor);
	}
	if (scene->GetKeyState ('S')) 
	{
		targetMatrix.m_posit -= targetMatrix.m_front.Scale(m_frontSpeed * timestep * slowDownFactor);
	}
	if (scene->GetKeyState ('A')) 
	{
		targetMatrix.m_posit -= targetMatrix.m_right.Scale(m_sidewaysSpeed * timestep * slowDownFactor);
	}
	if (scene->GetKeyState ('D')) 
	{
		targetMatrix.m_posit += targetMatrix.m_right.Scale(m_sidewaysSpeed * timestep * slowDownFactor);
	}

	if (scene->GetKeyState ('Q')) 
	{
		targetMatrix.m_posit -= targetMatrix.m_up.Scale(m_sidewaysSpeed * timestep * slowDownFactor);
	}

	if (scene->GetKeyState ('E')) 
	{
		targetMatrix.m_posit += targetMatrix.m_up.Scale(m_sidewaysSpeed * timestep * slowDownFactor);
	}

	bool mouseState = !scene->GetCaptured() && (scene->GetMouseKeyState(0) && !scene->GetMouseKeyState(1));

	// do camera rotation, only if we do not have anything picked
	bool buttonState = m_mouseLockState || mouseState;
	if (!m_targetPicked && buttonState) 
	{
		ndFloat32 mouseSpeedX = mouseX - m_mousePosX;
		ndFloat32 mouseSpeedY = mouseY - m_mousePosY;

		if (ImGui::IsMouseDown(0))
		{
			if (mouseSpeedX > 0.0f) 
			{
				m_yaw = ndAnglesAdd(m_yaw, m_yawRate);
			} 
			else if (mouseSpeedX < 0.0f)
			{
				m_yaw = ndAnglesAdd(m_yaw, -m_yawRate);
			}

			if (mouseSpeedY > 0.0f)
			{
				m_pitch += m_pitchRate;
			} 
			else if (mouseSpeedY < 0.0f)
			{
				m_pitch -= m_pitchRate;
			}
			m_pitch = ndClamp(m_pitch, ndFloat32 (-80.0f * ndDegreeToRad), ndFloat32 (80.0f * ndDegreeToRad));
		}
	}

	m_mousePosX = mouseX;
	m_mousePosY = mouseY;

	//m_yaw += 0.01f;
	ndMatrix matrix (ndRollMatrix(m_pitch) * ndYawMatrix(m_yaw));
	ndQuaternion rot (matrix);
	m_camera->SetMatrix (rot, targetMatrix.m_posit);

	// get the mouse pick parameter so that we can do replay for debugging
	ndVector p0(m_camera->ScreenToWorld(ndVector(mouseX, mouseY, 0.0f, 0.0f)));
	ndVector p1(m_camera->ScreenToWorld(ndVector(mouseX, mouseY, 1.0f, 0.0f)));

#ifdef D_ENABLE_CAMERA_REPLAY
	struct ndReplay
	{
		ndVector m_p0;
		ndVector m_p1;
		ndInt32 m_mouseState;
	};
	ndReplay replay;

	#ifdef D_RECORD_CAMERA
		replay.m_p0 = p0;
		replay.m_p1 = p1;
		replay.m_mouseState = mouseState ? 1 : 0;

		static FILE* file = fopen("cameraLog.bin", "wb");
		if (file) 
		{
			fwrite(&replay, sizeof(ndReplay), 1, file);
			fflush(file);
		}
	#else 
		static FILE* file = fopen("cameraLog.bin", "rb");
		if (file) 
		{
			fread(&replay, sizeof(ndReplay), 1, file);
			p0 = replay.m_p0;
			p1 = replay.m_p1;
			mouseState = replay.m_mouseState ? true : false;
		}
	#endif
#endif

	//dTrace(("frame: %d  camera angle: %f\n", scene->GetWorld()->GetFrameIndex(), m_yaw * dRadToDegree));
	UpdatePickBody(scene, mouseState, p0, p1, timestep);
}

void ndLabCameraManager::SetCameraMouseLock (bool loockState)
{
	m_mouseLockState = loockState;
}

void ndLabCameraManager::RenderPickedTarget () const
{
	if (m_targetPicked) 
	{
		ndAssert(0);
		//ndMatrix matrix;
		//NewtonBodyGetMatrix(m_targetPicked, &matrix[0][0]);
		//
		//ndVector p0 (matrix.TransformVector(m_pickedBodyLocalAtachmentPoint));
		//ndVector p1 (p0 + matrix.RotateVector (m_pickedBodyLocalAtachmentNormal.Scale (0.5f)));
		//ShowMousePicking (p0, p1);
	}
}

void ndLabCameraManager::InterpolateMatrices (ndPhysicsViewer* const scene, ndFloat32 param)
{
	D_TRACKTIME();

	for (ndPhysicsViewer::ndNode* node = scene->GetFirst(); node; node = node->GetNext()) 
	{
		ndBaseEntity* const entity = node->GetInfo();
		entity->InterpolateMatrix(param);
	}

	// interpolate the Camera matrix;
	m_camera->InterpolateMatrix (param);

	//world->UpdateTransformsUnlock();
}

void ndLabCameraManager::UpdatePickBody(ndPhysicsViewer* const scene, bool mousePickState, const ndVector& p0, const ndVector& p1, ndFloat32) 
{
	// handle pick body from the screen
	if (!m_targetPicked) 
	{
		if (!m_prevMouseState && mousePickState) 
		{
			ndFloat32 param;
			ndVector posit;
			ndVector normal;
		
			ndBodyKinematic* const body = MousePickBody (scene->GetWorld(), p0, p1, param, posit, normal);
			if (body) 
			{
				ndBaseEntityNotify* const notify = (ndBaseEntityNotify*)body->GetNotifyCallback();
				if (notify)
				{
					notify->OnObjectPick();
					m_targetPicked = body;
		
					m_pickedBodyParam = param;
					if (*m_pickJoint)
					{
						scene->GetWorld()->RemoveJoint(*m_pickJoint);
					}
		
					ndVector mass(m_targetPicked->GetMassMatrix());
		
					//change this to make the grabbing stronger or weaker
					//const ndFloat32 angularFritionAccel = 10.0f;
					const ndFloat32 angularFritionAccel = 10.0f;
					const ndFloat32 linearFrictionAccel = 40.0f * ndMax(ndAbs(DEMO_GRAVITY), ndFloat32(10.0f));
					const ndFloat32 inertia = ndMax(mass.m_z, ndMax(mass.m_x, mass.m_y));
		
					ndLabCameraPickBodyJoint* const pickJoint = new ndLabCameraPickBodyJoint(body, scene->GetWorld()->GetSentinelBody(), posit, this);
					m_pickJoint = ndSharedPtr<ndJointBilateralConstraint>(pickJoint);
					scene->GetWorld()->AddJoint(m_pickJoint);
					m_pickingMode ?
						pickJoint->SetControlMode(ndJointKinematicController::m_linear) :
						pickJoint->SetControlMode(ndJointKinematicController::m_linearPlusAngularFriction);
					
					pickJoint->SetMaxLinearFriction(mass.m_w * linearFrictionAccel);
					pickJoint->SetMaxAngularFriction(inertia * angularFritionAccel);
				}
			}
		}
	} 
	else 
	{
		if (mousePickState) 
		{
			m_pickedBodyTargetPosition = p0 + (p1 - p0).Scale (m_pickedBodyParam);
			
			if (*m_pickJoint) 
			{
				ndLabCameraPickBodyJoint* const pickJoint = (ndLabCameraPickBodyJoint*)*m_pickJoint;
				pickJoint->SetTargetPosit (m_pickedBodyTargetPosition); 
			}
		} 
		else 
		{
			if (*m_pickJoint)
			{
				scene->GetWorld()->RemoveJoint(*m_pickJoint);
			}
			ResetPickBody();
		}
	}

	m_prevMouseState = mousePickState;
}

void ndLabCameraManager::ResetPickBody()
{
	if (m_targetPicked) 
	{
		m_targetPicked->SetSleepState(false);
	}
	if (*m_pickJoint) 
	{
		ndLabCameraPickBodyJoint* const pickJoint = (ndLabCameraPickBodyJoint*)*m_pickJoint;
		pickJoint->m_manager = nullptr;
	}
	m_pickJoint = ndSharedPtr<ndJointBilateralConstraint>(nullptr);
	m_targetPicked = nullptr;
}

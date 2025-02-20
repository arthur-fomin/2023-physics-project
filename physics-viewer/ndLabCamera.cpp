#include "ndLabCamera.h"

#define MOUSE_PICK_DAMP			10.0f
#define MOUSE_PICK_STIFFNESS	100.0f

#define D_CAMERA_ANGLE			60.0f

ndLabCamera::ndLabCamera()
	:ndBaseEntity (ndGetIdentityMatrix(), nullptr) 
	,m_viewMatrix(ndGetIdentityMatrix())
	,m_projectionMatrix(ndGetIdentityMatrix())
	,m_fov(D_CAMERA_ANGLE * ndDegreeToRad)
	,m_backPlane(2000.0f)
	,m_frontPlane (0.01f)
	,m_cameraYaw(0.0f)
	,m_cameraPitch(0.0f)
{
}
	
ndLabCamera::~ndLabCamera()
{
}

void ndLabCamera::Render(ndFloat32, ndPhysicsViewer* const, const ndMatrix&) const
{
}

ndFloat32 ndLabCamera::GetYawAngle() const
{
	return m_cameraYaw;
}

ndFloat32 ndLabCamera::GetPichAngle() const
{
	return m_cameraPitch;
}

const ndMatrix& ndLabCamera::GetProjectionMatrix() const
{
	return m_projectionMatrix;
}

const ndMatrix& ndLabCamera::GetViewMatrix() const
{
	return m_viewMatrix;
}

ndMatrix ndLabCamera::CreateMatrixFromFrustum(ndFloat32 left, ndFloat32 right, ndFloat32 bottom, ndFloat32 top, ndFloat32 front, ndFloat32 back)
{
	ndMatrix projectionMatrix(ndGetIdentityMatrix());

	projectionMatrix[0][0] = 2.0f * front / (right - left);
	projectionMatrix[0][1] = 0.0f;
	projectionMatrix[0][2] = 0.0f;
	projectionMatrix[0][3] = 0.0f;

	projectionMatrix[1][0] = 0.0f;
	projectionMatrix[1][1] = 2.0f * front / (top - bottom);
	projectionMatrix[1][2] = 0.0f;
	projectionMatrix[1][3] = 0.0f;

	projectionMatrix[2][0] = (right + left) / (right - left);
	projectionMatrix[2][1] = (top + bottom) / (top - bottom);
	projectionMatrix[2][2] = -(back + front) / (back - front);
	projectionMatrix[2][3] = -1.0f;

	projectionMatrix[3][0] = 0.0f;
	projectionMatrix[3][1] = 0.0f;
	projectionMatrix[3][2] = -2.0f * back * front / (back - front);
	projectionMatrix[3][3] = 0.0f;

	return projectionMatrix;
}

ndMatrix ndLabCamera::CreateLookAtMatrix(const ndVector& eyepoint, const ndVector& eyepointTarget, const ndVector& normUp)
{
	ndMatrix result(ndGetIdentityMatrix());
	
	ndVector zAxis(eyepoint - eyepointTarget);
	zAxis = zAxis & ndVector::m_triplexMask;
	zAxis = zAxis.Normalize();

	ndVector xAxis(normUp.CrossProduct(zAxis) & ndVector::m_triplexMask);
	xAxis = xAxis.Normalize();
	ndVector YAxis (zAxis.CrossProduct(xAxis) & ndVector::m_triplexMask);

	result[0] = xAxis;
	result[1] = YAxis;
	result[2] = zAxis;
	result[3] = ndVector::m_wOne;
	result = result.Transpose();

	ndVector negEye (eyepoint);
	negEye = negEye.Scale(-1.0f);
	negEye[3] = 1.0f;
	result[3] = result.TransformVector(negEye);
	return result;
}

ndMatrix ndLabCamera::CreatePerspectiveMatrix(ndFloat32 fov, ndFloat32 aspect, ndFloat32 front, ndFloat32 back)
{
	fov = ndClamp (fov, ndFloat32 (0.0f), ndPi);
	ndFloat32 y = front * ndTan(fov * 0.5f);
	ndFloat32 x = y * aspect;
	ndMatrix projectionMatrix (CreateMatrixFromFrustum(-x, x, -y, y, front, back));
	return projectionMatrix;
}

void ndLabCamera::SetViewMatrix(ndInt32 width, ndInt32 height)
{
	// set the view port for this render section
	glViewport(0, 0, (GLint)width, (GLint)height);
	glGetIntegerv(GL_VIEWPORT, m_viewport);

	// calculate projection matrix
	m_projectionMatrix = CreatePerspectiveMatrix(m_fov, GLfloat(width) / GLfloat(height), m_frontPlane, m_backPlane);

	// set the model view matrix 
	const ndVector pointOfInterest(m_matrix.m_posit + m_matrix.m_front);
	m_viewMatrix = CreateLookAtMatrix(m_matrix.m_posit, pointOfInterest, m_matrix.m_up);
}

ndVector ndLabCamera::ScreenToWorld (const ndVector& screenPoint) const
{
	GLdouble winX = screenPoint.m_x; //Store the x cord;
	GLdouble winY = screenPoint.m_y; //Store the y cord
	GLdouble winZ = screenPoint.m_z; //Store the Z cord

	//Now windows coordinates start with (0,0) being at the top left 
	//whereas OpenGL cords start lower left so we have to do this to convert it: 
	//Remember we got viewport value before 
	winY = (ndFloat32)m_viewport[3] - winY; 

	GLdouble objx;
	GLdouble objy;
	GLdouble objz;
	GLdouble modelViewMatrix[16];
	GLdouble projectionViewMatrix[16];
	for (ndInt32 i = 0; i < 4; ++i) 
	{
		for (ndInt32 j = 0; j < 4; ++j)
		{
			modelViewMatrix[i * 4 + j] = m_viewMatrix[i][j];
			projectionViewMatrix[i * 4 + j] = m_projectionMatrix[i][j];
		}
	}

	gluUnProject (winX, winY, winZ, modelViewMatrix, projectionViewMatrix, (GLint*)&m_viewport, &objx, &objy, &objz);
	return ndVector (ndFloat32(objx), ndFloat32(objy), ndFloat32(objz), ndFloat32 (1.0f));
}

//ndVector ndLabCamera::WorldToScreen (const ndVector& worldPoint) const
ndVector ndLabCamera::WorldToScreen(const ndVector&) const
{
ndAssert (0);

/*
	ndInt32 win[4]; 
	GLint viewport[4]; 

	//Retrieves the viewport and stores it in the variable
	//get a point on the display array of the windows
	GLUI_Master.get_viewport_area(&win[0], &win[1], &win[2], &win[3]);
	viewport[0] = GLint (win[0]);
	viewport[1] = GLint (win[1]);
	viewport[2] = GLint (win[2]);
	viewport[3] = GLint (win[3]);


	//Where the 16 doubles of the matrix are to be stored
	GLdouble modelview[16]; 

	//Retrieve the matrix
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview); 

	GLdouble projection[16]; 
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	GLdouble winX;
	GLdouble winY;
	GLdouble winZ; //The coordinates to pass in

	//Now windows coordinates start with (0,0) being at the top left 
	//whereas OpenGL cords start lower left so we have to do this to convert it: 
	//Remember we got viewport value before 
	GLdouble objx = world.m_x;
	GLdouble objy = world.m_y;
	GLdouble objz = world.m_z;

	// use the real GL view port
	glGetIntegerv(GL_VIEWPORT, viewport); 
	gluProject (objx, objy, objz, modelview, projection, viewport, &winX, &winY, &winZ);

	winY = (ndFloat32)viewport[3] - winY; 

	return ndVector (ndFloat32(winX), ndFloat32 (winY), ndFloat32(winZ), 0.0f);
*/
	return ndVector::m_zero;
}

void ndLabCamera::SetMatrix (const ndQuaternion& rotation, const ndVector& position)
{
	ndMatrix matrix (rotation, position);
	m_cameraPitch = ndAsin (matrix.m_front.m_y);
	m_cameraYaw = ndAtan2 (-matrix.m_front.m_z, matrix.m_front.m_x);

	ndBaseEntity::SetMatrix (rotation, position);
}




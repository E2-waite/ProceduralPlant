 ////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"

/// Initializes the position and rotation of the camera to be at the origin of the scene.
CameraClass::CameraClass()
{
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}




XMFLOAT3& CameraClass::Rotation()
{
	return rotation;
}

float& CameraClass::Radius()
{
	return radius;
}

/// Provide updated view matrix to calling functions after the Render function has been called to create the view matrix.
void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}
void CameraClass::UpdateCam()
{
	// 		// Update the scene: update camera position.		
	//		static float radius = 3.0f;			
	static float theta  = (3.0f * XM_PI) / 2.0f;		
	static float phi = XM_PI/2;		
	float Cx, Cy, Cz;					
	theta = rotation.x / 10;
	phi = rotation.y;
	if(phi >= (XM_PI/9) * 8)		
	{						
		phi = (XM_PI /9) * 8;
	}		
	if(phi <= (XM_PI /9))
	{						
		phi = (XM_PI /9);
	}				
	Cx = radius * cosf(theta) * sinf(phi);		
	Cy = radius * cosf(phi);		
	Cz = radius * sinf(theta) * sinf(phi);				
	XMFLOAT3 position( Cx, -Cy, Cz);		
	XMFLOAT3 target(0.0f, 4.0f, 0.0f);
	XMFLOAT3 up(0.0f, 1.0f, 0.0f);
	m_viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&position), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

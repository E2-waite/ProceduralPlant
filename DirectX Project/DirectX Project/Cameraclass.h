#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

#include <DirectXMath.h>
using namespace DirectX;

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	XMFLOAT3& Rotation();
	float& Radius();

	void Render(XMVECTOR);
	void GetViewMatrix(XMMATRIX&);
	void UpdateCam();
private:
	XMFLOAT3 rotation = XMFLOAT3(0,1.8f,0);
	XMMATRIX m_viewMatrix;
	float radius = 25;
};

#endif
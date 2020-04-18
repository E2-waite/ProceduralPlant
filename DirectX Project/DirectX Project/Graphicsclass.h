////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <time.h>
#include "d3dclass.h"
#include "cameraclass.h"
#include "model.h"
#include "Lightshaderclass.h"
#include "Textureshaderclass.h"
#include "Lightclass.h"
#include "bitmapclass.h"
#include "Plant.h"
#include "Types.h"
#include "imconfig.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();
	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int,int);
	void SetCamPos(float, float, float);
	void CamPosX(float);
	void CamPosY(float);
	void CamPosZ(float);
	void CamRotX(float);
	void CamRotY(float);
	void GUI();
private:
	bool Render(float);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	LightShaderClass* m_LightShader;
	TextureShaderClass* m_TextureShader;
	LightClass* m_Light;
	BitmapClass* m_Bitmap;
	Plant* flower;
	Plant* bush;
	int* reset_leaves;
	int* reset_petals;
	int* reset_stems;
	std::string model_file;
	Type type = Type::FLOWER;
};
#endif
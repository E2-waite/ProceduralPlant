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
#include "modelclass.h"
#include "Lightshaderclass.h"
#include "Textureshaderclass.h"
#include "Lightclass.h"
#include "bitmapclass.h"

#include "imconfig.h"
#include "IMGUI\\imgui.h"
#include "IMGUI\\imgui_impl_win32.h"
#include "IMGUI\\imgui_impl_dx11.h"

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
	bool SetupPlant();
	void WriteToFile();
private:
	bool Render(float);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Leaf;
	ModelClass* m_Stem;
	ModelClass* m_Petal;
	ModelClass* m_Crate;
	LightShaderClass* m_LightShader;
	TextureShaderClass* m_TextureShader;
	LightClass* m_Light;
	BitmapClass* m_Bitmap;
	int num_leaves = 4;
	int num_petals = 5;
	int num_stems = 3;
	int* reset_leaves;
	int reset_petals;
	int reset_stems;
	string model_file;
};
#endif
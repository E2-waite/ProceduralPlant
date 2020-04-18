////////////////////////////////////////////////////////////////////////////////
// Filename: colourshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _COLOURSHADERCLASS_H_
#define _COLOURSHADERCLASS_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: ColourShaderClass
////////////////////////////////////////////////////////////////////////////////
class ColourShaderClass
{
private:
/// This typedef must be exactly the same as the one in tyhe vertex shader as the model data needs to match the typedefs in the shader for proper rendering
struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

public:
	ColourShaderClass();
	ColourShaderClass(const ColourShaderClass&);
	~ColourShaderClass();
	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

private:
	bool InitializeShader(ID3D11Device*, HWND, const char*, const char*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const char*);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
};
#endif
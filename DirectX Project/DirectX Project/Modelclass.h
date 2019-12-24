////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dx10math.h>
#include <fstream>
using namespace std;
#include "Textureclass.h"
////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};
	int curr_index = 0;


public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();
	bool Initialize(ID3D11Device*, const char*, const char*, D3DXVECTOR3 rot, D3DXVECTOR3 pos, D3DXVECTOR3 scl);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	bool SetPos(float, float, float);
	bool SetRot(float, float, float);
	bool SetScale(float, float, float);
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();
	void UpdateMatrix();
	int GetIndexCount();
	D3DXMATRIX GetWorldMatrix();
	ID3D11ShaderResourceView* GetTexture();
	bool WriteVector();
	bool WriteTex();
	bool WriteNorm();
	int WriteFaces(int);
	int GetIndCount();
	float GetHeight();
private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	bool LoadTexture(ID3D11Device*, const char*);
	void ReleaseTexture();
	bool LoadModel(const char*);
	void ReleaseModel();
private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;
	ModelType* m_model;
	D3DXMATRIX m_worldMatrix;
	VertexType* vertices;
	unsigned long* indices;
	float x_pos, y_pos, z_pos;
	float x_rot, y_rot, z_rot;
	D3DXVECTOR3 start_rot;
	D3DXVECTOR3 rotation;
	D3DXVECTOR3 start_pos;
	D3DXVECTOR3 position;
	float x_scl = 1, y_scl = 1, z_scl = 1;
	int num_polygons;
	float height = 0;
};

#endif
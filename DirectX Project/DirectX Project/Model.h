#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include "Textureclass.h"
using namespace DirectX;

class Model
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};
	int curr_index = 0;


public:
	Model();
	Model(const Model&);
	~Model();
	bool Initialize(ID3D11Device*, const char*, const char*, XMFLOAT3 rot, XMFLOAT3 pos, XMFLOAT3 scl);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	XMFLOAT3& Position();
	XMFLOAT3& Rotation();
	XMFLOAT3 StartRot();
	XMFLOAT3& Scale();
	XMFLOAT3& TopPos();
	void UpdateMatrix();
	int GetIndexCount();
	XMMATRIX GetWorldMatrix();
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
	XMMATRIX m_worldMatrix;
	VertexType* vertices;
	unsigned long* indices;
	float x_pos, y_pos, z_pos;
	float x_rot, y_rot, z_rot;
	XMFLOAT3 start_rot;
	XMFLOAT3 rotation = XMFLOAT3(0,0,0);
	XMFLOAT3 start_pos;
	XMFLOAT3 top_pos;
	XMFLOAT3 position;
	XMFLOAT3 scale= XMFLOAT3(1,1,1);
	int num_polygons;
	float height = 0;
};

#endif
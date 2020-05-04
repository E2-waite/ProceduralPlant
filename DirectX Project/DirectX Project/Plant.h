#pragma once
#include <string>
#include "model.h"
#include "Lightshaderclass.h"
#include "Lightclass.h"
#include "Types.h"
class Plant
{
public:
	Plant();
	~Plant();
	void Setup(Type, ID3D11Device*, int* = NULL, int* = NULL, int* = NULL);
	void SetupBamboo(ID3D11Device*, int* = NULL, int* = NULL);
	void SetupVine(ID3D11Device*, int* = NULL, int* = NULL);
	void SetupFlower(ID3D11Device*, int* = NULL, int* = NULL, int* = NULL);
	void Shutdown();
	void Update(Type);
	void Render(ID3D11DeviceContext*, LightShaderClass*, LightClass*, XMMATRIX view_matrix, XMMATRIX projection_matrix);
	std::string WriteToFile(Type);
	void SetRot(Type, Element, XMFLOAT3);
	void SetScl(Element, XMFLOAT3);
	void SetNum(Type,ID3D11Device* device, Element, int*);
	XMFLOAT3 GetCentre();
	bool FileExists(const std::string& filename);
private:
	Model* m_Leaf;
	Model* m_Stem;
	Model* m_Petal;
	int num_leaves = 4;
	int num_petals = 5;
	int num_stems = 4;
	bool setting_up = false;
};


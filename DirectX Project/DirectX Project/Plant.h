#pragma once
#include <string>
#include <iomanip>
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
	int Leaves();
	int Petals();
	int Stems();
private:
	Model* m_Leaf;
	Model* m_Stem;
	Model* m_Petal;
	int num_leaves = 2;
	int num_petals = 5;
	int num_stems = 2;
	bool setting_up = false;

	XMFLOAT3 stem_rgb = XMFLOAT3(0, 0.8f, 0);
	XMFLOAT3 petal_rgb = XMFLOAT3(1, 0, 0);
	XMFLOAT3 leaf_rgb = XMFLOAT3(0, 1, 0);
};


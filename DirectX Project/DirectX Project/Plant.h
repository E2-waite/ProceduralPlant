#pragma once
#include "model.h"
#include "Lightshaderclass.h"
#include "Lightclass.h"
#include "Types.h"
class Plant
{
public:
	Plant();
	~Plant();
	void Setup(ID3D11Device*);
	void Shutdown();
	void Update();
	void Render(ID3D11DeviceContext*, LightShaderClass*, LightClass*, XMMATRIX view_matrix, XMMATRIX projection_matrix);
	void WriteToFile();
	void SetRot(Element element, XMFLOAT3);
	XMFLOAT3 GetCentre();
private:
	Model* m_Leaf;
	Model* m_Stem;
	Model* m_Petal;
	int num_leaves = 4;
	int num_petals = 5;
	int num_stems = 3;
};


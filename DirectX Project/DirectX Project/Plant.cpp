#include "Plant.h"

Plant::Plant()
{

}
Plant::~Plant()
{

}
void Plant::Setup(ID3D11Device* device)
{
	if (m_Leaf)
	{
		for (int i = 0; i < num_leaves; i++)
		{
			m_Leaf[i].Shutdown();
		}
		delete[] m_Leaf;
		m_Leaf = 0;
	}

	if (m_Stem)
	{
		for (int i = 0; i < num_stems; i++)
		{
			m_Stem[i].Shutdown();
		}
		delete[] m_Stem;
		m_Stem = 0;
	}

	if (m_Petal)
	{
		for (int i = 0; i < num_petals; i++)
		{
			m_Petal[i].Shutdown();
		}
		delete[] m_Petal;
		m_Petal = 0;
	}
	float stem_rot = rand() % 360 + 0;

	num_leaves = rand() % 4 + 2;
	
	m_Stem = new Model[num_stems];
	m_Leaf = new Model[num_leaves];
	m_Petal = new Model[num_petals];

	float stem_pos = 0;
	float rot_swap = 45;
	for (int i = 0; i < num_stems; i++)
	{
		if (i >= 1)
		{
			m_Stem[i].Initialize(device, "Data/Stem.txt", "Data/leaf.dds", XMFLOAT3(0, 0, rot_swap), XMFLOAT3(0, stem_pos, 0),
				XMFLOAT3(0, 0, 0));
			rot_swap = -rot_swap;
		}
		else
		{
			m_Stem[i].Initialize(device, "Data/Stem.txt", "Data/leaf.dds", XMFLOAT3(0, 0, 0), XMFLOAT3(0, stem_pos, 0),
				XMFLOAT3(0, 0, 0));
			stem_pos += m_Stem[i].GetHeight();
		}
	}

	float x_rot = rand() % 360 + 0;
	for (int i = 0; i < 5; i++)
	{
		float rot_num = rand() % 10 + -10;
		m_Leaf[i].Initialize(device, "Data/Leaf2.txt", "Data/leaf.dds",
			XMFLOAT3(x_rot, 0, 0), XMFLOAT3(0, 3, 0), XMFLOAT3(1, 1, 1));
		m_Leaf[i].SetScale(1, 1, 1);
		x_rot -= 360 / num_leaves;
	}

	x_rot = rand() % 360 + 0;
	int flower_rot = rand() % 360 + 0;
	float rand_scale = rand() % 7 + 1;
	rand_scale = (rand_scale / 10) + 0.5f;
	for (int i = 0; i < num_petals; i++)
	{
		m_Petal[i].Initialize(device, "Data/Petal.txt", "Data/petal.dds", XMFLOAT3(flower_rot, x_rot, 30), XMFLOAT3(0, 6, 0),
			XMFLOAT3(1, 1, 1));
		x_rot -= (360 / num_petals) + 1;
	}
}

void Plant::Shutdown()
{
	// Release the model object.
	if (m_Leaf)
	{
		for (int i = 0; i < num_leaves; i++)
		{
			m_Leaf[i].Shutdown();
		}
		delete[] m_Leaf;
		m_Leaf = 0;
	}

	if (m_Stem)
	{
		for (int i = 0; i < num_stems; i++)
		{
			m_Stem[i].Shutdown();
		}
		delete[] m_Stem;
		m_Stem = 0;
	}
}
void Plant::Update()
{

}
void Plant::Render(ID3D11DeviceContext* context, LightShaderClass* shader, LightClass* light, XMMATRIX view_matrix, XMMATRIX projection_matrix)
{
	for (int i = 0; i < num_stems; i++)
	{
		m_Stem[i].Render(context);
		shader->Render(context, m_Stem[i].GetIndexCount(), m_Stem[i].GetWorldMatrix(), view_matrix,
			projection_matrix, m_Stem[i].GetTexture(), light->GetDirection(), light->GetAmbientColour(), light->GetDiffuseColour());
	}
	for (int i = 0; i < num_leaves; i++)
	{
		m_Leaf[i].Render(context);
		shader->Render(context, m_Leaf[i].GetIndexCount(), m_Leaf[i].GetWorldMatrix(), view_matrix,
			projection_matrix, m_Leaf[i].GetTexture(), light->GetDirection(), light->GetAmbientColour(), light->GetDiffuseColour());
	}
	for (int i = 0; i < num_petals; i++)
	{
		m_Petal[i].Render(context);
		shader->Render(context, m_Petal[i].GetIndexCount(), m_Petal[i].GetWorldMatrix(), view_matrix,
			projection_matrix, m_Petal[i].GetTexture(), light->GetDirection(), light->GetAmbientColour(), light->GetDiffuseColour());
	}
}

void Plant::SetRot(Element element, XMFLOAT3 offset)
{
	if (element == LEAF)
	{
		for (int i = 0; i < num_leaves; i++)
		{
			m_Leaf[i].SetRot(m_Leaf[i].GetRotation().x + offset.x, m_Leaf[i].GetRotation().y + offset.y, m_Leaf[i].GetRotation().z + offset.z);
		}
	}
}

void Plant::WriteToFile()
{
	std::ofstream output_file;
	output_file.open("plant.obj", std::ofstream::out | std::ofstream::trunc);
	output_file << "mtllib plant.mtl \n";
	output_file << "g default \n";
	output_file.close();
	for (int i = 0; i < num_stems; i++)
	{
		m_Stem[i].WriteVector();
	}
	for (int i = 0; i < num_petals; i++)
	{
		m_Petal[i].WriteVector();
	}
	for (int i = 0; i < num_leaves; i++)
	{
		m_Leaf[i].WriteVector();
	}

	for (int i = 0; i < num_stems; i++)
	{
		m_Stem[i].WriteTex();
	}
	for (int i = 0; i < num_petals; i++)
	{
		m_Petal[i].WriteTex();
	}
	for (int i = 0; i < num_leaves; i++)
	{
		m_Leaf[i].WriteTex();
	}


	for (int i = 0; i < num_stems; i++)
	{
		m_Stem[i].WriteNorm();
	}
	for (int i = 0; i < num_petals; i++)
	{
		m_Petal[i].WriteNorm();
	}
	for (int i = 0; i < num_leaves; i++)
	{
		m_Leaf[i].WriteNorm();
	}
	output_file.open("plant.obj", std::ios_base::app);
	output_file << "s off \n";
	output_file << "g plant \n";
	output_file << "usemtl stemMat \n";
	output_file.close();

	int total_indices = 0;

	for (int i = 0; i < num_stems; i++)
	{
		total_indices += m_Stem[i].WriteFaces(total_indices);
	}

	output_file.open("plant.obj", std::ios_base::app);
	output_file << "usemtl petalMat \n";
	output_file.close();
	for (int i = 0; i < num_petals; i++)
	{
		total_indices += m_Petal[i].WriteFaces(total_indices);
	}

	output_file.open("plant.obj", std::ios_base::app);
	output_file << "usemtl leafMat \n";
	output_file.close();
	for (int i = 0; i < num_leaves; i++)
	{
		total_indices += m_Leaf[i].WriteFaces(total_indices);
	}
}

XMFLOAT3 Plant::GetCentre()
{
	return m_Stem[0].GetPosition();
}
#include "Plant.h"

Plant::Plant()
{
	m_Leaf = NULL;
	m_Petal = NULL;
	m_Stem = NULL;
}
Plant::~Plant()
{

}
void Plant::Setup(ID3D11Device* device, int* reset_leaves, int* reset_petals, int* reset_stems)
{
	setting_up = true;
	if (m_Leaf)
	{
		for (int i = 0; i < num_leaves; i++)
		{
			m_Leaf[i].Shutdown();
		}
		delete[] m_Leaf;
		m_Leaf = NULL;
	}

	if (m_Stem)
	{
		for (int i = 0; i < num_stems; i++)
		{
			m_Stem[i].Shutdown();
		}
		delete[] m_Stem;
		m_Stem = NULL;
	}

	if (m_Petal)
	{
		for (int i = 0; i < num_petals; i++)
		{
			m_Petal[i].Shutdown();
		}
		delete[] m_Petal;
		m_Petal = NULL;
	}
	//float stem_rot = rand() % 360 + 0;

	if (reset_leaves)
	{
		num_leaves = *reset_leaves;
	}
	if (reset_petals)
	{
		num_petals = *reset_petals;
	}
	if (reset_stems)
	{
		num_stems = *reset_stems;
	}

	m_Stem = new Model[num_stems];
	m_Leaf = new Model[num_leaves];
	m_Petal = new Model[num_petals];

	float rot_swap = 45;
	for (int i = 0; i < num_stems; i++)
	{
		m_Stem[i].Initialize(device, "Data/Stem.txt", "Data/leaf.dds", XMFLOAT3(0, 0, 0), 
			XMFLOAT3(0, (m_Stem[i].GetHeight() * m_Stem[i].Scale().y) * i, 0), XMFLOAT3(0, 0, 0));
	}
	float x_rot = 0;
	for (int i = 0; i < num_leaves; i++)
	{
		float rot_num = rand() % 10 + -10;
		m_Leaf[i].Initialize(device, "Data/Leaf2.txt", "Data/leaf.dds",
			XMFLOAT3(x_rot, 0, 0), XMFLOAT3(0, 3, 0), XMFLOAT3(1, 1, 1));
		m_Leaf[i].Scale() = XMFLOAT3(1, 1, 1);
		x_rot -= 360 / num_leaves;
	}

	x_rot = 0;
	int flower_rot = 0;
	for (int i = 0; i < num_petals; i++)
	{
		m_Petal[i].Initialize(device, "Data/Petal.txt", "Data/petal.dds", XMFLOAT3(flower_rot, x_rot, 30), XMFLOAT3(0, m_Stem[0].Scale().y * m_Stem[0].GetHeight(), 0),
			XMFLOAT3(1, 1, 1));
		x_rot -= (360 / num_petals) + 1;
	}
	
	setting_up = false;
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
	for (int i = 0; i < num_petals; i++)
	{
		m_Petal[i].Position() = XMFLOAT3(0, (m_Stem[0].GetHeight() * m_Stem[0].Scale().y) * num_stems, 0);
	}
	for (int i = 0; i < num_stems; i++)
	{
		m_Stem[i].Position() = XMFLOAT3(0, (m_Stem[i].GetHeight() * m_Stem[i].Scale().y) * i, 0);
	}
	for (int i = 0; i < num_leaves; i++)
	{
		if (m_Leaf[i].Position().y > (m_Stem[0].GetHeight() * m_Stem[0].Scale().y)* num_stems)
		{
			m_Leaf[i].Position().y = (m_Stem[0].GetHeight() * m_Stem[0].Scale().y) * num_stems;
		}
	}
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

void Plant::SetRot(Element element, XMFLOAT3 rot)
{
	if (element == Element::LEAF)
	{
		for (int i = 0; i < num_leaves; i++)
		{
			m_Leaf[i].Rotation() = XMFLOAT3(m_Leaf[i].StartRot().x + rot.x, m_Leaf[i].StartRot().y + rot.y, m_Leaf[i].StartRot().z + rot.z);
		}
	}
	if (element == Element::PETAL)
	{
		for (int i = 0; i < num_petals; i++)
		{
			m_Petal[i].Rotation() = XMFLOAT3(m_Petal[i].StartRot().x + rot.x, m_Petal[i].StartRot().y + rot.y, m_Petal[i].StartRot().z + rot.z);
		}
	}
}

void Plant::SetScl(Element element, XMFLOAT3 scl)
{
	if (element == Element::LEAF)
	{
		for (int i = 0; i < num_leaves; i++)
		{
			m_Leaf[i].Scale() = XMFLOAT3(scl.x, scl.y, scl.z);
		}
	}
	if (element == Element::PETAL)
	{
		for (int i = 0; i < num_petals; i++)
		{
			m_Petal[i].Scale() = XMFLOAT3(scl.x, scl.y, scl.z);
		}
	}
	if (element == Element::STEM)
	{
		for (int i = 0; i < num_stems; i++)
		{
			m_Stem[i].Scale() = XMFLOAT3(scl.x, scl.y, scl.z);
		}
	}
}

void Plant::SetNum(ID3D11Device* device, Element element, int* num)
{
	if (!setting_up)
	{
		if (element == Element::LEAF && *num != num_leaves)
		{
			Setup(device, num, NULL, NULL);
		}
		if (element == Element::PETAL && *num != num_petals)
		{
			Setup(device, NULL, num, NULL);
		}
		if (element == Element::STEM && *num != num_stems)
		{
			Setup(device, NULL, NULL, num);
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
	return m_Stem[0].Position();
}
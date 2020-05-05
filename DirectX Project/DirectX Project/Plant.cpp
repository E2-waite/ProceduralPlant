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
void Plant::Setup(Type type,ID3D11Device* device, int* reset_leaves, int* reset_petals, int* reset_stems)
{
	type = type;
	if (type == Type::FLOWER)
	{
		SetupFlower(device, reset_leaves, reset_petals, reset_stems);
	}
	else if (type == Type::VINE)
	{
		num_petals = 0;
		SetupVine(device, reset_leaves, reset_stems);
	}
	else if (type == Type::BAMBOO)
	{
		num_petals = 0;
		SetupBamboo(device, reset_leaves, reset_stems);
	}
}

void Plant::SetupBamboo(ID3D11Device* device, int* reset_leaves, int* reset_stems)
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

	if (reset_leaves)
	{
		num_leaves = *reset_leaves;
	}
	
	if (reset_stems)
	{
		num_stems = *reset_stems;
	}

	m_Stem = new Model[num_stems];
	m_Leaf = new Model[num_leaves];

	for (int i = 0; i < num_stems; i++)
	{
		if (i > 0)
		{
			m_Stem[i].Initialize(device, "Data/Stem.txt", "Data/Bamboo.dds", XMFLOAT3(0, 0, 0),
				m_Stem[i].TopPos(), XMFLOAT3(0, 0, 0));
		}
		else
		{
			m_Stem[i].Initialize(device, "Data/Stem.txt", "Data/Bamboo.dds", XMFLOAT3(0, 0, 0),
				XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0));
		}
	}

	for (int i = 0; i < num_leaves; i++)
	{
		int pos = ceil((i + 1) / 2);
		m_Leaf[i].Initialize(device, "Data/Leaf3.txt", "Data/Leaf2.dds",
			XMFLOAT3(0, 0, 0), m_Stem[pos].TopPos(), XMFLOAT3(2, 1, 2));
	}
	setting_up = false;
}

void Plant::SetupVine(ID3D11Device* device, int* reset_leaves, int* reset_stems)
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

	if (reset_stems)
	{
		num_stems = *reset_stems;
		num_leaves = num_stems;
	}

	m_Stem = new Model[num_stems];
	m_Leaf = new Model[num_leaves];
	float rot_y = 30;
	for (int i = 0; i < num_stems; i++)
	{
		if (i > 0)
		{
			m_Stem[i].Initialize(device, "Data/Stem.txt", "Data/leaf.dds", XMFLOAT3(0, rot_y, 0),
				m_Stem[i].TopPos(), XMFLOAT3(0, 0, 0));
		}
		else
		{
			m_Stem[i].Initialize(device, "Data/Stem.txt", "Data/leaf.dds", XMFLOAT3(0, rot_y, 0),
				XMFLOAT3(0,0,0), XMFLOAT3(0, 0, 0));
		}

		rot_y += 50;
	}

	int scl = 1;
	bool flip = false;
	for (int i = 0; i < num_leaves; i++)
	{
		float rot_z = m_Stem[i].Rotation().z;
		if (flip)
		{
			rot_z -= 180;
		}
		m_Leaf[i].Initialize(device, "Data/Leaf2.txt", "Data/leaf.dds",
			XMFLOAT3(0, 90, rot_z), m_Stem[i].TopPos(), XMFLOAT3(1, 1, 1));
	}
	setting_up = false;
}
void Plant::SetupFlower(ID3D11Device* device, int* reset_leaves, int* reset_petals, int* reset_stems)
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
void Plant::Update(Type type)
{
	for (int i = 0; i < num_petals; i++)
	{
		m_Petal[i].Position() = m_Stem[num_stems - 1].TopPos();
	}
	for (int i = 0; i < num_stems; i++)
	{
		if (i > 0)
		{
			m_Stem[i].Position() = m_Stem[i - 1].TopPos();
		}
		else
		{
			m_Stem[i].Position() = XMFLOAT3(0, 0, 0);
		}
	}
	bool flip = false;
	for (int i = 0; i < num_leaves; i++)
	{
		if (type == Type::FLOWER)
		{
			if (m_Leaf[i].Position().y > (m_Stem[0].GetHeight() * m_Stem[0].Scale().y)* num_stems)
			{
				m_Leaf[i].Position().y = (m_Stem[0].GetHeight() * m_Stem[0].Scale().y) * num_stems;
			}
		}
		else if (type == Type::VINE)
		{
			float rot_z = m_Stem[i].Rotation().z;
			if (flip)
			{
				rot_z -= 180;
				flip = false;
			}
			else
			{
				flip = true;
			}
			m_Leaf[i].Position() = m_Stem[i].TopPos();
			m_Leaf[i].Rotation().z = rot_z;				

		}
		else if (type == Type::BAMBOO)
		{
			int pos = ceil((i + 1) / 2);
			m_Leaf[i].Position() = m_Stem[pos].TopPos();
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

void Plant::SetRot(Type type, Element element, XMFLOAT3 rot )
{
	if (element == Element::LEAF)
	{
		if (type == Type::FLOWER)
		{
			for (int i = 0; i < num_leaves; i++)
			{
				m_Leaf[i].Rotation() = XMFLOAT3(m_Leaf[i].StartRot().x + rot.x, m_Leaf[i].StartRot().y + rot.y, m_Leaf[i].StartRot().z + rot.z);
			}
		}
		else if (type == Type::VINE)
		{
			for (int i = 0; i < num_leaves; i++)
			{
				m_Leaf[i].Rotation() = XMFLOAT3(rot.x, 90 + rot.y, m_Leaf[i].StartRot().z);
			}
		}
		else if (type == Type::BAMBOO)
		{
			int rot_x = rot.x;
			for (int i = 0; i < num_leaves; i++)
			{
				m_Leaf[i].Rotation() = XMFLOAT3(rot_x, m_Leaf[i].StartRot().y + rot.y, m_Leaf[i].StartRot().z + rot.z);
				rot_x += 90;
			}
		}
	}
	if (element == Element::PETAL)
	{
		for (int i = 0; i < num_petals; i++)
		{
			m_Petal[i].Rotation() = XMFLOAT3(m_Petal[i].StartRot().x + rot.x, m_Petal[i].StartRot().y + rot.y, m_Petal[i].StartRot().z + rot.z);
		}
	}
	if (element == Element::STEM)
	{
		if (type == Type::VINE)
		{
			bool inc = true;
			int ind = 0, max = 3;
			for (int i = 0; i < num_stems; i++)
			{
				m_Stem[i].Rotation().x = rot.x;
				m_Stem[i].Rotation().y = rot.y + ((ind * 2) * rot.y);
				m_Stem[i].Rotation().z = rot.z + ((ind * 2) * rot.z);
				if (inc)
				{
					if (ind < max)
					{
						ind++;
					}
					else
					{
						inc = false;
						ind--;
					}
				}
				else
				{
					if (ind > - (max * 2))
					{
						ind--;
					}
					else
					{
						inc = true;
						ind++;
					}
				}
			}
		}
		if (type == Type::FLOWER)
		{
			for (int i = 0; i < num_stems; i++)
			{
				m_Stem[i].Rotation() = rot;
			}
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

void Plant::SetNum(Type type, ID3D11Device* device, Element element, int* num)
{
	if (!setting_up)
	{
		if (element == Element::LEAF && *num != num_leaves)
		{
			Setup(type, device, num, NULL, NULL);
		}
		if (element == Element::PETAL && *num != num_petals)
		{
			Setup(type, device, NULL, num, NULL);
		}
		if (element == Element::STEM && *num != num_stems)
		{
			Setup(type, device, NULL, NULL, num);
		}
	}
}

std::string Plant::WriteToFile(Type type)
{
	std::ofstream obj_file;
	bool name_found = false;
	std::string obj_filename = "";
	std::string mtl_filename = "";
	std::string directory = "Models/";
	int file_ind = 0;
	while (!name_found)
	{
		if (type == Type::FLOWER)
		{
			obj_filename = "Flower " + std::to_string(file_ind) + ".obj";
			mtl_filename = "Flower " + std::to_string(file_ind) + ".mtl";
		}
		else if (type == Type::VINE)
		{
			obj_filename = "Vine " + std::to_string(file_ind) + ".obj";
			mtl_filename = "Vine " + std::to_string(file_ind) + ".mtl";
		}
		else if (type == Type::BAMBOO)
		{
			obj_filename = "Bamboo " + std::to_string(file_ind) + ".obj";
			mtl_filename = "Bamboo " + std::to_string(file_ind) + ".mtl";
		}
		if (!FileExists(directory + obj_filename))
		{
			break;
		}
		file_ind++;
	}
	obj_file.open(directory + obj_filename, std::ofstream::out | std::ofstream::trunc);
	obj_file << "mtllib " + mtl_filename+ "\n";
	obj_file << "g default \n";
	for (int i = 0; i < num_stems; i++)
	{
		m_Stem[i].WriteVector(obj_file);
	}
	for (int i = 0; i < num_petals; i++)
	{
		m_Petal[i].WriteVector(obj_file);
	}
	for (int i = 0; i < num_leaves; i++)
	{
		m_Leaf[i].WriteVector(obj_file);
	}

	for (int i = 0; i < num_stems; i++)
	{
		m_Stem[i].WriteTex(obj_file);
	}
	for (int i = 0; i < num_petals; i++)
	{
		m_Petal[i].WriteTex(obj_file);
	}
	for (int i = 0; i < num_leaves; i++)
	{
		m_Leaf[i].WriteTex(obj_file);
	}


	for (int i = 0; i < num_stems; i++)
	{
		m_Stem[i].WriteNorm(obj_file);
	}
	for (int i = 0; i < num_petals; i++)
	{
		m_Petal[i].WriteNorm(obj_file);
	}
	for (int i = 0; i < num_leaves; i++)
	{
		m_Leaf[i].WriteNorm(obj_file);
	}
	obj_file << "s off \n";
	obj_file << "g plant \n";
	int total_indices = 0;
	std::ofstream mtl_file(directory + mtl_filename, std::ofstream::out | std::ofstream::trunc);
	mtl_file << std::fixed << std::showpoint;
	mtl_file << std::setprecision(2);
	if (num_stems > 0)
	{
		obj_file << "g stem \n";
		obj_file << "usemtl stemMat \n";
		for (int i = 0; i < num_stems; i++)
		{
			total_indices += m_Stem[i].WriteFaces(obj_file, total_indices);
		}

		mtl_file << "newmtl stemMat \n";
		mtl_file << "illum 1 \n";
		mtl_file << "Kd "  << stem_rgb.x << " " << stem_rgb.y << " " <<  stem_rgb.z << "\n";
		mtl_file << "Ka 0.00 0.00 0.00 \n";
		mtl_file << "Tf 1.00 1.00 1.00 \n";
		mtl_file << "Ni 1.00\n";

	}

	if (num_petals > 0)
	{
		obj_file << "g petals \n";
		obj_file << "usemtl petalMat \n";
		for (int i = 0; i < num_petals; i++)
		{
			total_indices += m_Petal[i].WriteFaces(obj_file, total_indices);
		}
		mtl_file << "newmtl petalMat \n";
		mtl_file << "illum 1 \n";
		mtl_file << "Kd "  << petal_rgb.x << " "  << petal_rgb.y << " " << petal_rgb.z << "\n";
		mtl_file << "Ka 0.00 0.00 0.00 \n";
		mtl_file << "Tf 1.00 1.00 1.00 \n";
		mtl_file << "Ni 1.00\n";
	}
	if (num_leaves > 0)
	{
		obj_file << "g leaves \n";
		obj_file << "usemtl leafMat \n";
		for (int i = 0; i < num_leaves; i++)
		{
			total_indices += m_Leaf[i].WriteFaces(obj_file, total_indices);
		}
		mtl_file << "newmtl leafMat \n";
		mtl_file << "illum 1 \n";
		mtl_file << "Kd " <<  leaf_rgb.x << " "  << leaf_rgb.y << " " << leaf_rgb.z << "\n";
		mtl_file << "Ka 0.00 0.00 0.00 \n";
		mtl_file << "Tf 1.00 1.00 1.00 \n";
		mtl_file << "Ni 1.00\n";
	}
	obj_file.close();
	mtl_file.close();
	return obj_filename;
}

bool Plant::FileExists(const std::string& filename)
{
	struct stat buf;
	if (stat(filename.c_str(), &buf) != -1)
	{
		return true;
	}
	return false;
}

XMFLOAT3 Plant::GetCentre()
{
	return m_Stem[0].Position();
}

int Plant::Leaves()
{
	return num_leaves;
}
int Plant::Petals()
{
	return num_petals;
}
int Plant::Stems()
{
	return num_stems;
}
 ////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "model.h"

/// Initializes the vertex and index buffer pointers to null
Model::Model()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_model = 0;
	x_pos = 0.0; y_pos = 0.0; z_pos = 0.0;
	x_rot = 0.0; y_rot = 0.0; z_rot = 0.0;
}

Model::Model(const Model& other)
{
}

Model::~Model()
{
}
 
/// The Initialize function will call the initialization functions for the vertex and index buffers. 
bool Model::Initialize(ID3D11Device* device, const char* modelFilename, const char* textureFilename, XMFLOAT3 rot, XMFLOAT3 pos, XMFLOAT3 scl)
{
	bool result;

	start_rot = rot;
	start_pos = pos;
	rotation = start_rot;
	position = start_pos;

	// Load in the model data.
	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	XMMatrixIdentity() = m_worldMatrix;

	return true;
}

void Model::Shutdown()
{
	//Release the model texture.
	ReleaseTexture();

	// Release the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}

/// Called from the GraphicsClass::Render function. 
/// Calls RenderBuffers to put the vertex and index buffers on the graphics pipeline so the color shader will be able to render them. 
void Model::Render(ID3D11DeviceContext* deviceContext)
{
	UpdateMatrix();
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

/// Returns the number of indexes in the model
int Model::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* Model::GetTexture()
{
	return m_Texture->GetTexture();
}

/// This is where we handle creating the vertex and index buffers
/// Usually you would read in a model and create the buffers from that data file.
bool Model::InitializeBuffers(ID3D11Device* device)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (int i = 0; i < m_vertexCount; i++)
	{
		if (m_model[i].y > height)
		{
			height = m_model[i].y;
		}
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	/// Use vertex and index arrays to create vertex and index buffers.
	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	/// After vertex and index buffers have been created, delete the vertex and index arrays as they are no longer needed.
	// Release the arrays now that the vertex and index buffers have been created and loaded.

	return true;
}

/// Releases vertex and index buffers that were created in the InitializeBuffers function
void Model::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
	return;
}

void Model::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return;
}

//LoadTexture will create the texture object and then initialize it with the input file name provided. This function is called during initialization.
bool Model::LoadTexture(ID3D11Device* device, const char* filename)
{
	bool result;

	// Create the texture object.
	m_Texture = new TextureClass;
	if(!m_Texture)
	{
		return false;
	}

	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void Model::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

bool Model::LoadModel(const char* filename)
{
	std::ifstream fin;
	char input;
	int i;

	// Open the model file.
	fin.open(filename);

	// Exit if could not open
	if (fin.fail())
	{
		return false;
	}

	// Read up to the value of the vertex count
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Take the vertex count from Cube.txt
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model by initializing an array for the model's vertices using the vertex count
	m_model = new ModelType[m_vertexCount];

	if (!m_model)
	{
		return false;
	}

	// Read up to beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in vertex data and assign to m_model
	for (int i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	fin >> num_polygons;

	fin.close();

	return true;
}

void Model::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}

XMFLOAT3& Model::Position()
{
	return position;
}

XMFLOAT3& Model::Rotation()
{
	return rotation;
}

XMFLOAT3 Model::StartRot()
{
	return start_rot;
}

XMFLOAT3& Model::Scale()
{
	return scale;
}

XMMATRIX Model::GetWorldMatrix()
{
	return m_worldMatrix;
}

XMFLOAT3& Model::TopPos()
{
	return top_pos;
}


void Model::UpdateMatrix()
{
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix, positionMatrix, scaleMatrix;

	pitch = rotation.x * 0.0174532925f;
	yaw = rotation.y * 0.0174532925f;
	roll = rotation.z * 0.0174532925f;

	rotationMatrix = XMMatrixRotationRollPitchYaw(roll, pitch, yaw);
	positionMatrix = XMMatrixTranslation(position.x, position.y, position.z);
	scaleMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);

	m_worldMatrix = scaleMatrix * rotationMatrix * positionMatrix;

	top_pos = XMFLOAT3(0, height, 0);
	XMVECTOR top_vec = XMVector3Transform(XMLoadFloat3(&top_pos), m_worldMatrix);
	XMStoreFloat3(&top_pos, top_vec);
}


bool Model::WriteVector()
{
	std::ofstream output_file;
	output_file.open("plant.obj", std::ios_base::app);
	XMVECTOR* positions;
	
	positions = new XMVECTOR[m_vertexCount];
	for (int i = 0; i < m_vertexCount; i++)
	{
		positions[i] = XMVector3Transform(XMLoadFloat3(&vertices[i].position), m_worldMatrix);
	}

	for (int i = 0; i < m_indexCount; i++)
	{
		XMFLOAT3 pos_f;
		XMStoreFloat3(&pos_f, positions[i]);
		output_file << "v ";
		output_file << pos_f.x;
		output_file << " ";
		output_file << pos_f.y;
		output_file << " ";
		output_file << pos_f.z;
		output_file << "\n";
	}
	output_file << "\n";
	output_file.close();
	return true;
}
bool Model::WriteTex()
{
	std::ofstream output_file;
	output_file.open("plant.obj", std::ios_base::app);

	for (int i = 0; i < m_indexCount; i++)
	{			
		output_file << "vt ";
		output_file << vertices[indices[i]].texture.x;
		output_file << " ";
		output_file << vertices[indices[i]].texture.y;
		output_file << "\n";
	}
	return true;
}
bool Model::WriteNorm()
{
	std::ofstream output_file;
	output_file.open("plant.obj", std::ios_base::app);
	XMVECTOR* n_positions;
	n_positions = new XMVECTOR[m_vertexCount];

	for (int i = 0; i < m_vertexCount; i++)
	{
		n_positions[i] = XMVector3TransformNormal(XMLoadFloat3(&vertices[i].normal), m_worldMatrix);
	}

	for (int i = 0; i < m_indexCount; i++)
	{		
		XMFLOAT3 pos_f;
		XMStoreFloat3(&pos_f, n_positions[i]);
		output_file << "vn ";
		output_file << pos_f.x;
		output_file << " ";
		output_file << pos_f.y;
		output_file << " ";
		output_file << pos_f.z;
		output_file << "\n";
	}
	return true;
}

int Model::WriteFaces(int offset)
{
	std::ofstream output_file;
	output_file.open("plant.obj", std::ios_base::app);
	int poly_ind = 1;
	for (int i = 0; i < num_polygons; i++)
	{
		output_file << "f ";
		for (int j = 0; j < 3; j++)
		{
			output_file << poly_ind + offset;
			output_file << "/";
			output_file << poly_ind + offset;
			output_file << "/";
			output_file << poly_ind + offset;
			output_file << " ";
			poly_ind++;
		}
		output_file << "\n";
	}
	return m_indexCount;
}

int Model::GetIndCount()
{
	return m_indexCount;
}

float Model::GetHeight()
{
	return height;
}
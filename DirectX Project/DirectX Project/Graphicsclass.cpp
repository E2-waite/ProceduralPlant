////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Leaf = 0;
	m_Stem = 0;
	m_LightShader = 0;
	m_TextureShader = 0;
	m_Light = 0;
	m_Bitmap = 0;
	reset_leaves = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	// Create the Direct3D object
	m_D3D = new D3DClass;

	if (!m_D3D)
	{
		return false;
	}

	

	// Initialize the Direct3D object
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);

	if (!result)
	{
		// If D3D's initialize function fails display error message
		MessageBox(hwnd, "Could not initialize Direct3D", "Error", MB_OK);
	}

	// Setup ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(m_D3D->GetDevice(), m_D3D->GetDeviceContext());
	ImGui::StyleColorsDark();


	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	result = SetupPlant();
	// Initialize the model object with the texture that will be used.
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the leaves.", "Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the texture shader object.", "Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the light shader object.", "Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColour(0.3f, 0.3f, 0.3f, 0.3f);
	m_Light->SetDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);

	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	if (!m_Bitmap)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, "Data/crate.dds", 256, 256);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the bitmap object.", "Error", MB_OK);
		return false;
	}


	return true;
}

bool GraphicsClass::SetupPlant()
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
	srand(time(NULL));
	float stem_rot = rand() % 360 + 0;

	num_leaves = rand() % 4 + 2;

	if (reset_leaves)
	{
		num_leaves = *reset_leaves;
	}

	
	m_Stem = new ModelClass[num_stems];
	m_Leaf = new ModelClass[num_leaves];
	m_Petal = new ModelClass[num_petals];
	if (!m_Leaf)
	{
		return false;
	}

	float stem_pos = 0;
	for (int i = 0; i < num_stems; i++)
	{
		m_Stem[i].Initialize(m_D3D->GetDevice(), "Data/Stem.txt", "Data/leaf.dds",  D3DXVECTOR3(0,0,0), D3DXVECTOR3(0, stem_pos, 0),
			D3DXVECTOR3(0,0,0));
		stem_pos += 3;
	}

	float x_rot = rand() % 360 + 0;
	for (int i = 0; i < num_leaves; i++)
	{
		float rot_num = rand() % 10 + -10;
		m_Leaf[i].Initialize(m_D3D->GetDevice(), "Data/Leaf2.txt", "Data/leaf.dds", 
			 D3DXVECTOR3(0, x_rot, 0), D3DXVECTOR3(0, 3, 0),
			D3DXVECTOR3(1, 1, 1));
		m_Leaf[i].SetScale(1, 1, 1);
		x_rot -= 360 / num_leaves;
	}

	x_rot = rand() % 360 + 0;
	int flower_rot = rand() % 360 + 0;
	float rand_scale = rand() % 7 + 1;
	rand_scale = (rand_scale / 10) + 0.5f;
	for (int i = 0; i < num_petals; i++)
	{
		m_Petal[i].Initialize(m_D3D->GetDevice(), "Data/Petal.txt", "Data/petal.dds",  D3DXVECTOR3(30, flower_rot, x_rot), D3DXVECTOR3(0, 6, 0),
			D3DXVECTOR3(1, 1, 1));
		x_rot -= (360 / num_petals) + 1;
	}
	return true;
}

void GraphicsClass::Shutdown()
{
	ImGui_ImplDX11_Shutdown();

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	if (reset_leaves)
	{
		delete reset_leaves;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}
	// Release the bitmap object.
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}


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

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if (m_D3D)
	{
		// On shutdown run D3D shutdown function and delete class instance
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}
	return;
}


bool GraphicsClass::Frame(int mouse_x, int mouse_y)
{
	bool result;
	static float rotation = 0.0f;
	// UPdate the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	// Call the render function each frame
	result = Render(rotation);
	if (!result)
	{
		return false;
	}


	return true;
}


bool GraphicsClass::Render(float rotation)
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix, orthoMatrix, rotMatrix;
	D3DXVECTOR3 cam_rotation;
	bool result;
	//m_Leaf[0].GetWorldMatrix()
	float yaw, pitch, roll;
	cam_rotation = m_Camera->GetRotation();


	pitch = cam_rotation.x * 0.0174532925f;
	yaw = cam_rotation.y * 0.0174532925f;
	roll = cam_rotation.z * 0.0174532925f;

	D3DXMatrixRotationYawPitchRoll(&rotMatrix, yaw, pitch, roll);

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.1f, 0.1f, 0.1f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	for (int i = 0; i < num_stems; i++)
	{
		m_Stem[i].Render(m_D3D->GetDeviceContext());
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Stem[i].GetIndexCount(), m_Stem[i].GetWorldMatrix(), viewMatrix,
			projectionMatrix, m_Stem[i].GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColour(), m_Light->GetDiffuseColour());
	}
	for (int i = 0; i < num_leaves; i++)
	{
		m_Leaf[i].Render(m_D3D->GetDeviceContext());
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Leaf[i].GetIndexCount(), m_Leaf[i].GetWorldMatrix(), viewMatrix,
			projectionMatrix, m_Leaf[i].GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColour(), m_Light->GetDiffuseColour());
	}
	for (int i = 0; i < num_petals; i++)
	{
		m_Petal[i].Render(m_D3D->GetDeviceContext());
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Petal[i].GetIndexCount(), m_Petal[i].GetWorldMatrix(), viewMatrix,
			projectionMatrix, m_Petal[i].GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColour(), m_Light->GetDiffuseColour());
	}
	if (!result)
	{
		return false;
	}

	m_Stem->Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Stem->GetIndexCount(), m_Stem->GetWorldMatrix(), viewMatrix,
		projectionMatrix, m_Stem->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColour(), m_Light->GetDiffuseColour());
							
	if (!result)
	{
		return false;
	}

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();
	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//result = m_Bitmap->Render(m_D3D->GetDeviceContext(), 100, 100);
	if (!result)
	{
		return false;
	}

	// Render the bitmap with the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), rotMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
	if (!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	static float transformOffset[3] = { 0, 0, 0 };

	for (int i = 0; i < num_leaves; i++)
	{
		m_Leaf[i].SetRot(m_Leaf[i].GetRotation().x + transformOffset[0], m_Leaf[i].GetRotation().y + transformOffset[1], m_Leaf[i].GetRotation().z + transformOffset[2]);
	}

	if (!reset_leaves)
	{
		reset_leaves = new int(0);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Test");
	if (ImGui::Button("Refresh"))
	{
		SetupPlant();
	}
	ImGui::SameLine();
	ImGui::Text("Refreshes Plant Setup");
	ImGui::DragFloat3("Leaf Rotation", transformOffset, 0.1f, -45.0f, 45.0f);
	ImGui::DragInt("Number of Leaves", reset_leaves, 0.1f, 0.0f, 5.0f);
	if (ImGui::Button("Write to file"))
	{
		WriteToFile();
	}
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	// Present the rendered scene to the screen
	m_D3D->EndScene();
	return true;
}

void GraphicsClass::SetCamPos(float x, float y, float z)
{
	m_Camera->SetPosition(x, y, z);
}

void GraphicsClass::CamPosX(float x)
{
	m_Camera->SetPosition(m_Camera->GetPosition().x + x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);
}

void GraphicsClass::CamPosY(float y)
{
	m_Camera->SetPosition(m_Camera->GetPosition().x, m_Camera->GetPosition().y + y, m_Camera->GetPosition().z);
}

void GraphicsClass::CamPosZ(float z)
{
	m_Camera->SetPosition(m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z + z);
}

void GraphicsClass::CamRotY(float x)
{
	m_Camera->SetRotation(m_Camera->GetRotation().x + x, m_Camera->GetRotation().y, m_Camera->GetRotation().z);
}

void GraphicsClass::CamRotX(float y)
{
	m_Camera->SetRotation(m_Camera->GetRotation().x, m_Camera->GetRotation().y + y, m_Camera->GetRotation().z);
}

void GraphicsClass::WriteToFile()
{
	ofstream output_file;
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
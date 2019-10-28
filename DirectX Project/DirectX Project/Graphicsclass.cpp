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
	m_Light = 0;
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

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	result = SetupLeaves();
	// Initialize the model object with the texture that will be used.
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the leaves.", "Error", MB_OK);
		return false;
	}
	

	m_Crate = new ModelClass;
	if (!m_Crate)
	{
		return false;
	}

	result = m_Crate->Initialize(m_D3D->GetDevice(), "Data/Cube.txt", "Data/crate.dds");
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
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
	m_Light->SetDirection(0.0f, -1.0f, 1.0f);

	return true;
}

bool GraphicsClass::SetupLeaves()
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
		m_Stem->Shutdown();
		delete m_Stem;
		m_Stem = 0;
	}

	srand(time(NULL));
	float stem_rot = rand() % 360 + 0;
	m_Stem = new ModelClass;
	m_Stem->Initialize(m_D3D->GetDevice(), "Data/Stem.txt", "Data/leaf.dds");
	m_Stem->SetRot(0, stem_rot, 0);

	num_leaves = rand() % 4 + 2;
	m_Leaf = new ModelClass[num_leaves];

	if (!m_Leaf)
	{
		return false;
	}

	float x_rot = rand() % 360 + 0;
	for (int i = 0; i < num_leaves; i++)
	{
		float rot_num = rand() % 10 + -10;
		m_Leaf[i].Initialize(m_D3D->GetDevice(), "Data/Leaf2.txt", "Data/leaf.dds");
		m_Leaf[i].SetPos(0, -3, 0);
		m_Leaf[i].SetRot(0, x_rot, 0);
		x_rot -= (360 / num_leaves) + (rand() % (50 / num_leaves) + -(50 / num_leaves));
	}
	return true;
}

void GraphicsClass::Shutdown()
{
	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
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
		m_Stem->Shutdown();
		delete m_Stem;
		m_Stem = 0;
	}

	if (m_Crate)
	{
		m_Crate->Shutdown();
		delete m_Crate;
		m_Crate = 0;
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


bool GraphicsClass::Frame()
{
	bool result;
	static float rotation = 0.0f;
	D3DXMATRIX leafMatrix;
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
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.1f, 0.1f, 0.1f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);


	for (int i = 0; i < num_leaves; i++)
	{
		m_Leaf[i].Render(m_D3D->GetDeviceContext());
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Leaf[i].GetIndexCount(), m_Leaf[i].GetWorldMatrix(), viewMatrix,
			projectionMatrix, m_Leaf[i].GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColour(), m_Light->GetDiffuseColour());
	}
	if (!result)
	{
		return false;
	}

	m_Stem->Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Stem->GetIndexCount(), m_Stem->GetWorldMatrix(), viewMatrix,
		projectionMatrix, m_Stem->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColour(), m_Light->GetDiffuseColour());

	m_Crate->SetPos(0, 0, 0);
	//m_Crate->Render(m_D3D->GetDeviceContext());
	//result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Crate->GetIndexCount(), m_Crate->GetWorldMatrix(), viewMatrix,
	//								projectionMatrix, m_Crate->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColour(), m_Light->GetDiffuseColour());
	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen
	m_D3D->EndScene();

	m_Camera->GetPosition().x;

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
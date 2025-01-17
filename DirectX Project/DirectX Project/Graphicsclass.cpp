////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_LightShader = 0;
	m_TextureShader = 0;
	m_Light = 0;
	m_Bitmap = 0;
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

	flower = new Plant;
	flower->Setup(Type::FLOWER,m_D3D->GetDevice());
	vine = new Plant;
	vine->Setup(Type::VINE, m_D3D->GetDevice());
	bamboo = new Plant;
	bamboo->Setup(Type::BAMBOO, m_D3D->GetDevice());
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

void GraphicsClass::Shutdown()
{
	ImGui_ImplDX11_Shutdown();

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
	// Release the bitmap object.
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}


	if (flower)
	{
		flower->Shutdown();
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
	rotation += (float)XM_PI * 0.005f;
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
	if (type == Type::FLOWER)
	{
		flower->Update(Type::FLOWER);
	}
	else if (type == Type::VINE)
	{
		vine->Update(Type::VINE);
	}
	else if (type == Type::BAMBOO)
	{
		bamboo->Update(Type::BAMBOO);
	}

	return true;
}


bool GraphicsClass::Render(float rotation)
{
	XMMATRIX viewMatrix, projectionMatrix, worldMatrix, orthoMatrix;
	XMFLOAT3 cam_rotation;
	bool result = false;
	float yaw, pitch, roll;
	cam_rotation = m_Camera->Rotation();


	pitch = cam_rotation.x * 0.0174532925f;
	yaw = cam_rotation.y * 0.0174532925f;
	roll = cam_rotation.z * 0.0174532925f;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.1f, 0.1f, 0.1f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->UpdateCam();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);						

	if (type == Type::FLOWER)
	{
		flower->Render(m_D3D->GetDeviceContext(), m_LightShader, m_Light, viewMatrix, projectionMatrix);
	}
	if (type == Type::VINE)
	{
		vine->Render(m_D3D->GetDeviceContext(), m_LightShader, m_Light, viewMatrix, projectionMatrix);
	}
	if (type == Type::BAMBOO)
	{
		bamboo->Render(m_D3D->GetDeviceContext(), m_LightShader, m_Light, viewMatrix, projectionMatrix);
	}
	GUI();

	// Present the rendered scene to the screen
	m_D3D->EndScene();
	return true;
}

void GraphicsClass::GUI()
{
	if (type == Type::FLOWER)
	{
		static float leaf_rot[3] = { 0, 0, 0 };
		static float leaf_scl[3] = { 1,1,1 };
		static float petal_rot[3] = { 0, 0, 0 };
		static float petal_scl[2] = { 1,1 };
		static float stem_scl[3] = { 1,1,1 };
		static float stem_rot[3] = { 0, 0 };
		int reset_leaves = flower->Leaves();
		int reset_petals = flower->Petals();
		int reset_stems = flower->Stems();


		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Test");
		if (ImGui::Button("Vine"))
		{
			type = Type::VINE;
		}
		ImGui::SameLine();
		if (ImGui::Button("Bamboo"))
		{
			type = Type::BAMBOO;
		}
		ImGui::DragInt("Number of Leaves", &reset_leaves, 0.1f, 0.0f, 5.0f);
		ImGui::DragFloat3("Leaf Rotation", leaf_rot, 0.1f, -45.0f, 45.0f);
		ImGui::DragFloat3("Leaf Scale", leaf_scl, 0.1f, 1.0f, 3.0f);
		ImGui::DragInt("Number of Petals", &reset_petals, 0.1f, 3.0f, 7.0f);
		ImGui::DragFloat3("Petal Rotation", petal_rot, 0.1f, 0.0f, 360.0f);
		ImGui::DragFloat2("Petal Scale", petal_scl, 0.1f, 1.0f, 3.0f);
		ImGui::DragInt("Stem Segments", &reset_stems, 0.1f, 1.0f, 5.0f);
		ImGui::DragFloat2("Stem Rotation", stem_rot, 0.1f, -35.0f, 35.0f);
		ImGui::DragFloat3("Stem Scale", stem_scl, 0.1f, 0.1f, 3.0f);
		if (ImGui::Button("Export"))
		{
			flower_file_name = flower->WriteToFile(Type::FLOWER);
		}
		if (flower_file_name != "")
		{
			ImGui::SameLine();
			std::string txt = "Saved as: " + flower_file_name;
			ImGui::Text(txt.c_str());
		}
		ImGui::End();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		flower->SetNum(Type::FLOWER,m_D3D->GetDevice(), Element::LEAF, &reset_leaves);
		flower->SetNum(Type::FLOWER,m_D3D->GetDevice(), Element::PETAL, &reset_petals);
		flower->SetNum(Type::FLOWER,m_D3D->GetDevice(), Element::STEM, &reset_stems);

		flower->SetRot(Type::FLOWER, Element::LEAF, XMFLOAT3{ leaf_rot[0] , leaf_rot[1] , leaf_rot[2] });
		flower->SetRot(Type::FLOWER, Element::PETAL, XMFLOAT3{ petal_rot[0] , petal_rot[1] , petal_rot[2] });
		flower->SetRot(Type::FLOWER, Element::STEM, XMFLOAT3{ 0, stem_rot[0], stem_rot[1] });

		flower->SetScl(Element::LEAF, XMFLOAT3{ leaf_scl[0] , leaf_scl[1] , leaf_scl[2] });
		flower->SetScl(Element::PETAL, XMFLOAT3{ petal_scl[0] , petal_scl[1] , 1 });
		flower->SetScl(Element::STEM, XMFLOAT3{ stem_scl[0] , stem_scl[1] , stem_scl[2] });
	}
	else if (type == Type::VINE)
	{
		static float leaf_rot[2] = { 0, 0 };
		static float leaf_scl[2] = { 1, 1 };
		static float stem_scl[3] = { 1,1,1 };
		static float stem_rot[3] = { 0, 0 };
		int reset_leaves = vine->Leaves();
		int reset_stems = vine->Stems();

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Test");
		if (ImGui::Button("Flower"))
		{
			type = Type::FLOWER;
		}
		ImGui::SameLine();
		if (ImGui::Button("Bamboo"))
		{
			type = Type::BAMBOO;
		}
		ImGui::DragInt("Stem Segments", &reset_stems, 0.1f, 1.0f, 25.0f);
		ImGui::DragFloat("Curve", &stem_rot[2], 0.1f, -35.0f, 35.0f);
		ImGui::DragFloat3("Stem Scale", stem_scl, 0.1f, 0.1f, 3.0f);
		ImGui::DragFloat2("Leaf Rotation", leaf_rot, 0.1f, -180.0f, 180.0f);
		ImGui::DragFloat2("Leaf Scale", leaf_scl, 0.1f, 0.1f, 3.0f);
		if (ImGui::Button("Export"))
		{
			vine_file_name = vine->WriteToFile(Type::VINE);
		}
		if (vine_file_name != "")
		{
			std::string txt = "Saved as: " + vine_file_name;
			ImGui::SameLine();
			ImGui::Text(txt.c_str());
		}
		ImGui::End();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		vine->SetNum(type, m_D3D->GetDevice(), Element::STEM, &reset_stems);
		vine->SetRot(type,Element::STEM, XMFLOAT3{ stem_rot[0], stem_rot[1], stem_rot[2] });
		vine->SetScl(Element::STEM, XMFLOAT3{ stem_scl[0] , stem_scl[1] , stem_scl[2] });
		vine->SetRot(type, Element::LEAF, XMFLOAT3{ leaf_rot[0], leaf_rot[1], 0 });
		vine->SetScl(Element::LEAF, XMFLOAT3{ leaf_scl[0] , 0 , leaf_scl[1] });
	}
	else if (type == Type::BAMBOO)
	{
		static float leaf_rot[3] = { 0, 0, 0 };
		static float leaf_scl[3] = { 1,1,1 };
		static float stem_scl[3] = { 1,1,1 };
		static float stem_rot[3] = { 0, 0 };
		int reset_leaves = bamboo->Leaves();
		int reset_stems = bamboo->Stems();

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Test");
		if (ImGui::Button("Flower"))
		{
			type = Type::FLOWER;
		}
		ImGui::SameLine();
		if (ImGui::Button("Vine"))
		{
			type = Type::VINE;
		}

		ImGui::DragInt("Stem Segments", &reset_stems, 0.1f, 1.0f, 5.0f);
		ImGui::DragFloat3("Stem Scale", stem_scl, 0.1f, 0.1f, 5.0f);
		ImGui::DragInt("Number of Leaves", &reset_leaves, 0.1f, 0.0f, bamboo->Stems() * 2);
		ImGui::DragFloat3("Leaf Rotation", leaf_rot, 0.1f, -180.0f, 180.0f);
		ImGui::DragFloat3("Leaf Scale", leaf_scl, 0.1f, 0.1f, 3.0f);
		if (ImGui::Button("Export"))
		{
			bamboo_file_name = bamboo->WriteToFile(Type::BAMBOO);
		}
		if (bamboo_file_name != "")
		{
			ImGui::SameLine();
			std::string txt = "Saved as: " + bamboo_file_name;
			ImGui::Text(txt.c_str());
		}
		ImGui::End();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		bamboo->SetNum(type, m_D3D->GetDevice(), Element::STEM, &reset_stems);
		bamboo->SetScl(Element::STEM, XMFLOAT3{ stem_scl[0] , stem_scl[1] , stem_scl[2] });
		bamboo->SetNum(type, m_D3D->GetDevice(), Element::LEAF, &reset_leaves);
		bamboo->SetRot(type, Element::LEAF, XMFLOAT3{ leaf_rot[0], leaf_rot[1], leaf_rot[2] });
		bamboo->SetScl(Element::LEAF, XMFLOAT3{ leaf_scl[0] , leaf_scl[1] , leaf_scl[2] });
	}

}

void GraphicsClass::UpdateCamRot(XMFLOAT2 rot)
{
	m_Camera->Rotation().x += rot.x;
	if (m_Camera->Radius() + rot.y > 5 && 
		m_Camera->Radius() + rot.y < 150)
	{
		m_Camera->Radius() += rot.y;
	}
}
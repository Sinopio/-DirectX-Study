#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_LightShader = 0;
	m_Light = 0;
	m_TextureShader = 0;
	m_Bitmap = 0;
	m_Text = 0;

	cam_y_pos = 2;
	polygon = 0;
	//스카이돔
	m_SkyDome = 0;
	m_SkyDomeShader = 0;

	on_fire = false;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}

GraphicsClass::~GraphicsClass()
{
}

//초기화 함수
bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	D3DXMATRIX baseViewMatrix;

	// Direct3D 객체 생성
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Direct3D 객체 초기화
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// 카메라 오브젝트 생성
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// 카메라의 기본 위치 설정
	m_Camera->SetPosition(0.0f, 0.0f, -20.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// 모델 오브젝트 생성
	m_Model = new ModelClass[21];
	if (!m_Model)
	{
		return false;
	}

	// 모델 오브젝트 초기화
	result = m_Model[0].Initialize(m_D3D->GetDevice(), (char *)"../Graphics/data/platform.obj", (WCHAR *)L"../Graphics/data/Ground.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[1].Initialize(m_D3D->GetDevice(), (char *)"../Graphics/data/Building_21.obj", (WCHAR *)L"../Graphics/data/Red_Block.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[2].Initialize(m_D3D->GetDevice(), (char *)"../Graphics/data/Window_21.obj", (WCHAR *)L"../Graphics/data/Glass1_1.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[3].Initialize(m_D3D->GetDevice(), (char *)"../Graphics/data/Road_21.obj", (WCHAR *)L"../Graphics/data/Road.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[4].Initialize(m_D3D->GetDevice(), (char *)"../Graphics/data/Tree_B_22.obj", (WCHAR *)L"../Graphics/data/Tree.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[5].Initialize(m_D3D->GetDevice(), (char *)"../Graphics/data/Tree_L_22.obj", (WCHAR *)L"../Graphics/data/Leaf.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[6].Initialize(m_D3D->GetDevice(), (char *)"../Graphics/data/Car_Body_17.obj", (WCHAR *)L"../Graphics/data/Car_Paint_1.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[7].Initialize(m_D3D->GetDevice(), (char *)"../Graphics/data/Car_Metal_17.obj", (WCHAR *)L"../Graphics/data/wall.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[8].Initialize(m_D3D->GetDevice(), (char *)"../Graphics/data/Car_Wheel_17.obj", (WCHAR *)L"../Graphics/data/Tire.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[9].Initialize(m_D3D->GetDevice(), (char *)"../Graphics/data/Car_Window_17.obj", (WCHAR *)L"../Graphics/data/Glass1_1.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[10].Initialize(m_D3D->GetDevice(), (char *)"../Graphics/data/Car_BodyM_17.obj", (WCHAR *)L"../Graphics/data/Car_Paint_1.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[11].Initialize(m_D3D->GetDevice(), (char *)"../Graphics/data/Car_MetalM_17.obj", (WCHAR *)L"../Graphics/data/wall.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[12].Initialize(m_D3D->GetDevice(), (char *)"../Graphics/data/Car_WheelM_17.obj", (WCHAR *)L"../Graphics/data/Tire.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[13].Initialize(m_D3D->GetDevice(), (char *)"../Graphics/data/Car_WindowM_17.obj", (WCHAR *)L"../Graphics/data/Glass1_1.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[14].Initialize(m_D3D->GetDevice(), (char *)"../Graphics/data/Ground_21.obj", (WCHAR *)L"../Graphics/data/Block.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[15].Initialize(m_D3D->GetDevice(), (char *)"../Graphics/data/Stair_21.obj", (WCHAR *)L"../Graphics/data/Block.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[16].Initialize(m_D3D->GetDevice(), (char *)"../Graphics/data/tuition_22.obj", (WCHAR *)L"../Graphics/data/Car_Paint_1.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[17].Initialize_f(m_D3D->GetDevice(), (char *)"../Graphics/data/Fire_22.obj", (WCHAR *)L"../Graphics/data/fire01.dds",
									(WCHAR *)L"../Graphics/data/noise01.dds", (WCHAR *)L"../Graphics/data/alpha01.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[18].Initialize_f(m_D3D->GetDevice(), (char *)"../Graphics/data/On_Fire_22.obj", (WCHAR *)L"../Graphics/data/fire01.dds",
		(WCHAR *)L"../Graphics/data/noise01.dds", (WCHAR *)L"../Graphics/data/alpha01.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[19].Initialize(m_D3D->GetDevice(), (char *)"../Graphics/data/UFO.obj", (WCHAR *)L"../Graphics/data/Metal.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[20].Initialize_f(m_D3D->GetDevice(), (char *)"../Graphics/data/UFO_Ray.obj", (WCHAR *)L"../Graphics/data/Ray.dds",
		(WCHAR *)L"../Graphics/data/noise01.dds", (WCHAR *)L"../Graphics/data/alpha01.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	for (int i = 0; i < 21; i++)
	{
		polygon += m_Model[i].GetIndexCount();
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}
	// Initialize the light shader object
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.5f, 0.5f, 0.5f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, 0.0f, 0.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(320.0f);

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	if (!m_Bitmap)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, (WCHAR *)L"../Graphics/data/seafloor.dds", 256, 256);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	//스카이돔
	// Create the sky dome object.
	m_SkyDome = new SkyDomeClass;
	if (!m_SkyDome)
	{
		return false;
	}

	// Initialize the sky dome object.
	result = m_SkyDome->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome object.", L"Error", MB_OK);
		return false;
	}

	// Create the sky dome shader object.
	m_SkyDomeShader = new SkyDomeShaderClass;
	if (!m_SkyDomeShader)
	{
		return false;
	}

	// Initialize the sky dome shader object.
	result = m_SkyDomeShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the fire shader object.
	m_FireShader = new FireShaderClass;
	if (!m_FireShader)
	{
		return false;
	}

	// Initialize the fire shader object.
	result = m_FireShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the fire shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

//모든 그래픽객체의 해제 담당
void GraphicsClass::Shutdown()
{
	if (m_FireShader)
	{
		m_FireShader->Shutdown();
		delete m_FireShader;
		m_FireShader = 0;
	}

	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}
	// Release the bitmap object.
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	//D3D 객체를 반환
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

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
	if (m_Model)
	{
		m_Model->Shutdown();
		delete[] m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	//스카이돔
	// Release the sky dome shader object.
	if (m_SkyDomeShader)
	{
		m_SkyDomeShader->Shutdown();
		delete m_SkyDomeShader;
		m_SkyDomeShader = 0;
	}

	// Release the sky dome object.
	if (m_SkyDome)
	{
		m_SkyDome->Shutdown();
		delete m_SkyDome;
		m_SkyDome = 0;
	}

	return;
}

//매 프레임마다 Render함수 호출
bool GraphicsClass::Frame(int fps, int cpu, float frameTime, int screenwidth, int screenheight) //20200621
{
	bool result;

	// Set the frames per second.
	result = m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Set the cpu usage.
	result = m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = m_Text->Count_polygons(polygon, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = m_Text->SetScreen(screenwidth, screenheight, m_D3D->GetDeviceContext()); //20200621
	if (!result)
	{
		return false;
	}

}

void GraphicsClass::Camera_move(int view)
{
	if (view == 11 || view == 12)
	{
		if (view == 11)
		{
			z_move = 1.0f;
		}
		else if (view == 12)
		{
			z_move = -1.0f;
		}
	}
	else
	{
		z_move = 0.0f;
	}

	if (view == 13 || view == 14) {
		if (view == 13)
		{
			x_move = -1.0f;
		}
		else if (view == 14)
		{
			x_move = 1.0f;
		}
	}
	else
	{
		x_move = 0.0f;
	}

	if (view == 20 || view == 21) {
		if (view == 20)
		{
			cam_y_pos += -1.0f;
		}
		else if (view == 21)
		{
			cam_y_pos += 1.0f;
		}
	}

	if (view == 0 || view == 1) {
		if (view == 0)
		{
			on_fire = false;
		}
		else if (view == 1)
		{
			on_fire = true;
		}
	}

	if (view == 15)
	{
		m_SkyDome->change_Night();
		m_Light->SetAmbientColor(0.5f, 0.5f, 0.5f, 1.0f);
		m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_Light->SetDirection(1.0f, 0.0f, 0.0f);
		m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_Light->SetSpecularPower(320.0f);
	}

	if (view == 16)
	{
		m_SkyDome->change_Day();
		m_Light->SetAmbientColor(0.1f, 0.1f, 0.1f, 1.0f);
		m_Light->SetDiffuseColor(0.5f, 0.5f, 0.5f, 1.0f);
		m_Light->SetDirection(0.0f, 0.0f, 0.0f);
		m_Light->SetSpecularColor(0.3f, 0.3f, 0.3f, 1.0f);
		m_Light->SetSpecularPower(3000000000.0f);
	}
}

void GraphicsClass::mouse_move_position()
{
	camera_movex += z_move * sin(m_Camera->return_rotation() * 0.0174532925f);
	camera_movex += x_move * cos(m_Camera->return_rotation() * 0.0174532925f);

	camera_movez += z_move * cos(m_Camera->return_rotation() * 0.0174532925f);
	camera_movez += x_move * -sin(m_Camera->return_rotation() * 0.0174532925f);
}

CameraClass * GraphicsClass::get_came()
{
	return m_Camera;
}

bool GraphicsClass::Render(float rotation)
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix, orthoMatrix;
	D3DXMATRIX car_moveMatrix;
	//스카이돔
	D3DXVECTOR3 cameraPosition;
	bool result;

	D3DXVECTOR3 scrollSpeeds, scales;
	D3DXVECTOR2 distortion1, distortion2, distortion3;
	float distortionScale, distortionBias;
	static float frameTime = 0.0f;

	// Increment the frame time counter.
	frameTime += 0.01f;
	if (frameTime > 1000.0f)
	{
		frameTime = 0.0f;
	}

	// Set the three scrolling speeds for the three different noise textures.
	scrollSpeeds = D3DXVECTOR3(1.3f, 2.1f, 2.3f);

	// Set the three scales which will be used to create the three different noise octave textures.
	scales = D3DXVECTOR3(1.0f, 2.0f, 3.0f);

	// Set the three different x and y distortion factors for the three different noise textures.
	distortion1 = D3DXVECTOR2(0.1f, 0.2f);
	distortion2 = D3DXVECTOR2(0.1f, 0.3f);
	distortion3 = D3DXVECTOR2(0.1f, 0.1f);

	// The the scale and bias of the texture coordinate sampling perturbation.
	distortionScale = 0.8f;
	distortionBias = 0.5f;


	if (move_point < 55.0f)
	{
		move_point += 0.3f;
	}

	if (move_point >= 50.0f)
	{
		move_point = -50.0f;
	}
	// 씬 그리기를 시작하기 위해 버퍼의 내용을 지웁니다.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	m_Camera->Render();

	m_Camera->GetViewMatrix(viewMatrix);

	//mouse_move_position(); //
	camera_movex += z_move * sin(m_Camera->return_rotation() * 0.0174532925f);
	camera_movex += x_move * cos(m_Camera->return_rotation() * 0.0174532925f);

	camera_movez += z_move * cos(m_Camera->return_rotation() * 0.0174532925f);
	camera_movez += x_move * -sin(m_Camera->return_rotation() * 0.0174532925f);

	m_Camera->SetPosition(camera_movex, cam_y_pos, camera_movez);
	// 카메로 포지션으로 뷰 매트릭스 생성

	// 차 월드 매트릭스 이동
	D3DXMatrixTranslation(&car_moveMatrix, move_point, 0.0f, 0.0f);

	// D3DClass 객체로부터 월드행렬과 투영 행렬을 복사
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	//스카이돔
	// Get the position of the camera.
	cameraPosition = m_Camera->GetPosition();

	// Translate the sky dome to be centered around the camera position.
	D3DXMatrixTranslation(&worldMatrix, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	// Turn off back face culling.
	m_D3D->TurnOffCulling();

	// Turn off the Z buffer.
	m_D3D->TurnZBufferOff();

	// Render the sky dome using the sky dome shader.
	m_SkyDome->Render(m_D3D->GetDeviceContext());
	m_SkyDomeShader->Render(m_D3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor());

	// Turn back face culling back on.
	m_D3D->TurnOnCulling();

	// Turn the Z buffer back on.
	m_D3D->TurnZBufferOn();

	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);

	m_D3D->TurnZBufferOff();
	m_D3D->TurnOnAlphaBlending();

	result = m_Bitmap->Render(m_D3D->GetDeviceContext(), 200, 200);
	if (!result)
	{
		return false;
	}

	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
	if (!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// 그래픽 파이프라인에 삼각형 모델을 그리게 한다.
	m_Model[0].Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[0].GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model[0].GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	m_Model[1].Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[1].GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model[1].GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	m_Model[2].Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[2].GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model[2].GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	m_Model[3].Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[3].GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model[3].GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	m_Model[4].Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[4].GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model[4].GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	m_Model[5].Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[5].GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model[5].GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	m_Model[6].Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[6].GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model[6].GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	m_Model[7].Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[7].GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model[7].GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}


	m_Model[8].Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[8].GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model[8].GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}


	m_Model[9].Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[9].GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model[9].GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	m_Model[10].Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[10].GetIndexCount(), car_moveMatrix, viewMatrix, projectionMatrix,
		m_Model[10].GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	m_Model[11].Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[11].GetIndexCount(), car_moveMatrix, viewMatrix, projectionMatrix,
		m_Model[11].GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	m_Model[12].Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[12].GetIndexCount(), car_moveMatrix, viewMatrix, projectionMatrix,
		m_Model[12].GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	m_Model[13].Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[13].GetIndexCount(), car_moveMatrix, viewMatrix, projectionMatrix,
		m_Model[13].GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	m_Model[14].Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[14].GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model[14].GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	m_Model[15].Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[15].GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model[15].GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	m_Model[16].Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[16].GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model[16].GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}


	m_Model[17].Render(m_D3D->GetDeviceContext());
	result = m_FireShader->Render(m_D3D->GetDeviceContext(), m_Model[17].GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model[17].GetTexture1(), m_Model[17].GetTexture2(), m_Model[17].GetTexture3(), frameTime, scrollSpeeds,
		scales, distortion1, distortion2, distortion3, distortionScale, distortionBias);
	if (!result)
	{
		return false;
	}

	if (on_fire)
	{
		m_Model[18].Render(m_D3D->GetDeviceContext());
		result = m_FireShader->Render(m_D3D->GetDeviceContext(), m_Model[18].GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
			m_Model[18].GetTexture1(), m_Model[18].GetTexture2(), m_Model[18].GetTexture3(), frameTime, scrollSpeeds,
			scales, distortion1, distortion2, distortion3, distortionScale, distortionBias);
		if (!result)
		{
			return false;
		}

		m_Model[19].Render(m_D3D->GetDeviceContext());
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[19].GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
			m_Model[19].GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
		if (!result)
		{
			return false;
		}

		m_Model[20].Render(m_D3D->GetDeviceContext());
		result = m_FireShader->Render(m_D3D->GetDeviceContext(), m_Model[20].GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
			m_Model[20].GetTexture1(), m_Model[20].GetTexture2(), m_Model[20].GetTexture3(), frameTime, scrollSpeeds,
			scales, distortion1, distortion2, distortion3, distortionScale, distortionBias);
		if (!result)
		{
			return false;
		}
	}


	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	// 다 그려졌다면 회면에 표시해준다.
	m_D3D->EndScene();

	return true;
}


#include "systemclass.h"

SystemClass::SystemClass()
{
	m_Input = 0;
	m_Graphics = 0;
	m_Fps = 0;
	m_Cpu = 0;
	m_Timer = 0;
	rotation = 0;
	isAmbient = 1;
	isDiffuse = 1;
	isDirection = 1;
	m_Sound = 0;

	m_CarSound = 0; //20200618
	IsCar = false; //20200618
}

SystemClass::SystemClass(const SystemClass& other)
{
}

SystemClass::~SystemClass()
{
}

//��� �ʱ�ȭ�۾� ����
bool SystemClass::Initialize()
{
	//��ũ�� ����, ����
	int screenWidth, screenHeight;
	bool result;

	// �Լ��� �ʺ�� ���̸� ���ϱ����� �ʱ�ȭ
	screenWidth = 0;
	screenHeight = 0;

	// windows api�� �̿��Ͽ� �ʱ�ȭ
	InitializeWindows(screenWidth, screenHeight);

	//20200621
	get_width = screenWidth;
	get_height = screenHeight;

	// Ű���� �Է��� �޴� Input��ü ����
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	// Input ��ü �ʱ�ȭ
	result = m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}


	// ���ø����̼��� ��� �׷��ȿ�Ҹ� �׸��� Graphics��ü ����
	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	// Graphics��ü �ʱ�ȭ
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	// Create the fps object.
	m_Fps = new FpsClass;
	if (!m_Fps)
	{
		return false;
	}

	// Initialize the fps object.
	m_Fps->Initialize();

	// Create the cpu object.
	m_Cpu = new CpuClass;
	if (!m_Cpu)
	{
		return false;
	}

	// Initialize the cpu object.
	m_Cpu->Initialize();

	// Create the timer object.
	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the Timer object.", L"Error", MB_OK);
		return false;
	}

	m_Sound = new SoundClass;
	if (!m_Sound)
	{
		return false;
	}

	// Initialize the sound object.
	result = m_Sound->Initialize(m_hwnd);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize Direct Sound.", L"Error", MB_OK);
		return false;
	}

	m_CarSound = new SoundClass;
	if (!m_CarSound)
	{
		return false;
	}

	// Initialize the sound object.
	result = m_CarSound->Initialize(m_hwnd);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize Direct Sound.", L"Error", MB_OK);
		return false;
	}


	return true;
}

// ��ü�� �����ϴ� �Լ�, ���� Graphics�� Input ��ü ��ȯ
void SystemClass::Shutdown()
{
	if (m_Sound) // 20200613 �߰�
	{
		m_Sound->Shutdown();
		delete m_Sound;
		m_Sound = 0;
	}

	if (m_CarSound) // 20200613 �߰�
	{
		m_CarSound->Shutdown();
		delete m_CarSound;
		m_CarSound = 0;
	}

	// Release the timer object.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the cpu object.
	if (m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	// Release the fps object.
	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	// Graphics ��ü ��ȯ
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Input ��ü ��ȯ
	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	// â�� �����Ų��.
	ShutdownWindows();

	return;
}

//�Լ��� ����� �� ���� ������ ���鼭 ���ø����̼��� �۾��� ó��
//�۾��� Frame�Լ����� ����
void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	// msg(�޼���) ����ü�� �ʱ�ȭ
	ZeroMemory(&msg, sizeof(MSG));

	// �����Է��� ���� ������ ���ø� ����.
	done = false;
	while (!done)
	{
		// ������ �޼����� ó��
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// �����Է��� ������� ����������(����?)
		if (msg.message == WM_QUIT)
		{
			done = true;
		}

		else
		{
			// �� ���� ��쿡�� Frame�Լ��� ó��
			result = Frame();
			if (!result)
			{
				done = true;
			}

			if (!m_Input->IsFkeyPressed()) //20200618
			{
				if (!IsCar)
				{
					m_CarSound->PlayCaralram();
				}
				IsCar = true;
			}
			else
			{
				IsCar = false;
			}
		}

		// Check if the user pressed escape and wants to quit.
		if (m_Input->IsEscapePressed() == true)
		{
			done = true;
		}
	}

	return;
}

//���ø����̼��� �۾��� ó���Ǵ� ��
//Input��ü�� ���� �÷��̾ Esc �� ���� �����Ű���� Ȯ��
//�����Է��� ���ٸ� Graphics ��ü�� ���ؼ� ȭ���� �׸��� �۾��� ����
bool SystemClass::Frame()
{
	bool result;
	int mouseX, mouseY;

	rotation += (float)D3DX_PI * 0.00005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Update the system stats.
	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();
	move_Camera();
	Set_Rotation_Camera();
	result = m_Input->Frame();
	if (!result)
	{
		return false;
	}

	m_Input->GetMouseLocation(mouseX, mouseY);

	// ������ Esc��ư�� �������� Ȯ��
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	// Graphics��ü�� �۾��� ó��
	result = m_Graphics->Frame(m_Fps->GetFps(), m_Cpu->GetCpuPercentage(), m_Timer->GetTime(), get_width, get_height);
	if (!result)
	{
		return false;
	}

	// Finally render the graphics to the screen.
	result = m_Graphics->Render(rotation);
	if (!result)
	{
		return false;
	}

	return true;
}

//�������� �ý��� �޼����� ����
LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// Ű���尡 ���ȴ��� Ȯ��
	case WM_KEYDOWN:
	{
		// Ű���尡 ���ȴٸ� Input��ü�� ����
		m_Input->KeyDown((unsigned int)wparam);
		return 0;
	}
	// Ű���尡 ���ȴٰ� ���������� Ȯ��(KeyUP?)
	case WM_KEYUP:
	{
		// Ű�� �������ٸ� input��ü�� �� ����� �����Ͽ� �� Ű�� ��������մϴ�.
		m_Input->KeyUp((unsigned int)wparam);
		return 0;
	}
	// �ٸ� �޼������� �⺻ �޼��� ó���⿡ ����
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}

void SystemClass::Set_Rotation_Camera()
{
	_Xi += m_Input->pos_x();
	_Yi += m_Input->pos_z();
	m_Graphics->get_came()->SetRotation(_Yi*0.03f, _Xi*0.03f, 0.0f);

}

//�������ϴ��ڵ�
//�� �Լ��� ȣ��� ��ũ����  �ʺ�� ���� ��ȯ
//FULL_SCREEN ������ ������ ȭ�� Ȯ��(false�� 800*600 ������) -> graphicsclass.h�� ����� ��������
void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// �ܺ������͸� �� ��ü�� ����
	ApplicationHandle = this;

	// ���ø����̼��� �ν��Ͻ�(���?)�� �����´�
	m_hinstance = GetModuleHandle(NULL);

	// �� ���ø����̼��� �̸� ����
	m_applicationName = L"Engine";

	// ������ Ŭ������ �⺻�������� �����.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// ������ Ŭ���� ���
	RegisterClassEx(&wc);

	// ������� �ػ󵵸� Ȯ��
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Ǯ��ũ�� ������ ���� ���� ȭ�� ����
	if (FULL_SCREEN)
	{
		// ���� Ǯ��ũ�� ����� ȭ�� ũ�⸦ ����ũ�� ũ�⿡ ���߰� ������ 32bit�� �մϴ�.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Ǯ��ũ���� �´� ���÷��� ����
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// ȭ���� �������� ����
		posX = posY = 0;
	}

	else
	{
		// ������ ����� 800 x 600 �ػ󵵷� ����
		screenWidth = 800;
		screenHeight = 600;
		// â�� ����� ����� ��ġ��Ŵ
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// ������ ������ â�� ����� �ڵ��� �����´�.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY, screenWidth,
		screenHeight, NULL, NULL, m_hinstance, NULL);

	// �����츦 ȭ�鿡 ǥ���ϰ� ��Ŀ���� �ش�?
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// ���콺 Ŀ�� ǥ�� ����
	ShowCursor(false);
	return;
}


// ȭ�鼳���� �ǵ����� �ڵ��� ��ȯ
void SystemClass::ShutdownWindows()
{

	// ���콺 Ŀ�� ǥ�� ����
	ShowCursor(true);

	// ��üȭ�� ����� ���÷��� ������ ����
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// â�� ����
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// ���ø����̼� �ν��Ͻ� ����(�ڵ�)
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// �ܺ���������������
	ApplicationHandle = NULL;
	return;
}

void SystemClass::move_Camera()
{
	if (m_Input->User_input() == 11)
	{
		m_Graphics->Camera_move(11);
	}

	else if (m_Input->User_input() == 12)
	{
		m_Graphics->Camera_move(12);
	}
	else
	{
		m_Graphics->z_move = 0.0f;
	}

	if (m_Input->User_input() == 13)
	{
		m_Graphics->Camera_move(13);
	}

	else if (m_Input->User_input() == 14)
	{
		m_Graphics->Camera_move(14);
	}

	else if (m_Input->User_input() == 15)
	{
		m_Graphics->Camera_move(15);
	}

	else if (m_Input->User_input() == 16)
	{
		m_Graphics->Camera_move(16);
	}

	else if (m_Input->User_input() == 20)
	{
		m_Graphics->Camera_move(20);
	}

	else if (m_Input->User_input() == 21)
	{
		m_Graphics->Camera_move(21);
	}

	else if (m_Input->User_input() == 0)
	{
		m_Graphics->Camera_move(0);
	}

	else if (m_Input->User_input() == 1)
	{
		m_Graphics->Camera_move(1);
	}

	else
	{
		m_Graphics->x_move = 0.0f;
	}
}

//������ý��ۿ��� �޼����� ������ ��
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// �����찡 ���ŵǾ����� Ȯ��
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	// �����찡 �������� Ȯ��
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	// �ٸ� ��� �޼����� systemŬ������ �޼��� ó���⿡ ����
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}
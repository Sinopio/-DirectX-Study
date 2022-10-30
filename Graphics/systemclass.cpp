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

//모든 초기화작업 수행
bool SystemClass::Initialize()
{
	//스크린 넓이, 높이
	int screenWidth, screenHeight;
	bool result;

	// 함수에 너비와 높이를 전하기전에 초기화
	screenWidth = 0;
	screenHeight = 0;

	// windows api를 이용하여 초기화
	InitializeWindows(screenWidth, screenHeight);

	//20200621
	get_width = screenWidth;
	get_height = screenHeight;

	// 키보드 입력을 받는 Input객체 생성
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	// Input 객체 초기화
	result = m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}


	// 어플리케이션의 모든 그래픽요소를 그리는 Graphics객체 생성
	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	// Graphics객체 초기화
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

// 객체를 정리하는 함수, 위에 Graphics와 Input 객체 반환
void SystemClass::Shutdown()
{
	if (m_Sound) // 20200613 추가
	{
		m_Sound->Shutdown();
		delete m_Sound;
		m_Sound = 0;
	}

	if (m_CarSound) // 20200613 추가
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

	// Graphics 객체 반환
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Input 객체 반환
	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	// 창을 종료시킨다.
	ShutdownWindows();

	return;
}

//함수가 종료될 때 까지 루프를 돌면서 어플리케이션의 작업을 처리
//작업은 Frame함수에서 수행
void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	// msg(메세지) 구조체를 초기화
	ZeroMemory(&msg, sizeof(MSG));

	// 종료입력을 받을 때까지 루플르 돈다.
	done = false;
	while (!done)
	{
		// 윈도우 메세지를 처리
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// 종료입력을 받을경우 빠져나간다(종료?)
		if (msg.message == WM_QUIT)
		{
			done = true;
		}

		else
		{
			// 그 외의 경우에는 Frame함수를 처리
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

//어플리케이션의 작업이 처리되는 곳
//Input객체를 통해 플레이어가 Esc 를 눌러 종료시키는지 확인
//종료입력이 없다면 Graphics 객체를 통해서 화면을 그리는 작업을 수행
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

	// 유저가 Esc버튼을 누르는지 확인
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	// Graphics객체의 작업을 처리
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

//윈도우의 시스템 메세지를 전달
LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// 키보드가 눌렸는지 확인
	case WM_KEYDOWN:
	{
		// 키보드가 눌렸다면 Input객체에 전달
		m_Input->KeyDown((unsigned int)wparam);
		return 0;
	}
	// 키보드가 눌렸다가 떼어졌는지 확인(KeyUP?)
	case WM_KEYUP:
	{
		// 키가 떼어졌다면 input객체에 이 사실을 전달하여 이 키를 해제토록합니다.
		m_Input->KeyUp((unsigned int)wparam);
		return 0;
	}
	// 다른 메세지들은 기본 메세지 처리기에 전달
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

//렌더링하는코드
//이 함수로 호출된 스크린의  너비와 넓이 반환
//FULL_SCREEN 변수로 가득찬 화면 확인(false면 800*600 사이즈) -> graphicsclass.h에 선언된 전역변수
void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// 외부포인터를 이 객체로 지정
	ApplicationHandle = this;

	// 어플리케이션의 인스턴스(사례?)를 가져온다
	m_hinstance = GetModuleHandle(NULL);

	// 이 어플리케이션의 이름 설정
	m_applicationName = L"Engine";

	// 윈도우 클래스를 기본설정으로 맞춘다.
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

	// 윈도우 클래스 등록
	RegisterClassEx(&wc);

	// 모니터의 해상도를 확인
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// 풀스크린 변수의 값에 따라 화면 설정
	if (FULL_SCREEN)
	{
		// 만약 풀스크린 모드라면 화면 크기를 데스크톱 크기에 맞추고 색상을 32bit로 합니다.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// 풀스크린에 맞는 디스플레이 설정
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// 화면을 왼쪽위로 설정
		posX = posY = 0;
	}

	else
	{
		// 윈도우 모드라면 800 x 600 해상도로 설정
		screenWidth = 800;
		screenHeight = 600;
		// 창을 모니터 가운데로 위치시킴
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// 설정한 값으로 창을 만들고 핸들을 가져온다.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY, screenWidth,
		screenHeight, NULL, NULL, m_hinstance, NULL);

	// 윈도우를 화면에 표시하고 포커스를 준다?
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// 마우스 커서 표시 여부
	ShowCursor(false);
	return;
}


// 화면설정을 되돌리고 핸들을 반환
void SystemClass::ShutdownWindows()
{

	// 마우스 커서 표시 여부
	ShowCursor(true);

	// 전체화면 종료시 디스플레이 설정을 변경
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// 창을 제거
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// 어플리케이션 인스턴스 제거(핸들)
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// 외부포인터참조제거
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

//윈도우시스템에서 메세지를 보내는 곳
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// 윈도우가 제거되었는지 확인
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	// 윈도우가 닫히는지 확인
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	// 다른 모든 메세지를 system클래스에 메세지 처리기에 전달
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}
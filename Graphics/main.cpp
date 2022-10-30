#include "systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;
	GraphicsClass color;
	bool result;

	// system ��ü�� ����
	System = new SystemClass;
	if (!System)
	{
		return 0;
	}

	// system ��ü�� �ʱ�ȭ
	// Run�Լ� ȣ��
	result = System->Initialize();
	if (result)
	{
		System->Run();
	}

	// system ��ü�� �����ϰ� �޸𸮸� ��ȯ
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}
#pragma once
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <dinput.h>

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void KeyDown(unsigned int);
	void KeyUp(unsigned int);
	bool IsKeyDown(unsigned int);

	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	void GetMouseLocation(int&, int&);

	int User_input();

	float pos_x();
	float pos_z();

	bool IsFkeyPressed(); //20200618

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	bool m_keys[256];

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
};
#endif

//DIrectInput Ʃ�丮�� ������ �����쿡�� �����ϴ� �Է� ���?
//InputŬ������ ������ Ű���� �Է��� ó��
//SystemClass::MessageHandler �Լ��κ��� �Է��� ���޹޴´�
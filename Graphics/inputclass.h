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

//DIrectInput 튜토리얼 전까지 윈도우에서 제공하는 입력 사용?
//Input클래스는 유저의 키보드 입력을 처리
//SystemClass::MessageHandler 함수로부터 입력을 전달받는다
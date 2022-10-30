#pragma once
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_
//빌드 속도를 높이기 위해 디파인으로 선언
#define WIN32_LEAN_AND_MEAN
//윈도우 함수들을 호출하기 위해 헤더파일 선언
#include <windows.h>
#include "inputclass.h"
#include "graphicsclass.h"
#include "cameraclass.h"
#include "fpsclass.h"
#include "cpuclass.h"
#include "timerclass.h"
#include "Soundclass.h"


class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();
	void move_Camera();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

	void Set_Rotation_Camera();

	int get_width; //20200621
	int get_height; //20200621

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputClass* m_Input;
	GraphicsClass* m_Graphics;
	FpsClass* m_Fps;
	CpuClass* m_Cpu;
	TimerClass* m_Timer;
	SoundClass* m_Sound;
	SoundClass* m_CarSound; //20200618

	float _Xi = 0.0f;
	float _Yi = 0.0f;

private:
	float rotation;
	int isAmbient;
	int isDiffuse;
	int isDirection;

	bool IsCar; //20200618
};

// Function prototypes
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Globals
static SystemClass* ApplicationHandle = 0;

#endif
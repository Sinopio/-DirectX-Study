#pragma once
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "textureshaderclass.h"
#include "bitmapclass.h"
#include "textclass.h"
#include "skydomeclass.h"
#include "skydomeshaderclass.h"
#include "fireshaderclass.h"

// Globals
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();

	bool Frame(int, int, float, int, int); //20200621
	bool Render(float);

	bool on_fire;
	float polygon;

	void Camera_move(int view);
	float x_move, z_move;
	float camera_movex = 0.0f;
	float camera_movez = -20.0f;

	float move_point = 0;

	float cam_y_pos;
	void mouse_move_position();
	CameraClass* get_came();

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	TextureShaderClass* m_TextureShader;
	BitmapClass* m_Bitmap;
	TextClass* m_Text;
	
	//½ºÄ«ÀÌµ¼
	SkyDomeClass* m_SkyDome;
	SkyDomeShaderClass* m_SkyDomeShader;

	FireShaderClass* m_FireShader;


};
#endif
#pragma once
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_
#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
#include "textureclass.h"
using namespace std;

class ModelClass
{
private:
	//정점 버퍼에 사용할 정점 구조체 선언
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct StructType
	{
		float x, y, z;
	};

	struct FaceType
	{
		int vIndex1, vIndex2, vIndex3;
		int tIndex1, tIndex2, tIndex3;
		int nIndex1, nIndex2, nIndex3;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	//정점 버퍼와 인덱스 버퍼의 초기화 와 종료과정을 제어
	bool Initialize(ID3D11Device*, char*, WCHAR*);
	bool Initialize_f(ID3D11Device*, char*, WCHAR*, WCHAR*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();


	ID3D11ShaderResourceView* GetTexture();
	ID3D11ShaderResourceView* GetTexture1();
	ID3D11ShaderResourceView* GetTexture2();
	ID3D11ShaderResourceView* GetTexture3();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	bool LoadTextures(ID3D11Device*, WCHAR*, WCHAR*, WCHAR*);
	void ReleaseTexture();
	void ReleaseTextures();

	bool LoadModel(char*);
	void ReleaseModel();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;
	TextureClass *m_Texture1, *m_Texture2, *m_Texture3;
	ModelType * m_model;

	// 과제 >> obj파일의 정보를 읽을 함수들을 선언
	bool ReadFileCounts(char* filename, int& vertexCount, int& textureCount, int& normalCount, int& faceCount);
	bool LoadDataStructures(char* filename, int vertexCount, int textureCount, int normalCount, int faceCount);

};
#endif
#include "modelclass.h"

// 정점버퍼와 인덱스 버퍼의 포인터를 null로 설정
ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_model = 0;
	m_Texture1 = 0;
	m_Texture2 = 0;
	m_Texture3 = 0;
}

ModelClass::ModelClass(const ModelClass& other)
{
}

ModelClass::~ModelClass()
{
}

// 버퍼들을 초기화 하는 함수를 호출
bool ModelClass::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename)
{
	bool result;

	// Load in the model data,
	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	// 정점 버퍼와 인덱스 버퍼를 초기화 한다.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ModelClass::Initialize_f(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename1, WCHAR* textureFilename2,
	WCHAR* textureFilename3)
{
	bool result;


	// Load in the model data,
	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// Load the textures for this model.
	result = LoadTextures(device, textureFilename1, textureFilename2, textureFilename3);
	if (!result)
	{
		return false;
	}

	return true;
}

// 정점버퍼와 인덱스 버퍼를 정리하는 함수를 호출한다.
void ModelClass::Shutdown()
{
	// Release the model textures.
	ReleaseTextures();

	// Release the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}


// GraphicsClass::Render 함수에서 호출된다.
// RenderBuffers 함수를 호풀하여 정점 버퍼와 인덱스 버퍼를
// 그래픽 파이프라인에 넣어 쉐이더가 그릴수 있게함
void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// 정점 버퍼와 인덱스 버퍼를 그래픽스 파이프라인에 넣어 화면에 그릴 준비
	RenderBuffers(deviceContext);
	return;
}

//모델의 인덱스의 개술르 반환한다. 컬러셰이더에서 모델을 그리기위해 필요하다
int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

// 정점 버퍼와 인덱스 버퍼를 생성하는 작업을 제어
// 데이터 파일로부터 모델의 정보를 읽어와서 버퍼들을 만드는 일을 한다.
bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// 정점 배열을 생성한다.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// 인덱스 배열을 생성한다.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);
		indices[i] = i;
	}

	// 정점 버퍼의 description을 작성
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// 정점 데이터를 가리키는 보조 리소스 구조체를 작성
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// 정점 버퍼를 생성한다.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// 인덱스 버퍼의 description을 작성
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 인덱스 데이터를 가리키는 보조 리소스 구조체를 작성
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// 인덱스 버퍼를 생성한다.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// 생성되고 값이 할당된 정점 버퍼와 인덱스 버퍼를 해제
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;
	return true;
}

bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;
	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}
	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}
	return true;
}

void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
	return;
}

// InitializeBuffers 에서 만든 정점, 인덱스 버퍼 해제
void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}
	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
	return;
}

// Render함수에서 호출
// 정점, 인덱스 버퍼를 GPU의 어셈블러의 버퍼로 활성화
// 위 과정으로 셰이덜르 이용하여 버퍼의 내용을 그림
// IASetPrimitiveTopology 함수를 이용하려 GPU에서 삼각형을 그리게함
void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// 정점 버퍼의 단위와 오프셋을 설정
	stride = sizeof(VertexType);
	offset = 0;

	// input assembler에 정점 버퍼를 활성화한다.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// input assembler에 인덱스 버퍼를 활성화한다.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 정점 버퍼로 그릴 기본형을 설정(삼각형)
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool ModelClass::LoadModel(char* filename)
{

	bool result;
	ifstream fin;
	char input;
	int i;
	int vertexCount, textureCount, normalCount, faceCount;

	result = ReadFileCounts(filename, vertexCount, textureCount, normalCount, faceCount);
	if (!result)
	{
		return -1;
	}

	m_vertexCount = faceCount * 3;
	// 인덱스값 = 버텍스값
	m_indexCount = m_vertexCount;

	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	result = LoadDataStructures(filename, vertexCount, textureCount, normalCount, faceCount);
	if (!result)
	{
		return -1;
	}

	// Close the model file.
	fin.close();

	return true;
}

// 과제 >> 	vertexCount, textureCount, normalCount, faceCount를 확인할 함수
bool ModelClass::ReadFileCounts(char* filename, int& vertexCount, int& textureCount, int& normalCount, int& faceCount)
{
	ifstream fin;
	char input;

	// Initialize the counts.
	vertexCount = 0;
	textureCount = 0;
	normalCount = 0;
	faceCount = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}

	// Read from the file and continue to read until the end of the file is reached.
	fin.get(input);
	while (!fin.eof())
	{
		// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') { vertexCount++; }
			if (input == 't') { textureCount++; }
			if (input == 'n') { normalCount++; }
		}
		// If the line starts with 'f' then increment the face count.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') { faceCount++; }
		}
		// Otherwise read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
		}
		// Start reading the beginning of the next line.
		fin.get(input);
	}
	// Close the file.
	fin.close();

	return true;
}

// 과제 >> 	모델들을 그려주는 함수
bool ModelClass::LoadDataStructures(char* filename, int vertexCount, int textureCount, int normalCount, int faceCount)
{
	StructType *vertices, *texcoords, *normals;
	FaceType *faces;
	ifstream fin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;


	vertices = new StructType[vertexCount];
	if (!vertices)
	{
		return false;
	}

	texcoords = new StructType[textureCount];
	if (!texcoords)
	{
		return false;
	}

	normals = new StructType[normalCount];
	if (!normals)
	{
		return false;
	}

	faces = new FaceType[faceCount];
	if (!faces)
	{
		return false;
	}

	// Initialize the indexes.
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}

	// Read in the vertices, texture coordinates, and normals into the data structures.
	// Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.
	fin.get(input);

	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);
			// Read in the vertices.
			if (input == ' ')
			{
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z;

				// Invert the Z vertex to change to left hand system.
				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
				vertexIndex++;
			}

			// Read in the texture uv coordinates.
			if (input == 't')
			{
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;

				// Invert the V texture coordinates to left hand system.
				texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;
				texcoordIndex++;
			}

			// Read in the normals.
			if (input == 'n')
			{
				fin >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z;

				// Invert the Z normal to change to left hand system.
				normals[normalIndex].z = normals[normalIndex].z * -1.0f;
				normalIndex++;
			}
		}

		// Read in the faces.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				if (normalCount != 0)
				{
					// Read the face data in backwards to convert it to a left hand system from right hand system.
					fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3
						>> faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2
						>> faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1;
					faceIndex++;
				}

				else
				{
					if (normalCount == 0)
					{
						// Read the face data in backwards to convert it to a left hand system from right hand system.
						fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3
							>> faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2
							>> faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1;
						faceIndex++;
					}
				}

			}
		}

		// Read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	int line_a = 0;
	int line_b = 0;
	int line_c = 0;

	if (normalCount != 0)
	{
		for (int i = 0; i < faceIndex; i++)
		{
			line_a = i * 3;
			line_b = line_a + 1;
			line_c = line_a + 2;

			vIndex = faces[i].vIndex1 - 1;
			tIndex = faces[i].tIndex1 - 1;
			nIndex = faces[i].nIndex1 - 1;
			m_model[line_a].x = vertices[vIndex].x;
			m_model[line_a].y = vertices[vIndex].y;
			m_model[line_a].z = vertices[vIndex].z;
			m_model[line_a].tu = texcoords[tIndex].x;
			m_model[line_a].tv = texcoords[tIndex].y;
			m_model[line_a].nx = normals[nIndex].x;
			m_model[line_a].ny = normals[nIndex].y;
			m_model[line_a].nz = normals[nIndex].z;

			vIndex = faces[i].vIndex2 - 1;
			tIndex = faces[i].tIndex2 - 1;
			nIndex = faces[i].nIndex2 - 1;
			m_model[line_b].x = vertices[vIndex].x;
			m_model[line_b].y = vertices[vIndex].y;
			m_model[line_b].z = vertices[vIndex].z;
			m_model[line_b].tu = texcoords[tIndex].x;
			m_model[line_b].tv = texcoords[tIndex].y;
			m_model[line_b].nx = normals[nIndex].x;
			m_model[line_b].ny = normals[nIndex].y;
			m_model[line_b].nz = normals[nIndex].z;

			vIndex = faces[i].vIndex3 - 1;
			tIndex = faces[i].tIndex3 - 1;
			nIndex = faces[i].nIndex3 - 1;
			m_model[line_c].x = vertices[vIndex].x;
			m_model[line_c].y = vertices[vIndex].y;
			m_model[line_c].z = vertices[vIndex].z;
			m_model[line_c].tu = texcoords[tIndex].x;
			m_model[line_c].tv = texcoords[tIndex].y;
			m_model[line_c].nx = normals[nIndex].x;
			m_model[line_c].ny = normals[nIndex].y;
			m_model[line_c].nz = normals[nIndex].z;
		}
	}

	else if (normalCount == 0)
	{
		for (int i = 0; i < faceIndex; i++)
		{
			line_a = i * 3;
			line_b = line_a + 1;
			line_c = line_a + 2;

			vIndex = faces[i].vIndex1 - 1;
			tIndex = faces[i].tIndex1 - 1;
			nIndex = faces[i].nIndex1 - 1;
			m_model[line_a].x = vertices[vIndex].x / 300;
			m_model[line_a].y = vertices[vIndex].y / 300;
			m_model[line_a].z = vertices[vIndex].z / 300;
			m_model[line_a].tu = texcoords[tIndex].x;
			m_model[line_a].tv = texcoords[tIndex].y;

			vIndex = faces[i].vIndex2 - 1;
			tIndex = faces[i].tIndex2 - 1;
			nIndex = faces[i].nIndex2 - 1;
			m_model[line_b].x = vertices[vIndex].x / 300;
			m_model[line_b].y = vertices[vIndex].y / 300;
			m_model[line_b].z = vertices[vIndex].z / 300;
			m_model[line_b].tu = texcoords[tIndex].x;
			m_model[line_b].tv = texcoords[tIndex].y;

			vIndex = faces[i].vIndex3 - 1;
			tIndex = faces[i].tIndex3 - 1;
			nIndex = faces[i].nIndex3 - 1;
			m_model[line_c].x = vertices[vIndex].x / 300;
			m_model[line_c].y = vertices[vIndex].y / 300;
			m_model[line_c].z = vertices[vIndex].z / 300;
			m_model[line_c].tu = texcoords[tIndex].x;
			m_model[line_c].tv = texcoords[tIndex].y;
		}
	}

	// Close the file.
	fin.close();

	// 동적할당 해제
// Release the four data structures.
	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}
	if (texcoords)
	{
		delete[] texcoords;
		texcoords = 0;
	}
	if (normals)
	{
		delete[] normals;
		normals = 0;
	}
	if (faces)
	{
		delete[] faces;
		faces = 0;
	}
	return true;
}



void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}

bool ModelClass::LoadTextures(ID3D11Device* device, WCHAR* textureFilename1, WCHAR* textureFilename2, WCHAR* textureFilename3)
{
	bool result;


	// Create the texture object.
	m_Texture1 = new TextureClass;
	if (!m_Texture1)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture1->Initialize(device, textureFilename1);
	if (!result)
	{
		return false;
	}

	// Create the texture object.
	m_Texture2 = new TextureClass;
	if (!m_Texture2)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture2->Initialize(device, textureFilename2);
	if (!result)
	{
		return false;
	}

	// Create the texture object.
	m_Texture3 = new TextureClass;
	if (!m_Texture3)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture3->Initialize(device, textureFilename3);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::ReleaseTextures()
{
	// Release the texture objects.
	if (m_Texture1)
	{
		m_Texture1->Shutdown();
		delete m_Texture1;
		m_Texture1 = 0;
	}

	if (m_Texture2)
	{
		m_Texture2->Shutdown();
		delete m_Texture2;
		m_Texture2 = 0;
	}

	if (m_Texture3)
	{
		m_Texture3->Shutdown();
		delete m_Texture3;
		m_Texture3 = 0;
	}

	return;
}


ID3D11ShaderResourceView* ModelClass::GetTexture1()
{
	return m_Texture1->GetTexture();
}


ID3D11ShaderResourceView* ModelClass::GetTexture2()
{
	return m_Texture2->GetTexture();
}


ID3D11ShaderResourceView* ModelClass::GetTexture3()
{
	return m_Texture3->GetTexture();
}
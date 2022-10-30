#include "cameraclass.h"

//카메라의 위치와 회전상태를 원점으로 설정
CameraClass::CameraClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}
CameraClass::CameraClass(const CameraClass& other)
{
}
CameraClass::~CameraClass()
{
}

//위치와 회전값 설정 함수
void CameraClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}
void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}

//위치와 회전값 반환함수
D3DXVECTOR3 CameraClass::GetPosition()
{
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}
D3DXVECTOR3 CameraClass::GetRotation()
{
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}


// 카메라의 위치와 회전정보를 이용해 뷰 행렬 갱신
//
void CameraClass::Render()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;

	// up vector 설정
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// 월드에 카메라의 위치를 설정한다.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// 카메라가 바라보든 디폴트값을 설정한다
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// 라디안 값으로 설정 the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations 
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	// 위 값을로 회전 매트릭스를 만든다.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// 뷰가 올바르게 회전하도록 lookAt 과 up vector를 변환?
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// 회전된 카메라 위치를 뷰 위치로 변환
	lookAt = position + lookAt;

	// 위에서 업데이트된 벡터를도 뷰매트릭스 구성
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);

	return;
}

// 뷰 행렬을 얻어온다?
void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}

float CameraClass::return_rotation()
{
	return m_rotationY;
}

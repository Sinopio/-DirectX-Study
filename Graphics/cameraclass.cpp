#include "cameraclass.h"

//ī�޶��� ��ġ�� ȸ�����¸� �������� ����
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

//��ġ�� ȸ���� ���� �Լ�
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

//��ġ�� ȸ���� ��ȯ�Լ�
D3DXVECTOR3 CameraClass::GetPosition()
{
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}
D3DXVECTOR3 CameraClass::GetRotation()
{
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}


// ī�޶��� ��ġ�� ȸ�������� �̿��� �� ��� ����
//
void CameraClass::Render()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;

	// up vector ����
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// ���忡 ī�޶��� ��ġ�� �����Ѵ�.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// ī�޶� �ٶ󺸵� ����Ʈ���� �����Ѵ�
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// ���� ������ ���� the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations 
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	// �� ������ ȸ�� ��Ʈ������ �����.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// �䰡 �ùٸ��� ȸ���ϵ��� lookAt �� up vector�� ��ȯ?
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// ȸ���� ī�޶� ��ġ�� �� ��ġ�� ��ȯ
	lookAt = position + lookAt;

	// ������ ������Ʈ�� ���͸��� ���Ʈ���� ����
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);

	return;
}

// �� ����� ���´�?
void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}

float CameraClass::return_rotation()
{
	return m_rotationY;
}

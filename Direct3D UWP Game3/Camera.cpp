#include "pch.h"
#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

const XMVECTORF32 Camera::START_POSITION = { 0.f, -1.5f, 0.f, 0.f };
const XMVECTORF32 Camera::ROOM_BOUNDS = { 18.f, 16.f, 42.f, 0.f };
const float Camera::ROTATION_GAIN = 0.004f;
const float Camera::MOVEMENT_GAIN = 0.07f;

Camera::Camera():
	m_yaw(0.f),
	m_pitch(0.f)
{
	m_camPos = START_POSITION.v;
}

XMMATRIX Camera::GetViewMatrix()
{
	float y = sinf(m_pitch);
	float r = cosf(m_pitch);
	float z = r * cosf(m_yaw);
	float x = r * sinf(m_yaw);

	XMVECTOR lookAt = m_camPos + Vector3(x, y, z);
	return XMMatrixLookAtRH(m_camPos, lookAt, DirectX::SimpleMath::Vector3::Up);
}

void Camera::MovePosition(DirectX::SimpleMath::Vector3 move)
{
	Quaternion q = Quaternion::CreateFromYawPitchRoll(m_yaw, -m_pitch, 0.f);

	move = Vector3::Transform(move, q);

	move *= MOVEMENT_GAIN;

	m_camPos += move;

	Vector3 halfBound = (Vector3(ROOM_BOUNDS.v) / Vector3(2.f))
		- Vector3(0.1f, 0.1f, 0.1f);

	m_camPos = Vector3::Min(m_camPos, halfBound);
	m_camPos = Vector3::Max(m_camPos, -halfBound);
}

void Camera::ChangeLookDirection(DirectX::SimpleMath::Vector3 direction)
{
	direction *= ROTATION_GAIN;

	m_pitch -= direction.y;
	m_yaw -= direction.x;

	// limit pitch to straight up or straight down
	// with a little fudge-factor to avoid gimbal lock
	float limit = XM_PI / 2.0f - 0.01f;
	m_pitch = std::max(-limit, m_pitch);
	m_pitch = std::min(+limit, m_pitch);

	// keep longitude in sane range by wrapping
	if (m_yaw > XM_PI)
	{
		m_yaw -= XM_PI * 2.0f;
	}
	else if (m_yaw < -XM_PI)
	{
		m_yaw += XM_PI * 2.0f;
	}
}

void Camera::Reset()
{
	m_camPos = START_POSITION.v;
	m_pitch = m_yaw = 0;
}

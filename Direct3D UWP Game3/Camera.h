#pragma once

#include <SimpleMath.h>


ref class Camera sealed
{
internal:
	Camera();

	DirectX::XMMATRIX GetViewMatrix();

	void MovePosition(DirectX::SimpleMath::Vector3 move);
	void ChangeLookDirection(DirectX::SimpleMath::Vector3 direction);

	void Reset();


private:

	static const DirectX::XMVECTORF32		START_POSITION;
	static const DirectX::XMVECTORF32		ROOM_BOUNDS;
	static const float						ROTATION_GAIN;
	static const float						MOVEMENT_GAIN;

	float									m_yaw;
	float									m_pitch;
	DirectX::SimpleMath::Vector3			m_camPos;
};


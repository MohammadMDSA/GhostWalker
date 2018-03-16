#pragma once

#include <SimpleMath.h>

#include "DeviceResource.h"

#include "StepTimer.h"
ref class GameObject abstract
{
internal:
	GameObject();

	// Inherited via IDrawable
	virtual void Draw(ID3D11DeviceContext3* deviceContext, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;
	virtual void Update(DX::StepTimer const& timer) = 0;
	virtual void CreateResource(DeviceResource^ device, DirectX::XMFLOAT3& size, bool rhcoords = true, bool invertn = false) = 0;
	virtual void OnDeviceLost() = 0;

protected private:
	DirectX::SimpleMath::Matrix							m_world;

	DirectX::SimpleMath::Vector3						m_position;
	DirectX::SimpleMath::Vector3						m_velocity;

};


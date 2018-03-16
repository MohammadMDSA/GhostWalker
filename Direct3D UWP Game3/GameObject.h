#pragma once

#include <SimpleMath.h>

#include "IDrawable.h"
#include "IUpdatable.h"

ref class GameObject abstract
{
internal:
	GameObject();

	// Inherited via IDrawable
	virtual void Draw(ID3D11DeviceContext3 deviceContext) = 0;
	virtual void Update(DX::StepTimer timer) = 0;

protected:
	DirectX::SimpleMath::Matrix							m_view;

	DirectX::SimpleMath::Vector3						m_position;
	DirectX::SimpleMath::Vector3						m_velocity;

};


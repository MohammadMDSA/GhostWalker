#pragma once

#include <SimpleMath.h>

#include "IDrawable.h"

ref class GameObject abstract : public IDrawable
{
public:
	GameObject();

	// Inherited via IDrawable
	virtual void Draw(ID3D11DeviceContext3 deviceContext) = 0;
	virtual

private:
	Matrix						m_view;

	Vector3						m_position;
	Vector3						m_velocity;

};


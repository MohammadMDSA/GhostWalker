#pragma once
#include "GameObject.h"
ref class Sphere sealed : public GameObject
{
internal:
	Sphere();

	// Inherited via GameObject
	virtual void Draw(ID3D11DeviceContext3 deviceContext) override;
	virtual void Update(DX::StepTimer timer) override;
};


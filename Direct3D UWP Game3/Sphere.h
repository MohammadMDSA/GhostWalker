#pragma once

#include "GameObject.h"

#include <GeometricPrimitive.h>


ref class Sphere sealed : public GameObject
{
internal:
	Sphere();

	// Inherited via GameObject
	virtual void Draw(ID3D11DeviceContext3* deviceContext, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
	virtual void Update(DX::StepTimer const& timer) override;
	virtual void CreateResource(DeviceResource^ device, DirectX::XMFLOAT3& size, bool rhcoords = true, bool invertn = false) override;
	virtual void OnDeviceLost() override;

private:
	std::unique_ptr<DirectX::GeometricPrimitive>		m_shape;
};


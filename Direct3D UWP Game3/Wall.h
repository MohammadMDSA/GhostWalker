#pragma once

#include "GameObject.h"

#include <GeometricPrimitive.h>

ref class Wall : public GameObject
{
internal:
	Wall();
	// Inherited via GameObject
	virtual void Draw(ID3D11DeviceContext3 * deviceContext, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
	virtual void Update(DX::StepTimer const & timer) override;
	virtual void CreateResource(DeviceResource^ device, DirectX::XMFLOAT3 & size, bool rhcoords = true, bool invertn = false) override;

private:
	std::unique_ptr<DirectX::GeometricPrimitive>			m_shape;
};


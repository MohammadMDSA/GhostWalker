#pragma once


#include "GameObject.h"
#include <Model.h>
#include <CommonStates.h>
#include <Effects.h>


ref class Wheel : public GameObject
{
internal:
	Wheel();

	// Inherited via GameObject
	virtual void Draw(ID3D11DeviceContext3 * deviceContext, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
	virtual void Update(DX::StepTimer const & timer) override;
	virtual void CreateResource(DeviceResource^ device, DirectX::XMFLOAT3 & size, bool rhcoords = true, bool invertn = false) override;
	virtual void OnDeviceLost() override;

private:

	std::unique_ptr<DirectX::Model>			m_model;
	std::unique_ptr<DirectX::CommonStates>	m_states;
};


#include "pch.h"
#include "Wheel.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;


Wheel::Wheel()
{

	m_world *= DirectX::SimpleMath::Matrix::CreateScale(0.005);
}

void Wheel::Draw(ID3D11DeviceContext3 * deviceContext, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	m_model->Draw(deviceContext, *m_states.get(), m_world, view, proj);
}

void Wheel::Update(DX::StepTimer const & timer)
{
	m_world *= Matrix::CreateRotationZ(0.01f);

}

void Wheel::CreateResource(DeviceResource^ device, DirectX::XMFLOAT3 & size, bool rhcoords, bool invertn)
{
	m_states = std::make_unique<CommonStates>(device->GetDevice());
	EffectFactory fx(device->GetDevice());
	m_model = Model::CreateFromSDKMESH(device->GetDevice(), L"Senza Titolo 2.sdkmesh", fx);
}


void Wheel::OnDeviceLost()
{
	m_model.reset();
}
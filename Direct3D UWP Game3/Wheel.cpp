#include "pch.h"
#include "Wheel.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

const float Wheel::CONST_SCALE =  0.005f;

Wheel::Wheel()
{
	m_position = Vector3(5.f, 5.f, 5.f);
	m_world *= DirectX::SimpleMath::Matrix::CreateScale(CONST_SCALE);
}

void Wheel::Draw(ID3D11DeviceContext3 * deviceContext, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	m_model->Draw(deviceContext, *m_states.get(), m_world, view, proj);
}

void Wheel::Update(DX::StepTimer const & timer)
{

	
	m_world *= Matrix::CreateTranslation(-m_position);
	m_world *= Matrix::CreateRotationZ(0.01f);
	m_world *= Matrix::CreateTranslation(m_position);

}

void Wheel::CreateResource(DeviceResource^ device, DirectX::XMFLOAT3 & size, bool rhcoords, bool invertn)
{
	m_states = std::make_unique<CommonStates>(device->GetDevice());
	EffectFactory fx(device->GetDevice());
	m_model = Model::CreateFromSDKMESH(device->GetDevice(), L"Senza Titolo 2.sdkmesh", fx);

	auto objs = m_model->meshes;
	for (auto obj = objs.begin(); obj != objs.end(); obj++)
	{
		m_world *= Matrix::CreateTranslation(m_position - (*obj)->boundingBox.Center * CONST_SCALE);
	}
}


void Wheel::OnDeviceLost()
{
	m_model.reset();
}
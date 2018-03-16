#include "pch.h"
#include "Sphere.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Sphere::Sphere()
{
}

void Sphere::Draw(ID3D11DeviceContext3* deviceContext, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	m_shape->Draw(m_world, view, proj);
}

void Sphere::Update(DX::StepTimer const& timer)
{
	m_position += m_velocity;

	m_world *= Matrix::CreateTranslation(m_velocity);
}

void Sphere::CreateResource(DeviceResource^ device, DirectX::XMFLOAT3& size, bool rhcoords, bool invertn)
{
	m_shape = DirectX::GeometricPrimitive::CreateSphere(device->GetDeviceContext(), 1.f, 16, rhcoords, invertn);
}

void Sphere::OnDeviceLost()
{
	m_shape.reset();
}

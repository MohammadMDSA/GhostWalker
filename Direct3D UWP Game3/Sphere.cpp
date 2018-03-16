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
}

void Sphere::CreateResource(ID3D11DeviceContext3 * deviceContext, DirectX::XMFLOAT3& size, bool rhcoords = true, bool invertn = false)
{
	m_shape = DirectX::GeometricPrimitive::CreateSphere(deviceContext, 1.f, 16, rhcoords, invertn);
}

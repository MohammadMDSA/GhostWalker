#include "pch.h"
#include "Wall.h"

using namespace DirectX;


static const XMVECTORF32 ROOM_BOUNDS = { 18.f, 16.f, 42.f, 0.f };


Wall::Wall()
{
}


void Wall::Draw(ID3D11DeviceContext3 * deviceContext, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	m_shape->Draw(m_world, view, proj);
}

void Wall::Update(DX::StepTimer const & timer)
{
}

void Wall::CreateResource(DeviceResource^ device, DirectX::XMFLOAT3 & size, bool rhcoords, bool invertn)
{
	m_shape = GeometricPrimitive::CreateBox(device->GetDeviceContext(), XMFLOAT3(ROOM_BOUNDS[0], ROOM_BOUNDS[1], ROOM_BOUNDS[2]), false, true);
}

void Wall::OnDeviceLost()
{
	m_shape.reset();
}
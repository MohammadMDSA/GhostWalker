#include "pch.h"
#include "Walls.h"

using namespace DirectX;


static const XMVECTORF32 ROOM_BOUNDS = { 18.f, 16.f, 42.f, 0.f };


Walls::Walls()
{
}


void Walls::Draw(ID3D11DeviceContext3 * deviceContext, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	m_shape->Draw(m_world, view, proj);
}

void Walls::Update(DX::StepTimer const & timer)
{
}

void Walls::CreateResource(ID3D11DeviceContext3 * deviceContext, DirectX::XMFLOAT3 & size, bool rhcoords, bool invertn)
{
	GeometricPrimitive::CreateBox(deviceContext, XMFLOAT3(ROOM_BOUNDS[0], ROOM_BOUNDS[1], ROOM_BOUNDS[2]), false, true);
}

#include "pch.h"
#include "GameObject.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;


GameObject::GameObject()
{
	m_world = Matrix::Identity;
}

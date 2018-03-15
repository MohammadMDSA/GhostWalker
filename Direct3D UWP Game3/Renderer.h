#pragma once

#include <SimpleMath.h>
#include <GeometricPrimitive.h>

#include "Game.h"


ref class Game;

ref class Renderer sealed
{
internal:
	Renderer(DeviceResource^ device);

	void SetGameResource(Game^ game) { this->m_game = game; }

	void CreateResource();

	void Render();
	

private:

	void Present();
	void Clear();


	DeviceResource^									m_device;

	DirectX::SimpleMath::Matrix						m_world;
	DirectX::SimpleMath::Matrix						m_proj;

	Game^											m_game;

};


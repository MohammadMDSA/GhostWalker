//
// Game.h
//

#pragma once

#include "DeviceResource.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Wall.h"
#include "Sphere.h"
#include "Wheel.h"

#include "StepTimer.h"

#include <Keyboard.h>
#include <Mouse.h>
#include <GamePad.h>

#include <Effects.h>
#include <Model.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <GeometricPrimitive.h>

#include "Camera.h"

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
ref class Game
{
internal:

	Game();

	// Initialization and management
	void Initialize(IUnknown* window, int width, int height, DXGI_MODE_ROTATION rotation);

	// Basic game loop
	void Tick();

	// Messages
	void OnActivated();
	void OnDeactivated();
	void OnSuspending();
	void OnResuming();
	void OnWindowSizeChanged(int width, int height, DXGI_MODE_ROTATION rotation);
	void ValidateDevice();

	// Getters
	Camera^ GetCamera()												{ return this->m_camera; }

	DX::StepTimer GetTimer()										{ return this->m_timer; }
	std::vector<DirectX::GeometricPrimitive*>* GetGameObjects()		{ return m_gameObjects; }
	std::vector<GameObject^>* GetGameObjectss()						{ return m_gameObjectss; }
	DirectX::GeometricPrimitive* GetWalls()							{ return m_walls.get(); }
	DirectX::Model* GetModel()										{ return m_model.get(); }
	DirectX::CommonStates* GetState()								{ return m_states.get(); }

	// Properties
	void GetDefaultSize(int& width, int& height) const;

	void OnDeviceLost();

private:

	void Update(DX::StepTimer const& timer);
	void Render();

	void Load();

	std::unique_ptr<DirectX::GeometricPrimitive>	m_walls;
	std::unique_ptr<DirectX::GeometricPrimitive>	m_ball;
	std::vector<DirectX::GeometricPrimitive*>*		m_gameObjects;
	std::vector<GameObject^>*						m_gameObjectss;

	std::unique_ptr<DirectX::CommonStates>			m_states;
	std::unique_ptr<DirectX::Model>					m_model;

	// Resources
	DeviceResource^									m_device;
	Renderer^										m_renderer;

	// Rendering loop timer.
	DX::StepTimer									m_timer;

	std::unique_ptr<DirectX::Keyboard>				m_keyboard;
	std::unique_ptr<DirectX::Mouse>					m_mouse;
	std::unique_ptr<DirectX::GamePad>				m_gamePad;

	Camera^											m_camera;
};

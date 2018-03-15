//
// Game.cpp
//

#include "pch.h"

#include "Game.h"

extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;


using Microsoft::WRL::ComPtr;

static const XMVECTORF32 ROOM_BOUNDS = { 18.f, 16.f, 42.f, 0.f };

Game::Game()
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(IUnknown* window, int width, int height, DXGI_MODE_ROTATION rotation)
{
	m_device = ref new DeviceResource();
	m_device->SetWindow(window);
	m_device->Initialize();

	m_renderer = ref new Renderer(m_device);
	m_renderer->SetGameResource(this);

	m_camera = ref new Camera();

	m_gameObjects = std::vector<DirectX::GeometricPrimitive*>();
	
	m_renderer->CreateResource();

	m_keyboard = std::make_unique<Keyboard>();
	m_keyboard->SetWindow(reinterpret_cast<ABI::Windows::UI::Core::ICoreWindow*>(window));

	m_mouse = std::make_unique<Mouse>();
	m_mouse->SetWindow(reinterpret_cast<ABI::Windows::UI::Core::ICoreWindow*>(window));

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/

	Load();
}

// Executes the basic game loop.
void Game::Tick()
{
	m_timer.Tick([&]()
	{
		Update(m_timer);
	});

	Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
	float elapsedTime = float(timer.GetElapsedSeconds());
	float time = float(timer.GetTotalSeconds());

	// TODO: Add your game logic here.
	elapsedTime;
	
	auto kb = m_keyboard->GetState();
	if (kb.Escape)
		ExitGame();

	if (kb.Home)
	{
		
	}

	Vector3 move = Vector3::Zero;

	if (kb.Up || kb.Space)
		move.y += 1.f;

	if (kb.Down || kb.X)
		move.y -= 1.f;

	if (kb.Left || kb.A)
		move.x += 1.f;

	if (kb.Right || kb.D)
		move.x -= 1.f;

	if (kb.PageUp || kb.W)
		move.z += 1.f;

	if (kb.PageDown || kb.S)
		move.z -= 1.f;
	
	m_camera->MovePosition(move);


	auto mouse = m_mouse->GetState();

	if (mouse.positionMode == Mouse::MODE_RELATIVE)
	{
		Vector3 delta = Vector3(float(mouse.x), float(mouse.y), 0.f);

		m_camera->ChangeLookDirection(delta);
	}

	m_mouse->SetMode(Mouse::MODE_RELATIVE);
	//m_mouse->SetMode(mouse.leftButton ? Mouse::MODE_RELATIVE : Mouse::MODE_ABSOLUTE);
}

// Draws the scene.
void Game::Render()
{
	m_renderer->Render();
}

void Game::Load()
{
	m_walls = GeometricPrimitive::CreateBox(m_device->GetDeviceContext(), XMFLOAT3(ROOM_BOUNDS[0], ROOM_BOUNDS[1], ROOM_BOUNDS[2]), false, true);

	m_gameObjects.push_back(m_walls.get());

}

// Helper method to clear the back buffers.


// Message handlers
void Game::OnActivated()
{
	// TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
	// TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
	m_device->GetDeviceContext()->ClearState();

	ComPtr<IDXGIDevice3> dxgiDevice;
	if (SUCCEEDED(m_device->GetDevice_Com().As(&dxgiDevice)))
	{
		dxgiDevice->Trim();
	}

	// TODO: Game is being power-suspended.
}

void Game::OnResuming()
{
	m_timer.ResetElapsedTime();

	// TODO: Game is being power-resumed.
}

void Game::OnWindowSizeChanged(int width, int height, DXGI_MODE_ROTATION rotation)
{
	m_device->SetOutputWidth(std::max(width, 1));
	m_device->SetOutputHeight(std::max(height, 1));
	m_device->SetOutputRotation(rotation);

	m_renderer->CreateResource();

	// TODO: Game window is being resized.
}

void Game::ValidateDevice()
{
	// The D3D Device is no longer valid if the default adapter changed since the device
	// was created or if the device has been removed.

	DXGI_ADAPTER_DESC previousDesc;
	{
		ComPtr<IDXGIDevice3> dxgiDevice;
		DX::ThrowIfFailed(m_device->GetDevice_Com().As(&dxgiDevice));

		ComPtr<IDXGIAdapter> deviceAdapter;
		DX::ThrowIfFailed(dxgiDevice->GetAdapter(deviceAdapter.GetAddressOf()));

		ComPtr<IDXGIFactory2> dxgiFactory;
		DX::ThrowIfFailed(deviceAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

		ComPtr<IDXGIAdapter1> previousDefaultAdapter;
		DX::ThrowIfFailed(dxgiFactory->EnumAdapters1(0, previousDefaultAdapter.GetAddressOf()));

		DX::ThrowIfFailed(previousDefaultAdapter->GetDesc(&previousDesc));
	}

	DXGI_ADAPTER_DESC currentDesc;
	{
		ComPtr<IDXGIFactory2> currentFactory;
		DX::ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(currentFactory.GetAddressOf())));

		ComPtr<IDXGIAdapter1> currentDefaultAdapter;
		DX::ThrowIfFailed(currentFactory->EnumAdapters1(0, currentDefaultAdapter.GetAddressOf()));

		DX::ThrowIfFailed(currentDefaultAdapter->GetDesc(&currentDesc));
	}

	// If the adapter LUIDs don't match, or if the device reports that it has been removed,
	// a new D3D device must be created.

	if (previousDesc.AdapterLuid.LowPart != currentDesc.AdapterLuid.LowPart
		|| previousDesc.AdapterLuid.HighPart != currentDesc.AdapterLuid.HighPart
		|| FAILED(m_device->GetDevice_Com()->GetDeviceRemovedReason()))
	{
		// Create a new device and swap chain.
		OnDeviceLost();
	}
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
	// TODO: Change to desired default window size (note minimum size is 320x200).
	width = 800;
	height = 600;
}

void Game::OnDeviceLost()
{
	// TODO: Add Direct3D resource cleanup here.

	m_device->OnDeviceLost();

	m_walls.reset();

	m_device->Initialize();

	m_renderer->CreateResource();
}
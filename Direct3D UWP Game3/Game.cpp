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

	m_gameObjects = new std::vector<DirectX::GeometricPrimitive*>();
	m_gameObjectss = new std::vector<GameObject^>();
	
	m_renderer->CreateResource();

	m_keyboard = std::make_unique<Keyboard>();
	m_keyboard->SetWindow(reinterpret_cast<ABI::Windows::UI::Core::ICoreWindow*>(window));

	m_mouse = std::make_unique<Mouse>();
	m_mouse->SetWindow(reinterpret_cast<ABI::Windows::UI::Core::ICoreWindow*>(window));

	m_gamePad = std::make_unique<GamePad>();









	m_gameObjectss->push_back(
		ref new Wall()
	);

	m_gameObjectss->push_back(
		ref new Wheel()
	);










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
	auto gamepad = m_gamePad->GetState(0);

	if (kb.Escape || gamepad.IsAPressed())
		ExitGame();

	if (kb.Home)
	{
		m_camera->Reset();
	}

	Vector3 move = Vector3::Zero;

	if (gamepad.IsConnected())
	{
		move.z += gamepad.thumbSticks.leftY;
		move.x -= gamepad.thumbSticks.leftX;
	}

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

	Vector3 delta = Vector3();

	if (mouse.positionMode == Mouse::MODE_RELATIVE)
	{
		delta.x = float(mouse.x);
		delta.y = float(mouse.y);
	}
	m_mouse->SetMode(Mouse::MODE_RELATIVE);

	if (gamepad.IsConnected())
	{
		delta.x += gamepad.thumbSticks.rightX * 5;
		delta.y -= gamepad.thumbSticks.rightY * 5;
	}

	m_camera->ChangeLookDirection(delta);

	// Updating game objects
	for (auto  object = m_gameObjectss->begin(); object != m_gameObjectss->end(); object++)
	{
		(*object)->Update(timer);
	}
}

// Draws the scene.
void Game::Render()
{
	m_renderer->Render();
}

void Game::Load()
{


	for (auto object = m_gameObjectss->begin(); object != m_gameObjectss->end(); object++)
	{
		(*object)->CreateResource(m_device, XMFLOAT3(0, 0, 0));
	}



	m_walls = GeometricPrimitive::CreateBox(m_device->GetDeviceContext(), XMFLOAT3(ROOM_BOUNDS[0], ROOM_BOUNDS[1], ROOM_BOUNDS[2]), false, true);

	m_ball = GeometricPrimitive::CreateSphere(m_device->GetDeviceContext());

	//m_gameObjects->push_back(m_ball.get());

	//m_gameObjects->push_back(m_walls.get());

	m_states = std::make_unique<CommonStates>(m_device->GetDevice());
	EffectFactory fx(m_device->GetDevice());
	auto a1 = m_device->GetDevice();
	m_model = Model::CreateFromSDKMESH(a1, L"Senza Titolo 2.sdkmesh", fx);

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

	// Resetting game objects
	for (auto object = m_gameObjectss->begin(); object != m_gameObjectss->end(); object++)
	{
		(*object)->OnDeviceLost();
	}

	m_walls.reset();

	m_states.reset();
	m_model.reset();

	m_device->Initialize();
	Load();

	m_renderer->CreateResource();
}
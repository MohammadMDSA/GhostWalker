#include "pch.h"
#include "Renderer.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

Renderer::Renderer(DeviceResource^ device) :
	m_device(device)
{
}

void Renderer::Render()
{
	// Don't try to render anything before the first Update.
	if (m_game->GetTimer().GetFrameCount() == 0)
	{
		return;
	}

	Clear();

	// TODO: Add your rendering code here.

	XMMATRIX view = m_game->GetCamera()->GetViewMatrix();

	auto objects = m_game->GetGameObjects();

	for (auto object = objects->begin(); object != objects->end(); object++)
	{
		(*object)->Draw(Matrix::Identity, view, m_proj);
	}

	m_game->GetModel()->Draw(m_device->m_d3dContext.Get(), *m_game->GetState(), m_world, view, m_proj);
	//m_game->GetWalls()->Draw(Matrix::Identity, view, m_proj);

	Present();
}

void Renderer::Clear()
{
	// Clear the views.
	m_device->m_d3dContext->ClearRenderTargetView(m_device->m_renderTargetView.Get(), Colors::CornflowerBlue);
	m_device->m_d3dContext->ClearDepthStencilView(m_device->m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_device->m_d3dContext->OMSetRenderTargets(1, m_device->m_renderTargetView.GetAddressOf(), m_device->m_depthStencilView.Get());

	// Set the viewport.
	CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_device->m_outputWidth), static_cast<float>(m_device->m_outputHeight));
	m_device->m_d3dContext->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void Renderer::Present()
{
	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.
	HRESULT hr = m_device->m_swapChain->Present(1, 0);

	// Discard the contents of the render target.
	// This is a valid operation only when the existing contents will be entirely
	// overwritten. If dirty or scroll rects are used, this call should be removed.
	m_device->m_d3dContext->DiscardView(m_device->m_renderTargetView.Get());

	// Discard the contents of the depth stencil.
	m_device->m_d3dContext->DiscardView(m_device->m_depthStencilView.Get());

	// If the device was reset we must completely reinitialize the renderer.
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		m_game->OnDeviceLost();
	}
	else
	{
		DX::ThrowIfFailed(hr);
	}
}

// Allocate all memory resources that change on a window SizeChanged event.
void Renderer::CreateResource()
{
	// Clear the previous window size specific context.
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	m_device->GetDeviceContext()->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
	m_device->m_renderTargetView.Reset();
	m_device->m_depthStencilView.Reset();
	m_device->GetDeviceContext()->Flush();

	UINT backBufferWidth = static_cast<UINT>(m_device->m_outputWidth);
	UINT backBufferHeight = static_cast<UINT>(m_device->m_outputHeight);
	DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
	DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	UINT backBufferCount = 2;

	// If the swap chain already exists, resize it, otherwise create one.
	if (m_device->m_swapChain)
	{
		HRESULT hr = m_device->m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			// If the device was removed for any reason, a new device and swap chain will need to be created.
			m_game->OnDeviceLost();

			// Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
			// and correctly set up the new device.
			return;
		}
		else
		{
			DX::ThrowIfFailed(hr);
		}
	}
	else
	{
		// First, retrieve the underlying DXGI Device from the D3D Device.
		ComPtr<IDXGIDevice1> dxgiDevice;
		DX::ThrowIfFailed(m_device->m_d3dDevice.As(&dxgiDevice));

		// Identify the physical adapter (GPU or card) this device is running on.
		ComPtr<IDXGIAdapter> dxgiAdapter;
		DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

		// And obtain the factory object that created it.
		ComPtr<IDXGIFactory2> dxgiFactory;
		DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

		// Create a descriptor for the swap chain.
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.Width = backBufferWidth;
		swapChainDesc.Height = backBufferHeight;
		swapChainDesc.Format = backBufferFormat;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = backBufferCount;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.Scaling = DXGI_SCALING_ASPECT_RATIO_STRETCH;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

		ComPtr<IDXGISwapChain1> swapChain;
		auto aa = m_device->m_d3dDevice.Get();
		auto bb = m_device->m_window;
		auto cc = swapChain.GetAddressOf();
		DX::ThrowIfFailed(dxgiFactory->CreateSwapChainForCoreWindow(
			m_device->m_d3dDevice.Get(),
			m_device->m_window,
			&swapChainDesc,
			nullptr,
			swapChain.GetAddressOf()
		));

		DX::ThrowIfFailed(swapChain.As(&(m_device->m_swapChain)));

		DX::ThrowIfFailed(dxgiDevice->SetMaximumFrameLatency(1));
	}

	DX::ThrowIfFailed(m_device->m_swapChain->SetRotation(m_device->m_outputRotation));

	// Obtain the backbuffer for this window which will be the final 3D rendertarget.
	ComPtr<ID3D11Texture2D> backBuffer;
	DX::ThrowIfFailed(m_device->m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

	// Create a view interface on the rendertarget to use on bind.
	DX::ThrowIfFailed(m_device->m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_device->m_renderTargetView.ReleaseAndGetAddressOf()));

	// Allocate a 2-D surface as the depth/stencil buffer and
	// create a DepthStencil view on this surface to use on bind.
	CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

	ComPtr<ID3D11Texture2D> depthStencil;
	DX::ThrowIfFailed(m_device->m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	DX::ThrowIfFailed(m_device->m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_device->m_depthStencilView.ReleaseAndGetAddressOf()));

	// TODO: Initialize windows-size dependent objects here.

	m_proj = Matrix::CreatePerspectiveFieldOfView(XMConvertToRadians(60.f), float(backBufferWidth) / float(backBufferHeight), 0.01f, 100.f);
	m_world = Matrix::Identity;

}
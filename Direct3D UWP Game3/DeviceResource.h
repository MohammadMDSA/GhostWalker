#pragma once

ref class DeviceResource
{
internal:

	DeviceResource();

	void Initialize();

	void OnDeviceLost();

	// Getters
	ID3D11DeviceContext3* GetDeviceContext()				{ return m_d3dContext.Get(); }
	ID3D11Device3* GetDevice()								{ return m_d3dDevice.Get(); }
	Microsoft::WRL::ComPtr<ID3D11Device3> GetDevice_Com()	{ return m_d3dDevice; }
	int GetOutputWidth()									{ return m_outputWidth; }
	int GetOutputHeight()									{ return m_outputHeight; }
	DXGI_MODE_ROTATION GetOutputRotation()					{ return m_outputRotation; }


	// Setters
	void SetOutputWidth(int width)							{ this->m_outputWidth = width; }
	void SetOutputHeight(int height)						{ this->m_outputHeight = height; }
	void SetOutputRotation(DXGI_MODE_ROTATION rotation)		{ this->m_outputRotation = rotation; }
	void SetWindow(IUnknown* window)						{ this->m_window = window; }

private:
	friend ref class Renderer;

	// Device resources

	IUnknown*										m_window;
	int												m_outputWidth;
	int												m_outputHeight;
	DXGI_MODE_ROTATION                              m_outputRotation;

	D3D_FEATURE_LEVEL                               m_featureLevel;
	Microsoft::WRL::ComPtr<ID3D11Device3>           m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext3>    m_d3dContext;

	Microsoft::WRL::ComPtr<IDXGISwapChain3>         m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

};


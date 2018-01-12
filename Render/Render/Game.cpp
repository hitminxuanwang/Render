#include "headers.h"
#include "Game.h"
#include "Mesh3D.h"
#include "BaseGeometric.h"

using Microsoft::WRL::ComPtr;

Game::Game() :hwnd(0),featureLevel(D3D_FEATURE_LEVEL_9_1)
{
}
void Game::Initailize(HWND window)
{
	hwnd = window;
	HRESULT hr=CreateDevice();
	CreateResources();
}

void Game::Tick()
{
	Render();
}
void Game::Render() 
{
	Clear();
	for (auto item = gameComponents.cbegin(); item != gameComponents.cend(); ++item)
	{
		auto component = (*item);
		component->Render(this);
	}
	Present();
}
void Game::Clear()
{
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	d3dContext->ClearRenderTargetView(renderTargetView, ClearColor);
	d3dContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	d3dContext->OMSetRenderTargets(1,&renderTargetView,depthStencilView);
}
void Game::Present()
{
	HRESULT hr = swapChain->Present(1, 0);
	// If the device was reset we must completely reinitialize the renderer.
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		//OnDeviceLost();
	}
	else
	{
		//DX::ThrowIfFailed(hr);
	}
}

HRESULT Game::CreateDevice()
{
//	UINT creationFlags = 0;
//#ifdef _DEBUG
//	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
//#endif // _DEBUG
//	static const D3D_FEATURE_LEVEL featureLevels[] =
//	{
//		// TODO: Modify for supported Direct3D feature levels (see code below related to 11.1 fallback handling)
//		D3D_FEATURE_LEVEL_11_1,
//		D3D_FEATURE_LEVEL_11_0,
//		D3D_FEATURE_LEVEL_10_1,
//		D3D_FEATURE_LEVEL_10_0,
//		D3D_FEATURE_LEVEL_9_3,
//		D3D_FEATURE_LEVEL_9_2,
//		D3D_FEATURE_LEVEL_9_1,
//	};
//	// Create the DX11 API device object, and get a corresponding context.
//	HRESULT hr = D3D11CreateDevice(
//		nullptr,                                // specify null to use the default adapter
//		D3D_DRIVER_TYPE_HARDWARE,
//		nullptr,                                // leave as nullptr unless software device
//		creationFlags,                          // optionally set debug and Direct2D compatibility flags
//		featureLevels,                          // list of feature levels this app can support
//		_countof(featureLevels),                // number of entries in above list
//		D3D11_SDK_VERSION,                      // always set this to D3D11_SDK_VERSION
//		d3dDevice.ReleaseAndGetAddressOf(),   // returns the Direct3D device created
//		&featureLevel,                        // returns feature level of device created
//		d3dContext.ReleaseAndGetAddressOf()   // returns the device immediate context
//	);


	HRESULT hr = S_OK;
	RECT rc;
	GetClientRect(hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 8;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		D3D_DRIVER_TYPE g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &swapChain, &d3dDevice, &featureLevel, &d3dContext);
		if (SUCCEEDED(hr)) 
			break;
	}
	if (FAILED(hr)) return hr;
	//create a render target view
	ID3D11Texture2D * pBackBuffer = NULL;
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pBackBuffer);
	if (FAILED(hr)) return hr;
	hr = d3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &renderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr)) return hr;
	d3dContext->OMSetRenderTargets(1, &renderTargetView, NULL);

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = d3dDevice->CreateTexture2D(&descDepth, NULL, &depthStencil);
	if (FAILED(hr))
		return hr;
	//g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = d3dDevice->CreateDepthStencilView(depthStencil, &descDSV, &depthStencilView);
	d3dContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
	if (FAILED(hr))
		return hr;
	return S_OK;

}
void Game::CreateResources()
{
	HRESULT hr = S_OK;
	RECT rc;
	GetWindowRect(hwnd, &rc);

	UINT backBufferWidth = std::max<UINT>(rc.right - rc.left, 1);
	UINT backBufferHeight = std::max<UINT>(rc.bottom - rc.top, 1);
	DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
	DXGI_FORMAT depthBufferFormat = (featureLevel >= D3D_FEATURE_LEVEL_10_0) ? DXGI_FORMAT_D32_FLOAT : DXGI_FORMAT_D16_UNORM;
	CD3D11_VIEWPORT viewPort(0.0f, 0.0f, static_cast<float>(backBufferWidth), static_cast<float>(backBufferHeight));
	d3dContext->RSSetViewports(1, &viewPort);
	commonStates.reset(new CommonStates(d3dDevice.Get()));
	this->camera.reset(new Camera(Vector3(0, 1.0f, -5.0f),Vector3(0.0f,0.0f,0.0f),viewPort, hwnd));
	// Load the resources for each of the components.
	
	this->gameComponents.push_back(new Mesh3D(L"../Media//sphere.sdkmesh",L"CubeMap",Vector3(0,0,0),Vector3::Zero,(2.0/3.0f)*Vector3::One));
	this->gameComponents.push_back(new Mesh3D(L"../Media//sphere.sdkmesh", L"CubeMap",Vector3(0,1.6,0),Vector3::Zero,0.5*Vector3::One));
	this->gameComponents.push_back(new BaseGeometric(Vector3(-1.24, 1.24, 0), Vector3(0,0,3.1415f/6.0f), Vector3::One));
	this->gameComponents.push_back(new BaseGeometric(Vector3(1.24, 1.24, 0), Vector3(0, 0, -3.1415f / 6.0f), Vector3::One));
	

	for (auto item = gameComponents.cbegin(); item != gameComponents.cend(); ++item)
	{
		auto component = (*item);
		component->LoadContent(this);
	}
}

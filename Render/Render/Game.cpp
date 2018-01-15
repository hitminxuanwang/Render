#include "headers.h"
#include "Game.h"
#include "Mesh3D.h"
#include "SnowMan.h"
#include "SkyBox.h"
#include "QuadPlane.h"

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
	static float t = 0.0f;
	static DWORD dwTimeStart = 0;
	DWORD dwTimeCur = GetTickCount();
	if (dwTimeStart == 0) dwTimeStart = dwTimeCur;
	t = (dwTimeCur - dwTimeStart) / 1000.0f;
	XMMATRIX rotation_with_time = XMMatrixRotationY(t);


	Clear();
	for (auto item = gameComponents.cbegin(); item != gameComponents.cend(); ++item)
	{
		auto component = (*item);
		component->Render(this,rotation_with_time);
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
	DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
	DXGI_FORMAT depthBufferFormat = (featureLevel >= D3D_FEATURE_LEVEL_10_0) ? DXGI_FORMAT_D32_FLOAT : DXGI_FORMAT_D16_UNORM;
	
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



	CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, width, height, 1, 1, D3D11_BIND_DEPTH_STENCIL, D3D11_USAGE_DEFAULT, 0U, 8);

	ComPtr<ID3D11Texture2D> depthStencil;
	d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf());

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2DMS);

	d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, &depthStencilView);
	
	UINT backBufferWidth = std::max<UINT>(rc.right - rc.left, 1);
	UINT backBufferHeight = std::max<UINT>(rc.bottom - rc.top, 1);
	CD3D11_VIEWPORT viewPort(0.0f, 0.0f, static_cast<float>(backBufferWidth), static_cast<float>(backBufferHeight));
	d3dContext->RSSetViewports(1, &viewPort);
	CommonStates * states = new CommonStates(d3dDevice.Get());
	commonStates.reset(states);

	this->camera.reset(new Camera(Vector3(0,3 , -9), Vector3(0, 3, 1), Vector3::Up, viewPort, hwnd));

	auto samplerState = commonStates->LinearWrap();
	d3dContext->PSSetSamplers(0, 1, &samplerState);

	return S_OK;

}
void Game::CreateResources()
{
	std::vector<Vector3> pos = { Vector3(100.0f, -1.0f, -100.0f) ,Vector3(-100.0f, -1.0f, -100.0f) ,Vector3(-100.0f, -1.0f, 100.0f) ,Vector3(100.0f,-1.0f,100.0f) };
	std::vector<Vector2> uv = { Vector2(1.0f, 1.0f),Vector2(0.0f, 1.0f),Vector2(0.0f, 0.0f),Vector2(1.0f, 0.0f) };

	this->gameComponents.push_back(new SnowMan(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3::One));
	this->gameComponents.push_back(new SnowMan(Vector3(3, 0, 3), Vector3(0, 0, 0), Vector3::One));
	this->gameComponents.push_back(new SkyBox(L"..\\Media\\skybox.dds",1.0f));
	this->gameComponents.push_back(new QuadPlane(pos, uv));
	//this->gameComponents.push_back(new Mesh3D(L"..\\Media\\Tree.sdkmesh", L"..\\Media\\Cubemap.dds", Vector3(0, -3, -5), Vector3::Zero, 3*Vector3::One));



	for (auto item = gameComponents.cbegin(); item != gameComponents.cend(); ++item)
	{
		auto component = (*item);
		component->LoadContent(this);
	}
}

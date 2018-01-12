#pragma once
#include "headers.h"
#include "Camera.h"
#include "GameComponent.h"
//#include "Camera.h"
using namespace DirectX::SimpleMath;

class Game
{
public:
	Game();							///initialize a new instance of Game class
	void Initailize(HWND window);	///initialize Direct3D resources required to run
	void Tick();					///executes basic game loop
	void Render();					///draws the scene
	void Clear();					///clear the backbuffers
	void Present();					///present the backbuffer contents to the screen
	

	/// Sets the appropriate back buffer on the device. You should call this when you are ready to draw to the back buffer. This takes
	/// the fixed back buffer and multisampling (if any) into account so you should use this rather than trying to set the back buffer directly.
	//void SetBackBuffer();	

	ID3D11Device *GetDevice()
	{
		return this->d3dDevice.Get();
	}
	Camera* GetCamera()
	{
		return camera.get();
	}
	ID3D11DeviceContext* GetImmediateContext()
	{
		return d3dContext.Get();
	}
	ID3D11RenderTargetView* const* GetRenderTargetView() const
	{
		return &renderTargetView;
	}
	ID3D11ShaderResourceView* GetRenderTargetShaderResourceView() const
	{
		return renderTargetSRV.Get();
	}

private:

	//void Update(DX::StepTimer const& timer);

	HRESULT CreateDevice();
	void CreateResources();

	//void OnDeviceLost();
	//StepTimer											timer;
	// Application state
	HWND												hwnd;

	// Direct3D Objects
	D3D_FEATURE_LEVEL									featureLevel;
	Microsoft::WRL::ComPtr<ID3D11Device>				d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			d3dContext;

	// Rendering resources
	Microsoft::WRL::ComPtr<IDXGISwapChain>				swapChain;
	ID3D11RenderTargetView*								renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	renderTargetSRV;
	//Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		depthStencilView;
	//Microsoft::WRL::ComPtr<ID3D11Texture2D>				depthStencil;
	ID3D11Texture2D*							depthStencil = NULL;
	ID3D11DepthStencilView*						depthStencilView;



	// A vector of GameComponent pointers. Used to load, update and render commponents.
	std::vector<GameComponent*>							gameComponents;
	std::unique_ptr<DirectX::CommonStates>				commonStates;
	// The active camera.
	std::unique_ptr<Camera>								camera;

};
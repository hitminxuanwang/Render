#include "SkyBox.h"
#include "Game.h"
#include "SnowManEffectFactory.h"


SkyBox::SkyBox(const std::wstring& cubemap,float size)
{
	this->cubefilename = cubemap;
	this->skybox_size = size;
	
}
void SkyBox::LoadContent(Game* game)
{
	auto device = game->GetDevice();
	auto context = game->GetImmediateContext();

	this->vertexShader = VertexShader::CompileShader(device, L"..//Media//shaders//skybox_vertexshader.hlsl");
	this->pixelShader = PixelShader::CompileShader(device, L"..//Media//shaders//skybox_pixelshader.hlsl");

	this->cube = DirectX::GeometricPrimitive::CreateBox(context,XMFLOAT3(1,1,1));
	//GeometricPrimitive* g_cube = this->cube.get();
	//for (size_t i = 0; i < g_cube->; i++)
	{

	}


	const D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	ID3D11InputLayout* meshLayout;

	// Create the input layout that we'll use to pass the model data to the shader.
	HRESULT hr = device->CreateInputLayout(inputLayoutDesc, ARRAYSIZE(inputLayoutDesc),
		this->vertexShader->GetShaderBlob()->GetBufferPointer(), this->vertexShader->GetShaderBlob()->GetBufferSize(), &meshLayout);
	this->inputLayout.reset(meshLayout);
	this->constantBuffer.Initialize(device);


	ID3D11ShaderResourceView * cube;
	CreateDDSTextureFromFile(device, L"..\\Media\\Cubemap.dds", nullptr, &cube);
	this->skyboxCubeMap.reset(cube);

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&sampDesc, &samplerLinear);
}
void SkyBox::Render(Game* game,XMMATRIX matrix)
{
	auto device = game->GetDevice();
	auto context = game->GetImmediateContext();
	auto camera = game->GetCamera();
	auto commonStates = game->GetCommonStates();

	this->cube->Draw(Matrix::Identity, Matrix::Identity, Matrix::Identity, Colors::Black, this->skyboxCubeMap.get(), false, [&]
	{
		Vector3 test = camera->GetPostion();
		this->constantBuffer.Data.world = XMMatrixTranspose(Matrix::CreateTranslation(camera->GetPostion()));
		this->constantBuffer.Data.view = XMMatrixTranspose(camera->GetView());
		this->constantBuffer.Data.projection = XMMatrixTranspose(camera->GetProjection());

		this->constantBuffer.ApplyChanges(context);
		this->constantBuffer.SetVS(context);
		this->constantBuffer.SetPS(context);
		this->vertexShader->Set(context);
		this->pixelShader->Set(context);
		ID3D11ShaderResourceView* texture = this->skyboxCubeMap.get();
		context->PSSetShaderResources(0, 1, &texture);
		context->PSSetSamplers(0, 1, &samplerLinear);
		context->IASetInputLayout(this->inputLayout.get());
		context->RSSetState(commonStates->CullNone());
	});

}

void SkyBox::update() 
{
	

}

SkyBox::~SkyBox()
{

}
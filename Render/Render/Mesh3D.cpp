#include "headers.h"
#include "Game.h"
#include "Mesh3D.h"
#include "Mesh3DEffectFactory.h"

Mesh3D::Mesh3D(LPCWSTR modelFilename, LPCWSTR cubemapFilename,Vector3 pos, Vector3 rotation, Vector3 scale)
{
	this->modelFilename = modelFilename;
	this->cubemapFilename = cubemapFilename;
	this->worldMatrix *= Matrix::CreateScale(scale)*Matrix::CreateRotationX(rotation.x)
		*Matrix::CreateRotationY(rotation.y)*Matrix::CreateRotationZ(rotation.z)*
		Matrix::CreateTranslation(pos);

}
void Mesh3D::CreateModel(Game *game)
{
	auto device = game->GetDevice();
	auto context = game->GetImmediateContext();
	m_states = std::make_unique<CommonStates>(device);
	IEffectFactory * effectFactory = new Mesh3DEffectFactory(device, std::wstring(this->modelFilename));
	this->model = Model::CreateFromSDKMESH(device,this->modelFilename,*effectFactory);

}

void Mesh3D::LoadContent(Game* game)
{
	auto device = game->GetDevice();
	auto context = game->GetImmediateContext();
	CreateModel(game);
	this->vertexShader = VertexShader::CompileShader(device, L"..//Media//shaders//geometric_vertexshader.hlsl");
	this->pixelShader = PixelShader::CompileShader(device, L"..//Media//shaders//geometric_pixelshader.hlsl");
	const D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	ID3D11InputLayout* meshLayout;

	// Create the input layout that we'll use to pass the model data to the shader.
	HRESULT hr = device->CreateInputLayout(inputLayoutDesc, ARRAYSIZE(inputLayoutDesc),
		this->vertexShader->GetShaderBlob()->GetBufferPointer(), this->vertexShader->GetShaderBlob()->GetBufferSize(), &meshLayout);

	this->inputLayout.reset(meshLayout);
	this->constantBuffer.Initialize(device);
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

void Mesh3D::Render(Game* game,XMMATRIX ratation_with_time)
{
	auto context = game->GetImmediateContext();
	auto commonStates = game->GetCommonStates();
	auto camera = game->GetCamera();

	//context->RSSetState(commonStates->CullCounterClockwise());
	this->model->Draw(context, *commonStates, this->worldMatrix, camera->GetView(), camera->GetProjection(), false, [&]
	{
		this->constantBuffer.Data.World = XMMatrixTranspose(this->worldMatrix);
		this->constantBuffer.Data.View = XMMatrixTranspose(camera->GetView());
		this->constantBuffer.Data.Projection = XMMatrixTranspose(camera->GetProjection());

		this->constantBuffer.ApplyChanges(context);
		this->constantBuffer.SetVS(context);
		this->constantBuffer.SetPS(context);
		this->vertexShader->Set(context);
		this->pixelShader->Set(context);
		context->PSSetSamplers(0, 1, &samplerLinear);
		context->IASetInputLayout(this->inputLayout.get());
		context->RSSetState(commonStates->CullCounterClockwise());
		float alpha[] = { 0,0,0,0 };
		context->OMSetBlendState(commonStates->AlphaBlend(), alpha, 0xFFFFFFFF);
	});/*
	this->model->Draw(context, *commonStates, this->worldMatrix, camera->GetView(), camera->GetProjection(), false, [&]
	{
		context->RSSetState(commonStates->CullCounterClockwise());
	});*/
}

Mesh3D::~Mesh3D()
{
}

#include "headers.h"
#include "Game.h"
#include "Mesh3D.h"
#include "Mesh3DEffectFactory.h"

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
};
Mesh3D::Mesh3D(LPCWSTR modelFilename, LPCWSTR cubemapFilename,Vector3 pos, Vector3 rotation, Vector3 scale)
{
	this->modelFilename = modelFilename;
	this->cubemapFilename = cubemapFilename;
	this->worldMatrix *= Matrix::CreateScale(scale)*Matrix::CreateTranslation(pos);
}
void Mesh3D::CreateModel(Game *game)
{
	auto device = game->GetDevice();
	auto context = game->GetImmediateContext();

	//IEffectFactory* effectFactory = new Mesh3DEffctFactory(device, std::wstring(this->modelname));
	IEffectFactory *effectFactory = new Mesh3DEffectFactory(device, std::wstring(this->modelFilename));
	this->model = Model::CreateFromSDKMESH(device,this->modelFilename,*effectFactory);


	/*SimpleVertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
	};
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 24;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	device->CreateBuffer(&bd, &InitData, &vertexbuffer);
	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &vertexbuffer, &stride, &offset);
	WORD indices[] =
	{
		3,1,0,
		2,1,3,

		6,4,5,
		7,4,6,

		11,9,8,
		10,9,11,

		14,12,13,
		15,12,14,

		19,17,16,
		18,17,19,

		22,20,21,
		23,20,22
	};

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 36;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices;
	device->CreateBuffer(&bd, &InitData, &indicesbuffer);
	context->IASetIndexBuffer(indicesbuffer, DXGI_FORMAT_R16_UINT, 0);*/

	//context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void Mesh3D::LoadContent(Game* game)
{
	auto device = game->GetDevice();
	auto context = game->GetImmediateContext();
	// Load the shaders from the compiled CSO files. We're going to override
	// the default shaders created in the effect factory with the following shaders.




	this->vertexShader = VertexShader::CompileShader(device, L"..//Media//shaders//vertexshader.hlsl");
	this->pixelShader = PixelShader::CompileShader(device, L"..//Media//shaders//pixelshader.hlsl");

	// Specifiy what model data we want to access within the shader.
	const D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		// { "COLOR",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ID3D11InputLayout* meshLayout;

	// Create the input layout that we'll use to pass the model data to the shader.
	device->CreateInputLayout(inputLayoutDesc, ARRAYSIZE(inputLayoutDesc),
		this->vertexShader->GetShaderBlob()->GetBufferPointer(), this->vertexShader->GetShaderBlob()->GetBufferSize(), &meshLayout);

	this->inputLayout.reset(meshLayout);
	context->IASetInputLayout(meshLayout);

	CreateModel(game);
	this->constantBuffer.Initialize(device);
	// The shader constant buffer allows us to pass
	// variables that change each frame to the shader.
	//this->constantBuffer.Initialize(device);
	//D3DX11CreateShaderResourceViewFromFile(device, L"../Media//seafloor.dds", NULL, NULL, &textureMap, NULL);
	//DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"..\\Media\\IrradianceMap.dds", nullptr, &irradianceCubemap));
	//ID3D11ShaderResourceView* irradianceCubemap;
	CreateDDSTextureFromFile(device, L"..\\Media\\texture.dds", nullptr, &textureMap);
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

void Mesh3D::Render(Game* game)
{
	auto context = game->GetImmediateContext();
	auto commonStates = game->GetCommonStates();
	auto camera = game->GetCamera();

	//Matrix trans=Matrix::CreateTranslation(Vector3(1.0f,2.0f,3.0f));
	this->model->Draw(context,*commonStates,this->worldMatrix, Matrix::Identity, Matrix::Identity,false,
		[&]() {
		this->constantBuffer.Data.World = XMMatrixTranspose(this->worldMatrix);
		this->constantBuffer.Data.View = XMMatrixTranspose(camera->GetView());
		this->constantBuffer.Data.Projection = XMMatrixTranspose(camera->GetProjection());
		this->constantBuffer.ApplyChanges(context);
		this->constantBuffer.SetVS(context);
		this->constantBuffer.SetPS(context);
		this->vertexShader->Set(context);
		this->pixelShader->Set(context);
		context->PSSetShaderResources(0, 1, &textureMap);
		context->PSSetSamplers(0, 1, &samplerLinear);
		context->IASetInputLayout(this->inputLayout.get());
	});

	//this->constantBuffer.Data.World = XMMatrixTranspose(this->worldMatrix);
	//this->constantBuffer.Data.View = XMMatrixTranspose(camera->GetView());
	//this->constantBuffer.Data.Projection = XMMatrixTranspose(camera->GetProjection());
	//this->constantBuffer.ApplyChanges(context);
	//this->constantBuffer.SetVS(context);
	//this->constantBuffer.SetPS(context);
	//this->vertexShader->Set(context);
	//this->pixelShader->Set(context);
	//context->PSSetShaderResources(0,1,&textureMap);
	//context->PSSetSamplers(0, 1, &samplerLinear);
	//context->IASetInputLayout(this->inputLayout.get());
	//context->DrawIndexed(36, 0, 0);
	// Present the information rendered to the back buffer to the front buffer (the screen)
	//Present(0, 0);
}

Mesh3D::~Mesh3D()
{
}
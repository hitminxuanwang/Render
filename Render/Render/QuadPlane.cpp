#include "QuadPlane.h"
#include "Game.h"

QuadPlane::QuadPlane(std::vector<Vector3> pos,std::vector<Vector2> uv)
{
	this->vertices = new Vertex[4];
	for (size_t i = 0; i < 4; i++)
	{
		this->vertices[i].Position = pos[i];
		this->vertices[i].TextureCoordinates = uv[i];
	}
}
QuadPlane::~QuadPlane()
{
}
void QuadPlane::LoadContent(Game *game)
{
	auto device = game->GetDevice();
	auto context = game->GetImmediateContext();
	this->vertexShader = VertexShader::CompileShader(device, L"..//Media//shaders//vertexshader.hlsl");
	this->pixelShader = PixelShader::CompileShader(device, L"..//Media//shaders//pixelshader.hlsl");
	const D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ID3D11InputLayout* meshLayout;

	device->CreateInputLayout(inputLayoutDesc, ARRAYSIZE(inputLayoutDesc),
		this->vertexShader->GetShaderBlob()->GetBufferPointer(), this->vertexShader->GetShaderBlob()->GetBufferSize(), &meshLayout);

	this->inputLayout.reset(meshLayout);


	CreateDDSTextureFromFile(device, L"..\\Media\\snow_ground.dds", nullptr, &textureMap);
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
void QuadPlane::Render(Game *game,XMMATRIX matrix)
{
	auto context=game->GetImmediateContext();
	auto camera = game->GetCamera();
	auto device = game->GetDevice();
	auto commonstate = game->GetCommonStates();
	context->IASetInputLayout(inputLayout.get());

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = this->vertices;
	device->CreateBuffer(&bd, &InitData, &vertexBuffer);
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	WORD indices[] =
	{
		0,2,1,
		2,0,3
	};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 6;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices;
	device->CreateBuffer(&bd, &InitData, &indexBuffer);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(QuadConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, NULL, &cbuffer);

	QuadConstantBuffer cmatrix;
	cmatrix.world =  XMMatrixTranspose(XMMatrixIdentity());
	cmatrix.view = XMMatrixTranspose(camera->GetView());
	cmatrix.projection = XMMatrixTranspose(camera->GetProjection());
	context->UpdateSubresource(cbuffer, 0, NULL, &cmatrix, 0, 0);


	this->vertexShader->Set(context);
	this->pixelShader->Set(context);
	context->PSSetShaderResources(0, 1, &textureMap);
	context->PSSetSamplers(0, 1, &samplerLinear);
	//context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	//context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->VSSetConstantBuffers(0, 1, &cbuffer);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->RSSetState(commonstate->CullClockwise());
	context->DrawIndexed(6, 0, 0);

}

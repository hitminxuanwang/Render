#include "SnowMan.h"
#include "Game.h"
#include "SnowManEffectFactory.h"

SnowMan::SnowMan(Vector3 pos, Vector3 rotation, Vector3 scale){
	this->worldMatrix *= Matrix::CreateScale(scale)*Matrix::CreateRotationX(rotation.x)
		*Matrix::CreateRotationY(rotation.y)*Matrix::CreateRotationZ(rotation.z)*
		Matrix::CreateTranslation(pos);

	Vector3 element_pos;
	Vector3 element_rotation;
	Vector3 element_scale;
	element_pos = Vector3(0, 0, 0);
	element_rotation = Vector3::Zero;
	element_scale = Vector3::One;
	this->bodyTransform*=Matrix::CreateScale(element_scale)*Matrix::CreateRotationX(element_rotation.x)
		*Matrix::CreateRotationY(element_rotation.y)*Matrix::CreateRotationZ(element_rotation.z)*
		Matrix::CreateTranslation(element_pos);

	element_pos = Vector3(0, 1.3, 0);
	element_scale = 0.8*Vector3::One;
	this->headTransform *= Matrix::CreateScale(element_scale)*Matrix::CreateRotationX(element_rotation.x)
		*Matrix::CreateRotationY(element_rotation.y)*Matrix::CreateRotationZ(element_rotation.z)*
		Matrix::CreateTranslation(element_pos);

	element_pos = Vector3(-0.7,0.7, 0);
	element_rotation = Vector3(0,0,3.1415f/6.0f);
	element_scale = Vector3::One;
	this->leftArmTransform*= Matrix::CreateScale(element_scale)*Matrix::CreateRotationX(element_rotation.x)
		*Matrix::CreateRotationY(element_rotation.y)*Matrix::CreateRotationZ(element_rotation.z)*
		Matrix::CreateTranslation(element_pos);

	element_pos = Vector3(0.7, 0.7, 0);
	element_rotation = Vector3( 0,0, -3.1415f / 6.0f);
	element_scale = Vector3::One;
	this->rightArmTransform *= Matrix::CreateScale(element_scale)*Matrix::CreateRotationX(element_rotation.x)
		*Matrix::CreateRotationY(element_rotation.y)*Matrix::CreateRotationZ(element_rotation.z)*
		Matrix::CreateTranslation(element_pos);

	element_pos = Vector3(0, 1.3, -0.5);
	element_rotation = Vector3(-3.14f/2, 0, 0);
	element_scale = Vector3::One;
	this->noseTransform *= Matrix::CreateScale(element_scale)*Matrix::CreateRotationX(element_rotation.x)
		*Matrix::CreateRotationY(element_rotation.y)*Matrix::CreateRotationZ(element_rotation.z)*
		Matrix::CreateTranslation(element_pos);

	element_pos = Vector3(0.1, 1.5, -0.5);
	element_rotation = Vector3(-3.14f / 2, 0, 0);
	element_scale = Vector3::One;
	this->leftEyeTransform *= Matrix::CreateScale(element_scale)*Matrix::CreateRotationX(element_rotation.x)
		*Matrix::CreateRotationY(element_rotation.y)*Matrix::CreateRotationZ(element_rotation.z)*
		Matrix::CreateTranslation(element_pos);

	element_pos = Vector3(-0.1, 1.5, -0.5);
	element_rotation = Vector3(-3.14f / 2, 0, 0);
	element_scale = Vector3::One;
	this->rightEyeTransform *= Matrix::CreateScale(element_scale)*Matrix::CreateRotationX(element_rotation.x)
		*Matrix::CreateRotationY(element_rotation.y)*Matrix::CreateRotationZ(element_rotation.z)*
		Matrix::CreateTranslation(element_pos);

	element_pos = Vector3(0, 1.1, -0.5);
	element_rotation = Vector3(-3.14f / 2, 0, 0);
	element_scale = Vector3::One;
	this->mouseTransform *= Matrix::CreateScale(element_scale)*Matrix::CreateRotationX(element_rotation.x)
		*Matrix::CreateRotationY(element_rotation.y)*Matrix::CreateRotationZ(element_rotation.z)*
		Matrix::CreateTranslation(element_pos);


}



void SnowMan::LoadContent(Game *game)
{
	auto device = game->GetDevice();
	auto context = game->GetImmediateContext();
	//IEffectFactory* effectFactory = new SnowManEffectFactory(device);
	
	this->body = DirectX::GeometricPrimitive::CreateSphere(context,1.5,64);
	//this->body = DirectX::GeometricPrimitive::CreateCylinder(context, 1,0.1,32);
	this->head = DirectX::GeometricPrimitive::CreateSphere(context, 1.4, 64);
	this->leftArm = DirectX::GeometricPrimitive::CreateCylinder(context, 1, 0.05, 64);
	this->rightArm = DirectX::GeometricPrimitive::CreateCylinder(context, 1, 0.05, 64);
	this->nose = DirectX::GeometricPrimitive::CreateCone(context, 0.1, 0.8, 64);
	this->leftEye = DirectX::GeometricPrimitive::CreateSphere(context, 0.1, 32);
	this->rightEye = DirectX::GeometricPrimitive::CreateSphere(context, 0.1, 32);
	this->mouse = DirectX::GeometricPrimitive::CreateSphere(context, 0.1, 32);

	this->vertexShader = VertexShader::CompileShader(device, L"..//Media//shaders//geometric_vertexshader.hlsl");
	this->pixelShader = PixelShader::CompileShader(device, L"..//Media//shaders//geometric_pixelshader.hlsl");

	const D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	ID3D11InputLayout* meshLayout;

	// Create the input layout that we'll use to pass the model data to the shader.
	HRESULT hr=device->CreateInputLayout(inputLayoutDesc, ARRAYSIZE(inputLayoutDesc),
		this->vertexShader->GetShaderBlob()->GetBufferPointer(), this->vertexShader->GetShaderBlob()->GetBufferSize(), &meshLayout);

	this->inputLayout.reset(meshLayout);
	this->constantBuffer.Initialize(device);

	ID3D11ShaderResourceView *bodyTexture;
	hr = CreateDDSTextureFromFile(device, L"..\\Media\\texture.dds",nullptr,&bodyTexture);
	this->bodyTextureMap.reset(bodyTexture);

	ID3D11ShaderResourceView *armTexture;
	hr = CreateDDSTextureFromFile(device, L"..\\Media\\arm.dds", nullptr, &armTexture);
	this->armTextureMap.reset(armTexture);

	ID3D11ShaderResourceView *noseTexture;
	hr = CreateDDSTextureFromFile(device, L"..\\Media\\nose.dds", nullptr, &noseTexture);
	this->noseTextureMap.reset(noseTexture);

	ID3D11ShaderResourceView *eyeTexture;
	hr = CreateDDSTextureFromFile(device, L"..\\Media\\black.dds", nullptr, &eyeTexture);
	this->eyeTextureMap.reset(eyeTexture);

	ID3D11ShaderResourceView *mouseTexture;
	hr = CreateDDSTextureFromFile(device, L"..\\Media\\mouse.dds", nullptr, &mouseTexture);
	this->mouseTextureMap.reset(mouseTexture);

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
void SnowMan::Render(Game* game,XMMATRIX rotation_with_matrix)
{
	//auto device = game->GetDevice();
	auto context = game->GetImmediateContext();
	auto camera = game->GetCamera();
	auto commonStates = game->GetCommonStates();
	body->Draw(this->worldMatrix, Matrix::Identity, Matrix::Identity, Colors::Black, this->bodyTextureMap.get(), false, [&]
	{
		this->constantBuffer.Data.world= XMMatrixTranspose(this->bodyTransform*this->worldMatrix*rotation_with_matrix);
		this->constantBuffer.Data.view = XMMatrixTranspose(camera->GetView());
		this->constantBuffer.Data.projection = XMMatrixTranspose(camera->GetProjection());

		this->constantBuffer.ApplyChanges(context);
		this->constantBuffer.SetVS(context);
		this->constantBuffer.SetPS(context);
		this->vertexShader->Set(context);
		this->pixelShader->Set(context);
		ID3D11ShaderResourceView* texture = this->bodyTextureMap.get();
		context->PSSetShaderResources(0, 1, &texture);
		context->PSSetSamplers(0, 1, &samplerLinear);
		context->IASetInputLayout(this->inputLayout.get());
		context->RSSetState(commonStates->CullClockwise());
	});
	head->Draw(this->worldMatrix, Matrix::Identity, Matrix::Identity, Colors::Black, this->bodyTextureMap.get(), false, [&]
	{
		this->constantBuffer.Data.world = XMMatrixTranspose(this->headTransform*this->worldMatrix*rotation_with_matrix);
		this->constantBuffer.Data.view = XMMatrixTranspose(camera->GetView());
		this->constantBuffer.Data.projection = XMMatrixTranspose(camera->GetProjection());

		this->constantBuffer.ApplyChanges(context);
		this->constantBuffer.SetVS(context);
		this->constantBuffer.SetPS(context);
		this->vertexShader->Set(context);
		this->pixelShader->Set(context);
		ID3D11ShaderResourceView* texture = this->bodyTextureMap.get();
		context->PSSetShaderResources(0, 1, &texture);
		context->PSSetSamplers(0, 1, &samplerLinear);
		context->IASetInputLayout(this->inputLayout.get());
		context->RSSetState(commonStates->CullClockwise());
	});
	leftArm->Draw(this->worldMatrix, Matrix::Identity, Matrix::Identity, Colors::Black, this->bodyTextureMap.get(), false, [&]
	{
		this->constantBuffer.Data.world = XMMatrixTranspose(this->leftArmTransform*this->worldMatrix*rotation_with_matrix);
		this->constantBuffer.Data.view = XMMatrixTranspose(camera->GetView());
		this->constantBuffer.Data.projection = XMMatrixTranspose(camera->GetProjection());

		this->constantBuffer.ApplyChanges(context);
		this->constantBuffer.SetVS(context);
		this->constantBuffer.SetPS(context);
		this->vertexShader->Set(context);
		this->pixelShader->Set(context);
		ID3D11ShaderResourceView* texture = this->armTextureMap.get();
		context->PSSetShaderResources(0, 1, &texture);
		context->PSSetSamplers(0, 1, &samplerLinear);
		context->IASetInputLayout(this->inputLayout.get());
		context->RSSetState(commonStates->CullClockwise());
	});
	rightArm->Draw(this->worldMatrix, Matrix::Identity, Matrix::Identity, Colors::Black, this->bodyTextureMap.get(), false, [&]
	{
		this->constantBuffer.Data.world = XMMatrixTranspose(this->rightArmTransform*this->worldMatrix*rotation_with_matrix);
		this->constantBuffer.Data.view = XMMatrixTranspose(camera->GetView());
		this->constantBuffer.Data.projection = XMMatrixTranspose(camera->GetProjection());

		this->constantBuffer.ApplyChanges(context);
		this->constantBuffer.SetVS(context);
		this->constantBuffer.SetPS(context);
		this->vertexShader->Set(context);
		this->pixelShader->Set(context);
		ID3D11ShaderResourceView* texture = this->armTextureMap.get();
		context->PSSetShaderResources(0, 1, &texture);
		context->PSSetSamplers(0, 1, &samplerLinear);
		context->IASetInputLayout(this->inputLayout.get());
		context->RSSetState(commonStates->CullClockwise());
	});
	nose->Draw(this->worldMatrix, Matrix::Identity, Matrix::Identity, Colors::Black, this->noseTextureMap.get(), false, [&]
	{
		this->constantBuffer.Data.world = XMMatrixTranspose(this->noseTransform*this->worldMatrix*rotation_with_matrix);
		this->constantBuffer.Data.view = XMMatrixTranspose(camera->GetView());
		this->constantBuffer.Data.projection = XMMatrixTranspose(camera->GetProjection());

		this->constantBuffer.ApplyChanges(context);
		this->constantBuffer.SetVS(context);
		this->constantBuffer.SetPS(context);
		this->vertexShader->Set(context);
		this->pixelShader->Set(context);
		ID3D11ShaderResourceView* texture = this->noseTextureMap.get();
		context->PSSetShaderResources(0, 1, &texture);
		context->PSSetSamplers(0, 1, &samplerLinear);
		context->IASetInputLayout(this->inputLayout.get());
		context->RSSetState(commonStates->CullClockwise());
	});
	leftEye->Draw(this->worldMatrix, Matrix::Identity, Matrix::Identity, Colors::Black, this->eyeTextureMap.get(), false, [&]
	{
		this->constantBuffer.Data.world = XMMatrixTranspose(this->leftEyeTransform*this->worldMatrix*rotation_with_matrix);
		this->constantBuffer.Data.view = XMMatrixTranspose(camera->GetView());
		this->constantBuffer.Data.projection = XMMatrixTranspose(camera->GetProjection());

		this->constantBuffer.ApplyChanges(context);
		this->constantBuffer.SetVS(context);
		this->constantBuffer.SetPS(context);
		this->vertexShader->Set(context);
		this->pixelShader->Set(context);
		ID3D11ShaderResourceView* texture = this->eyeTextureMap.get();
		context->PSSetShaderResources(0, 1, &texture);
		context->PSSetSamplers(0, 1, &samplerLinear);
		context->IASetInputLayout(this->inputLayout.get());
		context->RSSetState(commonStates->CullClockwise());
	});
	rightEye->Draw(this->worldMatrix, Matrix::Identity, Matrix::Identity, Colors::Black, this->eyeTextureMap.get(), false, [&]
	{
		this->constantBuffer.Data.world = XMMatrixTranspose(this->rightEyeTransform*this->worldMatrix*rotation_with_matrix);
		this->constantBuffer.Data.view = XMMatrixTranspose(camera->GetView());
		this->constantBuffer.Data.projection = XMMatrixTranspose(camera->GetProjection());

		this->constantBuffer.ApplyChanges(context);
		this->constantBuffer.SetVS(context);
		this->constantBuffer.SetPS(context);
		this->vertexShader->Set(context);
		this->pixelShader->Set(context);
		ID3D11ShaderResourceView* texture = this->eyeTextureMap.get();
		context->PSSetShaderResources(0, 1, &texture);
		context->PSSetSamplers(0, 1, &samplerLinear);
		context->IASetInputLayout(this->inputLayout.get());
		context->RSSetState(commonStates->CullClockwise());
	});
	mouse->Draw(this->worldMatrix, Matrix::Identity, Matrix::Identity, Colors::Black, this->mouseTextureMap.get(), false, [&]
	{
		this->constantBuffer.Data.world = XMMatrixTranspose(this->mouseTransform*this->worldMatrix*rotation_with_matrix);
		this->constantBuffer.Data.view = XMMatrixTranspose(camera->GetView());
		this->constantBuffer.Data.projection = XMMatrixTranspose(camera->GetProjection());

		this->constantBuffer.ApplyChanges(context);
		this->constantBuffer.SetVS(context);
		this->constantBuffer.SetPS(context);
		this->vertexShader->Set(context);
		this->pixelShader->Set(context);
		ID3D11ShaderResourceView* texture = this->mouseTextureMap.get();
		context->PSSetShaderResources(0, 1, &texture);
		context->PSSetSamplers(0, 1, &samplerLinear);
		context->IASetInputLayout(this->inputLayout.get());
		context->RSSetState(commonStates->CullClockwise());
	});
	context->RSSetState(commonStates->CullNone());
}

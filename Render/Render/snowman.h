#pragma once
#include "headers.h"
#include "GameComponent.h"
#include "GeometricPrimitive.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

struct SnowManConstantBuffer {
	Matrix world;
	Matrix view;
	Matrix projection;
};


class SnowMan :public GameComponent
{
public:
	SnowMan(Vector3 pos, Vector3 rotation, Vector3 scale);
	~SnowMan() {};
	virtual void LoadContent(Game *game);
	virtual void Render(Game* game,XMMATRIX rotation_with_time);
private:
	std::unique_ptr<DirectX::GeometricPrimitive>	body;
	std::unique_ptr<DirectX::GeometricPrimitive>	head;
	std::unique_ptr<DirectX::GeometricPrimitive>	leftArm;
	std::unique_ptr<DirectX::GeometricPrimitive>	rightArm;
	std::unique_ptr<DirectX::GeometricPrimitive>	nose;
	std::unique_ptr<DirectX::GeometricPrimitive>	leftEye;
	std::unique_ptr<DirectX::GeometricPrimitive>	rightEye;
	std::unique_ptr<DirectX::GeometricPrimitive>	mouse;
	Matrix											worldMatrix = XMMatrixIdentity();
	Matrix											bodyTransform = XMMatrixIdentity();
	Matrix											headTransform = XMMatrixIdentity();
	Matrix											leftArmTransform = XMMatrixIdentity();
	Matrix											rightArmTransform = XMMatrixIdentity();
	Matrix											noseTransform = XMMatrixIdentity();
	Matrix											leftEyeTransform = XMMatrixIdentity();
	Matrix											rightEyeTransform = XMMatrixIdentity();
	Matrix											mouseTransform = XMMatrixIdentity();
	std::unique_ptr<VertexShader>					vertexShader;
	std::unique_ptr<PixelShader>					pixelShader;
	ConstantBuffer<SnowManConstantBuffer>			constantBuffer;
	std::unique_ptr<ID3D11InputLayout>				inputLayout;
	ID3D11SamplerState*								samplerLinear = NULL;
	std::unique_ptr<ID3D11ShaderResourceView>		bodyTextureMap;
	std::unique_ptr<ID3D11ShaderResourceView>		armTextureMap;
	std::unique_ptr<ID3D11ShaderResourceView>		noseTextureMap;
	std::unique_ptr<ID3D11ShaderResourceView>		eyeTextureMap;
	std::unique_ptr<ID3D11ShaderResourceView>		mouseTextureMap;
	IEffect *										myeffect;
};

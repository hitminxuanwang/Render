#pragma once
#include "headers.h"
#include "game.h"
#include "GameComponent.h"
#include "GeometricPrimitive.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

struct SkyBoxConstantBuffer {
	Matrix world;
	Matrix view;
	Matrix projection;
};
class SkyBox :public GameComponent
{
public:
	SkyBox(const std::wstring& cubemap,float size);
	~SkyBox();
	void update();
	virtual void LoadContent(Game *game);
	virtual void Render(Game* game, XMMATRIX matrix);
private:
	float skybox_size=1;
	std::wstring cubefilename;
	std::unique_ptr<GeometricPrimitive> box;
	std::unique_ptr<ID3D11ShaderResourceView> skyboxCubeMap;

	std::unique_ptr<VertexShader> vertexShader;
	std::unique_ptr<PixelShader> pixelShader;
	std::unique_ptr<GeometricPrimitive>	cube;
	std::unique_ptr<ID3D11InputLayout> inputLayout;
	ConstantBuffer<SkyBoxConstantBuffer>			constantBuffer;
	ID3D11SamplerState*								samplerLinear = NULL;
};
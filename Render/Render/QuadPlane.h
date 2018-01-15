#pragma once
#include "headers.h"
#include "GameComponent.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;
struct Vertex
{
	Vector3 Position;
	Vector2 TextureCoordinates;
};
struct QuadConstantBuffer
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};

class QuadPlane:public GameComponent
{
public:
	QuadPlane(std::vector<Vector3>, std::vector<Vector2>);
	~QuadPlane();
	virtual void LoadContent(Game *game);
	virtual void Render(Game* game, XMMATRIX matrix);
private:
	std::unique_ptr<VertexShader>						vertexShader;
	std::unique_ptr<PixelShader>						pixelShader;
	std::unique_ptr<ID3D11InputLayout>					inputLayout;
	ID3D11ShaderResourceView *							textureMap;
	ID3D11Buffer*										vertexBuffer = NULL;
	ID3D11Buffer*										indexBuffer = NULL;
	ID3D11SamplerState*									samplerLinear = NULL;
	Vertex*												vertices;
	ID3D11Buffer*										cbuffer;
};
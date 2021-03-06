#pragma once
#include "headers.h"
#include "GameComponent.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "Effects.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

struct Mesh3DConstantBuffer
{
	XMMATRIX World;
	XMMATRIX View;
	XMMATRIX Projection;
	Vector3 CameraPos;
};

class Mesh3D :public GameComponent
{
public:
	Mesh3D(LPCWSTR modelFilename, LPCWSTR cubemapFilename,Vector3 pos, Vector3 rotation, Vector3 scale);
	~Mesh3D();
	virtual void LoadContent(Game* game);
	virtual void Render(Game* game,XMMATRIX rotation_with_matrix);
	void CreateModel(Game* game);
private:
	LPCWSTR												modelFilename;
	LPCWSTR												cubemapFilename;

	std::unique_ptr <Model>								model;
	std::unique_ptr<VertexShader>						vertexShader;
	std::unique_ptr<PixelShader>						pixelShader;
	std::unique_ptr<ID3D11InputLayout>					inputLayout;
	//std::unique_ptr<ID3D11ShaderResourceView>			irradianceMap;
	ID3D11ShaderResourceView *							textureMap;
	//ID3D11ShaderResourceView *							treeTextureMap;

	ConstantBuffer <Mesh3DConstantBuffer>				constantBuffer;
	ID3D11Buffer*										vertexbuffer=NULL;
	ID3D11Buffer*										indicesbuffer = NULL;
	ID3D11SamplerState*									samplerLinear = NULL;
	Matrix												worldMatrix = XMMatrixIdentity();

	std::unique_ptr<DirectX::CommonStates> m_states;
	std::unique_ptr<DirectX::EffectFactory> m_fxFactory;
	std::unique_ptr<DirectX::Model> m_model;
	std::vector<std::shared_ptr<DirectX::IEffect>> m_modelNormal;

};
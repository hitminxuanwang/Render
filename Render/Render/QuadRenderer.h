#pragma once
#include "headers.h"

using namespace DirectX::SimpleMath;
struct Vertex
{
	Vector3 Position;
	Vector2 TextureCoordinates;
};

class QuadRender
{
public:
	QuadRender(ID3D11Device *device);
	~QuadRender();
	void Draw(ID3D11DeviceContext*);
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

};

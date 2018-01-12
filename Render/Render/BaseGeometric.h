#pragma once
#include "headers.h"
#include "GameComponent.h"
#include "GeometricPrimitive.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

class BaseGeometric :public GameComponent
{
public:
	BaseGeometric(Vector3 pos, Vector3 rotation, Vector3 scale)
	{
		this->worldMatrix *= Matrix::CreateScale(scale)*Matrix::CreateRotationX(rotation.x)
			*Matrix::CreateRotationY(rotation.y)*Matrix::CreateRotationZ(rotation.z)*
			Matrix::CreateTranslation(pos);
	};
	~BaseGeometric() {};
	virtual void LoadContent(Game *game);
	virtual void Render(Game* game);
private:
	std::unique_ptr<DirectX::GeometricPrimitive>	shape;
	Matrix											worldMatrix = XMMatrixIdentity();
};

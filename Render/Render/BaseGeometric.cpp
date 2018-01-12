#include "BaseGeometric.h"
#include "Game.h"


void BaseGeometric::LoadContent(Game *game)
{
	auto device = game->GetDevice();
	auto context = game->GetImmediateContext();
	shape = DirectX::GeometricPrimitive::CreateCylinder(context,1,0.1,32);
}
void BaseGeometric::Render(Game* game)
{
	auto device = game->GetDevice();
	auto context = game->GetImmediateContext();
	auto camera = game->GetCamera();

	shape->Draw(this->worldMatrix,camera->GetView(),camera->GetProjection(),Colors::CornflowerBlue);
}
#pragma once
#include "headers.h"
#include "SimpleMath.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;
class Game;

class GameComponent
{
public:
	GameComponent() {}
	virtual ~GameComponent(){}
	virtual void LoadContent(Game *game) = 0;
	virtual void Render(Game* game,XMMATRIX matrix)= 0;
};

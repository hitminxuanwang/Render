#pragma once
#include "headers.h"

class Game;

class GameComponent
{
public:
	GameComponent() {}
	virtual ~GameComponent(){}
	virtual void LoadContent(Game *game) = 0;
	virtual void Render(Game* game)= 0;
};

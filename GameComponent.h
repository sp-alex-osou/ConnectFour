#pragma once

#include "Include.h"

class Game;

class GameComponent
{
public:
	GameComponent(Game& game);
	virtual ~GameComponent() = 0;

	virtual void init();
	virtual void update();
	virtual void draw();
protected:
	Game& game;
	CL_DisplayWindow& window;
};
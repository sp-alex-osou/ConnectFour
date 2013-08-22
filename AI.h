#pragma once

#include "GameComponent.h"
#include "Result.h"
#include "Board.h"

#define WIN_VALUE 1000

#define MIN -1000
#define MAX 1000
#define HORIZON 10

class AI : public GameComponent
{
public:
	AI(Game& game);

	int getSlot() const;
private:
	Result negaMax(int depth, Result alpha, Result beta, bool ai) const;

	mutable Board board;
};


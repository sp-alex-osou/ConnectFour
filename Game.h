#pragma once

#include <list>

#include "Include.h"
#include "GameComponent.h"
#include "Board.h"
#include "AI.h"

#define AI_STARTS false
#define BOARD_WIDTH 7
#define BOARD_HEIGHT 6

class Game
{
public:
	Game(CL_DisplayWindow& window);
	~Game();
	void run();

	Board& getBoard();
	Board::FieldType getFieldType(bool ai) const;

	CL_DisplayWindow& getDisplayWindow() const;
private:
	void init();
	void update();
	void draw();

	void put(int slot);

	void onKeyDown(const CL_InputEvent &key, const CL_InputState &state);
	void onMouseDown(const CL_InputEvent &key, const CL_InputState &state);
	void onWindowClose();

	CL_DisplayWindow& window;

	bool quit;
	bool end;

	CL_Slot slotQuit;
	CL_Slot slotKeyDown;
	CL_Slot slotMouseDown;

	Board* board;
	AI* ai;

	std::list<GameComponent*> components;
};


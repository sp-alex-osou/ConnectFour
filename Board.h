#pragma once

#include "GameComponent.h"

using std::vector;

class Board : public GameComponent
{
public:
	Board(Game& game, const CL_Size& size);

	void draw();
	void init();

	enum FieldType { Red, Yellow, Empty, OutOfBounds };
	enum Result { Win, Error, OK };

	typedef vector<vector<FieldType>>::iterator fieldIterator;

	Result put(int slot, FieldType fieldType);
	bool remove(int slot);

	FieldType operator()(int row, int col) const;
	Board& operator=(const Board& other);

	bool checkIndex(int row, int col) const;

	CL_Size size;	
	CL_Vec2<float> fieldSize;

	int heuristic;
private:
	vector<FieldType> fields;

	bool checkWin(int row, int col, FieldType type);
	bool checkLine(int row, int col, int deltaRow, int deltaCol, FieldType type);
	int getIndex(int row, int col) const;
};


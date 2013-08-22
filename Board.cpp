#include "Board.h"


Board::Board(Game& game, const CL_Size& size) : GameComponent(game), size(size)
{
}


// initialisiert das Spielfeld
void Board::init()
{
	// Felder initialisieren
	fields = vector<FieldType>(size.width * size.height, Empty);

	CL_Rect viewport = window.get_viewport();

	// Feldgröße berechnen
	fieldSize.x = (float)viewport.get_width() / (float)size.width;
	fieldSize.y = (float)viewport.get_height() / (float)size.height;
}


// zeichnet das Spielfeld
void Board::draw()
{
	// für alle Zeilen und alle Spalten
	for (int row = 0; row < size.height; ++row)
		for (int col = 0; col < size.width; ++col)
		{
			// aktuellen Punkt berechnen
			CL_Pointf p = CL_Pointf(col * fieldSize.x, row * fieldSize.y);

			// Viereck zeichnen
			CL_Draw::box(window.get_gc(), p, p + fieldSize, CL_Colorf::white);

			// aktuelles Feld auslesen
			FieldType field = fields[getIndex(row, col)];

			// wenn Feld nicht leer
			if (field != Empty)
			{
				// Farbe des Tokens bestimmen
				CL_Colorf color = (field == Red) ? CL_Colorf::red : CL_Colorf::yellow;
				float radius = 0.9f * min(fieldSize.x, fieldSize.y) / 2.0f;

				// Token zeichnen
				CL_Draw::circle(window.get_gc(), p + fieldSize / 2, radius, color);
			}
		}
}


// einen Token einwerfen
Board::Result Board::put(int col, FieldType fieldType)
{
	// Spielfeldgrenze überprüfen
	if (col < 0 || col >= size.width)
		return Error;

	int row = size.height - 1;

	// Zeile ermitteln
	while (row >= 0 && fields[getIndex(row, col)] != Empty)
		--row;

	if (row == -1)
		return Error;

	// Feld setzen
	fields[getIndex(row, col)] = fieldType;

	// Gewinnsituation überprüfen und Heuristik berechnen
	return checkWin(row, col, fieldType) ? Win : OK;
}


// einen Token entfernen
bool Board::remove(int col)
{
	// Spielfeldgrenze überprüfen
	if (col < 0 || col >= size.width)
		return false;

	int row = 0;

	// Zeile ermitteln
	while (row < size.height && fields[getIndex(row, col)] == Empty)
		++row;

	if (row == size.height)
		return false;

	// Feld leer setzen
	fields[getIndex(row, col)] = Empty;

	return true;
}


// auf ein bestimmtes Feld zugreifen
Board::FieldType Board::operator()(int row, int col) const
{
	// Index überprüfe
	if (!checkIndex(row, col))
		return OutOfBounds;

	// Kopie des Feldwertes zurückliefern
	return fields[getIndex(row, col)];
}


// Assignment-Operator
Board& Board::operator=(const Board& other)
{
	size = other.size;
	fieldSize = other.fieldSize;
	fields = other.fields;

	return *this;
}


// Spielfeld auf Gewinnsituation überprüfen und gleichzeitig Heuristik-Wert berechnen
bool Board::checkWin(int row, int col, FieldType type)
{
	heuristic = 0;

	// Vertikale, Horizontale sowie beide Diagonalen überprüfen
	return checkLine(row, col - 3, 0, 1, type) || checkLine(row - 3, col, 1, 0, type) || 
		checkLine(row - 3, col - 3, 1, 1, type) || checkLine(row - 3, col + 3, 1, -1, type);
}


// eine Linie auf Gewinnsituation überprüfen
bool Board::checkLine(int row, int col, int deltaRow, int deltaCol, FieldType type)
{
	int count = 0, maxCount = 0, tokens = 0;

	bool countTokens = true;

	// 3 Felder vor aktuellem Feld bis 3 Felder nach aktuellem Feld
	for (int i = 7; i > 0 && tokens < 4; i--)
	{
		// Feldwert auslesen
		FieldType field = operator()(row, col);

		// wenn gesuchter Token oder leer
		if (field == type || field == Empty)
			// Zähler für möglichen Vierer erhöhen
			++count;
		else
			// sonst Zähler zurücksetzen
			count = 0;

		// maximalen Wert für möglichen Vierer aktualisieren
		maxCount = max(maxCount, count);

		// wenn zusammenhängende Tokens gezählt werden sollen
		if (countTokens)
		{
			// wenn gesuchter Token
			if (field == type)
				// Zähler für zusammenhängende Tokens erhöhen
				++tokens;
			// wenn nicht gesuchter Token und mittlerer Token noch nicht überschritten
			else if (i > 4)
				// Zähler zurücksetzen
				tokens = 0;
			// wenn nicht gesuchter Token und mittlerer Token bereits überschritten
			else
				// Zählen von zusammenhängenden Tokens beenden
				countTokens = false;
		}

		// zum nächsten Feld in der Linie gehen
		row += deltaRow;
		col += deltaCol;
	}

	// wenn Vierer möglich
	if (maxCount >= 4)
	{
		// Heuristik je nach Anzahl zusammenhängender Tokens erhöhen
		switch (tokens)
		{
			case 1: heuristic += 1; break;
			case 2: heuristic += 5; break;
			case 3: heuristic += 25; break;
		}
	}

	// zurückliefern, ob Gewinnsituation eingetreten ist
	return tokens == 4;
}


// Index im Feld-Array berechnen
int Board::getIndex(int row, int col) const
{
	return row * size.width + col;
}


// Index überprüfen
bool Board::checkIndex(int row, int col) const
{
	return row >= 0 && row < size.height && col >= 0 && col < size.width;
}
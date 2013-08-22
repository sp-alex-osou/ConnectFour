#include "AI.h"
#include "Game.h"

AI::AI(Game& game) : GameComponent(game), board(game.getBoard())
{
}


// berechnet den n�chsten Zug der KI
int AI::getSlot() const
{
	// aktuelles Spielbrett kopieren
	board = game.getBoard();

	// negaMax Algorithmus starten
	return negaMax(HORIZON, Result(MIN), Result(MAX), true).slot;
}


// berechnet rekursiv alle Spielz�ge bis zu einer bestimmten Tiefe
Result AI::negaMax(int depth, Result alpha, Result beta, bool ai) const
{
	// wenn Blattknoten erreicht
	if (depth == 0)
		// Heuristik-Wert des letzten Zuges zur�ckgeben
		return board.heuristic;

	Result child;

	alpha.slot = -1;

	// f�r alle Kinder des aktuellen Knoten
	for (int i = 0; i < board.size.width; ++i)
	{
		// Token an aktueller Position einwerfen
		Board::Result boardResult = board.put(i, game.getFieldType(ai));

		// wenn g�ltige Spalte gew�hlt wurde
		if (boardResult != Board::Error)
		{
			// wenn Gewinnsituation eingetreten
			if (boardResult == Board::Win)
				// Gewinn-Wert durch Tiefe dividieren
				child = Result(WIN_VALUE / (HORIZON - depth + 1), i);
			else
				// sonst negaMax f�r aktuelles Kind starten und zur�ckgelieferten Wert negieren
				child = Result(-negaMax(depth-1, -beta, -alpha, !ai), i);
			
			// zuletzt eingeworfenen Token wieder entfernen
			board.remove(i);

			// Alpha bzw. Beta Cut
			if (child >= beta)
				return Result(beta, i);

			// �berpr�fen, ob Heuristik Wert gr��er als bisheriges Maximum
			if (child > alpha || alpha.slot == -1)
				alpha = child;
		}
	}

	return alpha;
}
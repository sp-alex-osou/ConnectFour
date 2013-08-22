#include "Game.h"

Game::Game(CL_DisplayWindow& window) : window(window), quit(false), end(false)
{
	// Input-Eventhandler anlegen
	slotQuit = window.sig_window_close().connect(this, &Game::onWindowClose);
	slotKeyDown = window.get_ic().get_keyboard().sig_key_down().connect(this, &Game::onKeyDown);
	slotMouseDown = window.get_ic().get_mouse().sig_key_down().connect(this, &Game::onMouseDown);

	// Board und AI erstellen
	board = new Board(*this, CL_Size(BOARD_WIDTH, BOARD_HEIGHT));
	ai = new AI(*this);

	// Board und AI den Spielkomponenten hinzufügen
	components.push_back(board);
	components.push_back(ai);
}


// Destruktor
Game::~Game()
{
	// Spielkomponenten löschen
	for (std::list<GameComponent *>::iterator it = components.begin(); it != components.end(); ++it)
		delete *it;
}


// startet das Spiel
void Game::run()
{
	// Spielkomponenten initialisieren
	init();

	// wenn AI startet
	if (AI_STARTS)
		// Spalte berechnen und Token einwerfen
		board->put(ai->getSlot(), getFieldType(true));

	// solange Spiel nicht zuende
	while (!quit)
	{
		// Spielkomponenten aktualisieren und zeichnen
		update();
		draw();

		window.flip(1);
		CL_KeepAlive::process();
	}
}


// initialisiert die Spielkomponenten
void Game::init()
{	
	for (std::list<GameComponent *>::iterator it = components.begin(); it != components.end(); ++it)
		(*it)->init();
}


// aktualisiert die Spielkomponenten
void Game::update()
{	
	for (std::list<GameComponent *>::iterator it = components.begin(); it != components.end(); ++it)
		(*it)->update();
}


// zeichnet die Spielkomponenten
void Game::draw()
{
	CL_Draw::fill(window.get_gc(), window.get_viewport(), CL_Colorf::black);

	for(std::list<GameComponent*>::iterator it = components.begin(); it != components.end(); ++it)
		(*it)->draw();
}


// liefert das DisplayWindow
CL_DisplayWindow& Game::getDisplayWindow() const
{
	return window;
}


// Event für Schließen des Fensters
void Game::onWindowClose()
{
	quit = true;
}


// Event für Tastendruck
void Game::onKeyDown(const CL_InputEvent &key, const CL_InputState &state)
{
	switch (key.id)
	{
		case CL_KEY_ESCAPE: quit = true;
	}
}


// Event für Mausklick
void Game::onMouseDown(const CL_InputEvent &key, const CL_InputState &state)
{
	switch (key.id)
	{
		case CL_MOUSE_LEFT: put(key.mouse_pos.x / (int)board->fieldSize.x);
	}
}


// einen Token einwerfen
void Game::put(int slot)
{
	// wenn Spiel nicht zuende
	if (!end)
	{
		// Spielertoken einwerfen
		Board::Result result = board->put(slot, getFieldType(false));

		// wenn Einwurf erfolgreich
		if (result != Board::Error)
			// AI Token einwerfen
			end = (result == Board::Win) || (board->put(ai->getSlot(), getFieldType(true)) == Board::Win);
	}
}


// liefert eine Referenz auf das Spielfeld
Board& Game::getBoard()
{
	return *board;
}


// liefert die Token-Farbe von Spieler bzw. AI
Board::FieldType Game::getFieldType(bool ai) const
{
	return (ai) ? Board::Yellow : Board::Red;
}
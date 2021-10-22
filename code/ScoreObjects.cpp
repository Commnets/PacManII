#include "ScoreObjects.hpp"
#include "Game.hpp"

// ---
PacManII::Lives::Lives (int lF)
		: QGAMES::ScoreObject (__PACMANII_LIVESSCOBJID__),
		  _livesForm (lF),
		  _lives (0)
{
	_forms.insert (QGAMES::Forms::value_type (_livesForm, QGAMES::Game::game () -> form (_livesForm)));
	setCurrentForm (_livesForm); 
}

// ---
void PacManII::Lives::setLives (int l)
{
	_lives = l;
	if (_lives < 0) _lives = 0;
	if (_lives > __PACMANII_MAXNUMBEROFLIVESINSCREEN__) 
		_lives = __PACMANII_MAXNUMBEROFLIVESINSCREEN__;
}

// ---
void PacManII::Lives::initialize ()
{
	QGAMES::ScoreObject::initialize ();

	_lives = 0;
}

// ---
void PacManII::Lives::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::Position iPos = QGAMES::Position::_cero;
	QGAMES::Frame* frame = currentForm () -> frame (0);
	QGAMES::bdata w = __BD (frame -> originalForm () -> frameWidth () + 5);
	for (int i = 0; i < _lives; i++, iPos += QGAMES::Vector (w, __BD 0, __BD 0))
		frame -> drawOn (s, p + iPos);
}

// ---
void PacManII::Lives::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __PACMANII_LIVESACTUALIZED__)
		setLives (evnt.values ().openValue (__PACMANII_LIVESVALUEPARAMETER__).intValue ());
	else
		QGAMES::ScoreObject::processEvent (evnt); // To the parent...
}

// ---
PacManII::TimeCounter::TimeCounter (int f)
	: QGAMES::ScoreObjectText (__PACMANII_GAMETIMECOUNTERID__),
	  _font (f)
{
	_forms.insert (QGAMES::Forms::value_type (_font, QGAMES::Game::game () -> form (_font)));
	setCurrentForm (_font); 
	setMaxTextLength (5); // Minutes plus seconds....
	setSpace (0); // All together..
}

// ---
void PacManII::TimeCounter::setSeconds (int nS)
{
	std::string cero (2, '0');
	std::string nMTs = std::to_string (nS / 60);
	std::string nSTs = std::to_string (nS % 60);
	std::string tx = cero.substr (0, 2 - nMTs.length ()) + nMTs + ":" +
		cero.substr (0, 2 - nSTs.length ()) + nSTs;
	setText (tx);
}

// ---
void PacManII::TimeCounter::updatePositions ()
{
	QGAMES::ScoreObjectText::updatePositions ();

	setSeconds (dynamic_cast <PacManII::Game*> (game ()) -> seconds ());
}


// ---
void PacManII::TimeCounter::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __PACMANII_GAMETIMEACTUALIZED__)
		setSeconds (evnt.values ().openValue (__PACMANII_GAMETIMEVALUEPARAMETER__).intValue ());
	else
		QGAMES::ScoreObjectText::processEvent (evnt);
}

// ---
PacManII::ScoreCounter::ScoreCounter (int f)
	: QGAMES::ScoreObjectNumber (__PACMANII_GAMESCOREID__, 0, QGAMES::Forms (), 
		16 /** The difference between the space position and cero position. */),
	  _font (f)
{
	_forms.insert (QGAMES::Forms::value_type (_font, QGAMES::Game::game () -> form (_font)));
	setCurrentForm (_font); 
	setNumberLength (5);
	setSpace (0); // All together...
}

// ---
void PacManII::ScoreCounter::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __PACMANII_SCOREACTUALIZED__)
		setNumber (evnt.values ().openValue (__PACMANII_SCOREVALUEPARAMETER__).intValue ());
	else
		QGAMES::ScoreObjectNumber::processEvent (evnt);
}

// ---
PacManII::FixText::FixText (int id, const std::string& fT, int f)
	: QGAMES::ScoreObjectText (__PACMANII_GAMESCOREFIXTEXTBASEID__ + id, fT),
	  _font (f)
{
	_forms.insert (QGAMES::Forms::value_type (_font, QGAMES::Game::game () -> form (_font)));
	setCurrentForm (_font); 
	setSpace (0); // All together...

	setText (fT);
}

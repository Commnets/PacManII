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
	QGAMES::bdata w = __BD (frame -> originalForm () -> frameWidth () + 2);
	for (int i = 0; i < _lives && i < __PACMANII_MAXNUMBEROFLIVESINSCREEN__; i++, iPos += QGAMES::Vector (w, __BD 0, __BD 0))
		frame -> drawOn (s, p + iPos);
}

// ---
void PacManII::Lives::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __PACMANII_LIVESACTUALIZED__)
		setLives (evnt.values ().openValue (__PACMANII_LIVESVALUEPARAMETER__).intValue ());
		
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

// ---
PacManII::FruitsEaten::FruitsEaten (int lF)
		: QGAMES::ScoreObject (__PACMANII_GAMESCOREFRUITSCARRIEDID__),
		  _fruitsForm (lF),
		  _fruitsEaten ()
{
	_forms.insert (QGAMES::Forms::value_type (_fruitsForm, QGAMES::Game::game () -> form (_fruitsForm)));
	setCurrentForm (_fruitsForm); 
}

// ---
void PacManII::FruitsEaten::setFruitsEaten (const std::string& fE)
{
	_fruitsEaten = { };
	for (auto i : QGAMES::getElementsFromAsInt (fE, ','))
		_fruitsEaten.push_back (i);
}

// ---
void PacManII::FruitsEaten::initialize ()
{
	QGAMES::ScoreObject::initialize ();

	_fruitsEaten = { };
}

// ---
void PacManII::FruitsEaten::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::Position iPos = QGAMES::Position::_cero;
	int fI = (int) _fruitsEaten.size () - __PACMANII_MAXNUMBEROFFRUITSINSCREEN__; if (fI < 0) fI = 0; 
	// The last __PACMANII_MAXNUMBEROFFRUITSINSCREEN__ are presented. No more.
	for (int i = fI; i < (int) _fruitsEaten.size (); i++)
	{
		QGAMES::Frame* frame = currentForm () -> frame (_fruitsEaten [i]);
		frame -> drawOn (s, p + iPos);
		iPos += QGAMES::Vector (__BD (frame -> originalForm () -> frameWidth () - 5), __BD 0, __BD 0);
	}
}

// ---
void PacManII::FruitsEaten::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __PACMANII_FRUITSCARRIEDACTUALIZED__)
		setFruitsEaten (evnt.values ().openValue (__PACMANII_FRUITSCARRIEDVALUEPARAMETER__).strValue ());

	QGAMES::ScoreObject::processEvent (evnt); // To the parent...
}

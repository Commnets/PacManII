#include "Fruit.hpp"
#include "Game.hpp"
#include "Worlds.hpp"
#include "Scenes.hpp"
#include "Maps.hpp"

// ---
void PacManII::Fruit::setType (int t)
{
	assert (t < __PACMANII_NUMBERENTITIES__);

	_type = t;

	switch (_type)
	{
		case 0:
			setCurrentState (__PACMANII_FRUIT01STATESTAYING__);
			break;

		case 1:
			setCurrentState (__PACMANII_FRUIT02STATESTAYING__);
			break;

		case 2:
			setCurrentState (__PACMANII_FRUIT03STATESTAYING__);
			break;

		case 3:
			setCurrentState (__PACMANII_FRUIT04STATESTAYING__);
			break;

		case 4:
			setCurrentState (__PACMANII_FRUIT05STATESTAYING__);
			break;

		case 5:
			setCurrentState (__PACMANII_FRUIT06STATESTAYING__);
			break;

		case 6:
			setCurrentState (__PACMANII_FRUIT07STATESTAYING__);
			break;

		case 7:
			setCurrentState (__PACMANII_FRUIT08STATESTAYING__);
			break;

		case 8:
			setCurrentState (__PACMANII_FRUIT09STATESTAYING__);
			break;

		case 9:
			setCurrentState (__PACMANII_FRUIT10STATESTAYING__);
			break;

		case 10:
			setCurrentState (__PACMANII_FRUIT11STATESTAYING__);
			break;

		case 11:
			setCurrentState (__PACMANII_FRUIT12STATESTAYING__);
			break;

		default:
			assert (false);
	};
}

// ---
void PacManII::Fruit::setStatus (const PacManII::Fruit::Status& st)
{
	switch (st)
	{
		case PacManII::Fruit::Status::_NOTDEFINED:
			{
				setVisible (false);
				onOffSwitch (_SWITCHPOINTSVISIBLE) -> set (false);
			}

			break;

		case PacManII::Fruit::Status::_SHOWN:
			{
				setPosition (mazePositionToMapPosition (pMap () -> fruitPosition ()) - 
					QGAMES::Vector (__BD (visualLength () >> 1), __BD (visualHeight () >> 1), __BD 0)); 
				setVisible (true);
				onOffSwitch (_SWITCHPOINTSVISIBLE) -> set (false);
			}

			break;

		case PacManII::Fruit::Status::_EATEN:
			{
				setVisible (true);
				onOffSwitch (_SWITCHPOINTSVISIBLE) -> set (true);
			}

			break;

		default:
			assert (false);
	}

	_status = st;
}

// ---
void PacManII::Fruit::initialize ()
{
	PacManII::PacmanElement::initialize ();

	reStartAllCounters ();
	reStartAllOnOffSwitches ();

	setType (0); // The very basic initially...

	setStatus (PacManII::Fruit::Status::_NOTDEFINED);
}

// ---
void PacManII::Fruit::updatePositions ()
{
	PacManII::PacmanElement::updatePositions ();

	if (onOffSwitch (_SWITCHPOINTSVISIBLE) -> isOn ())
	{
		if (counter (_COUNTERPOINTSVISIBLE) -> isEnd ())
			setStatus (PacManII::Fruit::Status::_NOTDEFINED);
	}
}

// ---
void PacManII::Fruit::drawOn (QGAMES::Screen* scr, const QGAMES::Position& p)
{
	if (!isVisible ())
		return;

	if (onOffSwitch (_SWITCHPOINTSVISIBLE) -> isOn ())
	{
		PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
		assert (g != nullptr);
		QGAMES::ScoreObjectText * txt = 
			g -> textBuilder () -> createText (__PACMANII_NEWTOUR24GREENLETTERS__, 
				std::to_string (g -> levelDefinition (g -> level ()).bonusPoints ()));
		txt -> drawOn (scr,
			(p == QGAMES::Position::_noPoint) 
				? centerPosition () - 
					QGAMES::Vector (__BD (txt -> visualLength () >> 1), __BD (txt -> visualHeight () >> 1), __BD 0)
				: p);
		delete (txt);
	}
	else
		PacManII::PacmanElement::drawOn (scr, p);
}

// ---
void PacManII::Fruit::whenCollisionWith (QGAMES::Entity* e)
{
	if (dynamic_cast <PacManII::PacMan*> (e) != nullptr && isAlive () && 
		((centerPosition () - e -> centerPosition ()).module () < __BD (visualLength () >> 1)))
		setStatus (PacManII::Fruit::Status::_EATEN);
}

// ---
__IMPLEMENTCOUNTERS__ (PacManII::Fruit::Counters)
{
	addCounter (new QGAMES::Counter (_COUNTERPOINTSVISIBLE, 1 * QGAMES::Game::game () -> framesPerSecond (), true, true));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (PacManII::Fruit::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch (_SWITCHPOINTSVISIBLE, false));
}

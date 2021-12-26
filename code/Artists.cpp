#include "Artists.hpp"
#include "Movements.hpp"
#include "Game.hpp"
#include "Worlds.hpp"

// ---
const PacManII::Map* PacManII::PacmanElement::pMap () const
{ 
	return (dynamic_cast <const Map*> (map ())); 
}

// ---
PacManII::Map* PacManII::PacmanElement::pMap ()
{ 
	return (dynamic_cast <Map*> (map ())); 
}

// ---
QGAMES::MazeModel::PositionInMaze PacManII::PacmanElement::mapPositionToMazePosition (const QGAMES::Position& p) const
{ 
	return ((pMap () == nullptr) 
		? QGAMES::MazeModel::_noPosition : pMap () -> mapPositionToMazePosition (p));
}

// ---
QGAMES::Position PacManII::PacmanElement::mazePositionToMapPosition (const QGAMES::MazeModel::PositionInMaze& p) const
{ 
	return ((pMap () == nullptr) 
		? QGAMES::Position::_noPoint : pMap () -> mazePositionToMapPosition (p)); 
}

// ---
bool PacManII::PacmanElement::doesPositionMatchesTile (const QGAMES::Position& p) const
{ 
	if (pMap () == nullptr)
		return (false);

	QGAMES::Vector c (__BD (visualLength () >> 1), __BD (visualHeight () >> 1), __BD 0);
	return ((p + c) == mazePositionToMapPosition (mapPositionToMazePosition (p + c))); 
}

// ---
void PacManII::Fruit::setType (int t)
{
	assert (t < __PACMANII_NUMBERENTITIES__);

	switch (t)
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

// ---
PacManII::Artist::Artist (int cId, const QGAMES::Forms& f, const QGAMES::Entity::Data& d)
	: PacManII::PacmanElement (cId, f, d),
	  _lastMazePosition (QGAMES::MazeModel::_noPosition),
	  _pathInMaze (), // No defined by default...
	  _referenceArtists (),
	  _nextDirectionWhenPossible (QGAMES::Vector::_cero),
	  _referenceArtistLastMazePositions ()
{ 
	// Adds a buoy to be abble to "stop" out of the process event manipulation...
	assert (!buoys ().empty ()); // Just in case...
	buoys ().insert (QGAMES::Buoys::value_type
		(__PACMANII_TOSTOPBUOYID__, new PacManII::Artist::ToStopBuoy ()));
}

// ---
bool PacManII::Artist::canMove (const QGAMES::Vector& d, const QGAMES::Vector& a)
{
	// TODO

	return (true);
}

// ---
void PacManII::Artist::changeDirectionWhenPossibleTo (const QGAMES::Vector& d)
{ 
	// An Artist can change the direction of the movement when it is moving...
	// The change can happen in any moment between two tiles
	// but it is only executed inmediately when the requested new direction is the opposite to the previous one...
	// If it is not, the request is written down to be executed later.
	if (isMoving () && direction () == -d)
		toMove (d);
	else
		_nextDirectionWhenPossible = d;
}

// ---
void PacManII::Artist::initialize ()
{
	PacManII::PacmanElement::initialize ();

	_lastMazePosition = QGAMES::MazeModel::_noPosition;

	_pathInMaze = { };

	_referenceArtists = { };

	_nextDirectionWhenPossible = QGAMES::Vector::_cero;

	_referenceArtistLastMazePositions = { };
}

// ---
void PacManII::Artist::updatePositions ()
{
	QGAMES::MazeModel::PositionInMaze lP = currentMazePosition ();

	PacManII::PacmanElement::updatePositions ();

	if (currentMazePosition () != lP)
		_lastMazePosition = lP;

	for (auto i : _referenceArtists)
		_referenceArtistLastMazePositions [i -> id ()] = i -> currentMazePosition ();
}

// ---
void PacManII::Artist::drawOn (QGAMES::Screen* scr, const QGAMES::Position& p)
{
	if (!isVisible ())
		return;

	PacManII::PacmanElement::drawOn (scr, p);

#ifndef NDEBUG
	QGAMES::Position posI = mazePositionToMapPosition (currentMazePosition ());
	if (posI != QGAMES::Position::_noPoint)
	{
		QGAMES::Position posT = mazePositionToMapPosition (targetMazePosition ());
		for (int i = 0; i < 3; i++)
			scr -> drawCircle (posT, QGAMES::Vector::_zNormal, __BD (16 - i), __BD (16 - i), __QGAMES_REDCOLOR__, false);
		scr -> drawLine (posI, posT, __QGAMES_REDCOLOR__, 3);
	}
#endif
}

// ---
void PacManII::Artist::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __PACMANII_MOVEMENTDORIENTATIONCHANGED__)
	{
		QGAMES::SetOfOpenValues dC = evnt.values ();
		assert (dC.existOpenValue (0));
		QGAMES::Vector o (dC.openValue (0).strValue ());
		toMove (o);
	}
	else
	if (evnt.code () == __PACMANII_LIMITMOVEMENTREACHED__)
		toStopDeferred ();
	
	PacManII::PacmanElement::processEvent (evnt);
}

// ---
void* PacManII::Artist::ToStopBuoy::treatFor (QGAMES::Element* e)
{
	PacManII::Artist* art = dynamic_cast <PacManII::Artist*> (e);
	assert (art);

	art -> toStand ();

	return (this);
}

// ---
void PacManII::Artist::toStopDeferred ()
{
	PacManII::Artist::ToStopBuoy* b = 
		dynamic_cast <PacManII::Artist::ToStopBuoy*> (buoy (__PACMANII_TOSTOPBUOYID__));
	assert (b); // Just in case...

	b -> active (true); 
}

// ---
QGAMES::MazeModel::PathInMaze& PacManII::Artist::recalculatePathInMazeAvoiding (const std::vector <QGAMES::Vector>& d)
{ 
	return (_pathInMaze = 
		pMap () -> maze ().next2StepsToGoTo (currentMazePosition (), targetMazePosition (), d)); 
}

// ---
bool PacManII::Artist::isArtistEnteringATunnel () const
{
	if (_pathInMaze.size () < 2)
		return (false); // Either it is not moving, or the path has only only step!

	return ((nextMazePosition (1).asVector () - nextMazePosition (0).asVector ()) != 
			 pMap () -> maze ().directionToGetFromTo (nextMazePosition (1), nextMazePosition (0)));
}

// ---
bool PacManII::Artist::hasReferenceArtistsPositionChanged () const
{
	bool result = false;

	for (std::vector <const PacManII::Artist*>::const_iterator i = _referenceArtists.begin ();
			i != _referenceArtists.end () && !result; i++)
	{
		std::map <int, QGAMES::MazeModel::PositionInMaze>::const_iterator j = 
			_referenceArtistLastMazePositions.find ((*i) -> id ());
		if (j != _referenceArtistLastMazePositions.end () && (*i) -> currentMazePosition () != (*j).second)
			result = true;
	}

	return (result);
}

// ---
void PacManII::PacMan::toStand ()
{
	// The orientation is not changed...
	if (!isStanding ())
		setStatus (PacManII::PacMan::Status::_STOPPED);
}

// ---
void PacManII::PacMan::toMove (const QGAMES::Vector& d)
{
	// The orientation is changed always...
	setOrientation (d);

	if (!isMoving ())
		setStatus (PacManII::PacMan::Status::_MOVING);
	else
		setStatus (status ()); // Keeps the same...
}

// ---
void PacManII::PacMan::toChase (bool c)
{
	if (isMoving ())
		setStatus (c ? PacManII::PacMan::Status::_CHASING : PacManII::PacMan::Status::_MOVING);
}

// ---
bool PacManII::PacMan::isEnemy (const PacmanElement* elmnt) const
{ 
	return (dynamic_cast <const PacManII::Monster*> (elmnt) != nullptr); 
}

// ---
void PacManII::PacMan::setScore (int s) 
{
	_score = s; 

	QGAMES::SetOfOpenValues oV;
	oV.addOpenValue (__PACMANII_SCOREVALUEPARAMETER__, QGAMES::OpenValue (_score));
	notify (QGAMES::Event (__PACMANII_SCOREACTUALIZED__, this, oV));

	int mS = 0;
	PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
	assert (g);
	if ((mS = _score / g -> dataGame ().everyToGetAnExtraLive ()) > _lastMulScoreNotified) 
	{
		_lastMulScoreNotified = mS;
		notify (QGAMES::Event (__PACMANII_PACMANPOINTSHIGHLIGHTREACHED__, this, oV));
	}

	g -> setScore (_score); // Keep it...
}

// ---
void PacManII::PacMan::setFruitsEaten (const std::map <int, bool>& fE)
{
	_fruitsEaten = fE;

	std::string dt;
	for (auto i : _fruitsEaten)
		if (i.second)
			dt += std::string (",") + std::to_string (i.first);
	if (dt.length () > 1)
		dt = dt.substr (1); // To remove the first element, that it would be a comma...

	QGAMES::SetOfOpenValues oV;
	oV.addOpenValue (__PACMANII_FRUITSCARRIEDVALUEPARAMETER__, QGAMES::OpenValue (dt));
	notify (QGAMES::Event (__PACMANII_FRUITSCARRIEDACTUALIZED__, this, oV));

	PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
	assert (g);
	g -> setFruitsEaten (_fruitsEaten);
}

// ---
void PacManII::PacMan::initialize ()
{
	PacManII::Artist::initialize ();

	setOrientation (QGAMES::Vector (__BD 1, __BD 0, __BD 0));
	setStatus (PacManII::PacMan::Status::_NOTDEFINED);

	PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
	assert (g != nullptr);
	setScore (g -> score ());
	setFruitsEaten (g -> fruitsEaten ());

	_lastMulScoreNotified = _score / g -> dataGame ().everyToGetAnExtraLive ();
}

// ---
void PacManII::PacMan::updatePositions ()
{
	PacManII::Artist::updatePositions ();

	if (doesPositionMatchesTile (position ()))
	{
		if (isStanding ())
		{
			if (_nextDirectionWhenPossible != QGAMES::Vector::_cero)
				toMove (_nextDirectionWhenPossible);
		}
	}

/*
	For testing purposes only:

	if (_counter++ == 400)
		notify (QGAMES::Event (__1IN2PROBABLE__ ? __PACMANII_PACMANREACHEDGOAL__ : __PACMANII_PACMANDESTROYED__, this));
		notify (QGAMES::Event (__PACMANII_PACMANDESTROYED__, this));
		notify (QGAMES::Event (__PACMANII_PACMANREACHEDGOAL__, this));
*/
}

// ---
void PacManII::PacMan::finalize ()
{
	PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
	assert (g != nullptr);
	g -> setScore (_score);
	g -> setFruitsEaten (_fruitsEaten);

	PacManII::Artist::finalize ();
}

// ---
void PacManII::PacMan::whenCollisionWith (QGAMES::Entity* e)
{
	PacManII::PacmanElement* pE = dynamic_cast <PacManII::PacmanElement*> (e);
	if (e == nullptr)
		return;

	if (pE -> isAlive ())
	{
		if (isEnemy (pE))
			QGAMES::Event (__PACMANII_PACMANDESTROYED__, this);
		else
		{
			if (dynamic_cast <PacManII::Fruit*> (e) != nullptr &&
				((centerPosition () - e -> centerPosition ()).module () < __BD (visualLength () >> 1)))
			{
				PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
				assert (g != nullptr);

				setScore (score () + g -> levelDefinition (g -> level ()).bonusPoints ());

				addFruitEaten (g -> levelDefinition (g -> level ()).bonusSymbolId ());

				game () -> soundBuilder () -> sound (__PACMANII_SOUNDEATFRUIT__) -> play (-1);
			}
		}
		
	}
}

// ---
void PacManII::PacMan::setStatus (const PacManII::PacMan::Status& st)
{
	// In the case of pacman all changes among types of status are allowed
	// It is not the case in Monster...
	
	_status = st;
	
	switch (st)
	{
		case PacManII::PacMan::Status::_NOTDEFINED:
		case PacManII::PacMan::Status::_STOPPED:
			whatToDoWhenStopStatusIsRequested (orientation ());
			break;

		case PacManII::PacMan::Status::_MOVING:
		case PacManII::PacMan::Status::_CHASING:
			whatToDoWhenMovementStatusIsRequested (orientation ());
			break;

		default:
			assert (false); // It shouldn't be here, but just in case...
	}
}

// ---
QGAMES::MazeModel::PositionInMaze PacManII::PacMan::targetMazePosition () const
{
	if (pMap () == nullptr)
		return (QGAMES::MazeModel::_noPosition);

	if (isStanding ())
		return (currentMazePosition ());

	QGAMES::Vector dir = direction ();
	if (isMoving () && 
		_nextDirectionWhenPossible != QGAMES::Vector::_cero && 
		pMap () -> maze ().isPossibleToMoveTo (currentMazePosition (), _nextDirectionWhenPossible))
		dir = _nextDirectionWhenPossible;
	QGAMES::MazeModel::PathInMaze pth = pMap () -> maze ().limitPathFromFollowing (currentMazePosition (), dir);
	assert (!pth.empty ());
	return (pth [(int) pth.size () - 1]);
}

// ---
void PacManII::PacMan::whatToDoWhenStopStatusIsRequested (const QGAMES::Vector& d)
{
	setOrientation (d);
	setStateToStandLookingTo (orientation ()); // Looking to the last direction...
	setMove (QGAMES::Vector::_cero); // ...but not moving... 
	_pathInMaze = { }; // ..and with no path to follow
	changeDirectionWhenPossibleTo (QGAMES::Vector::_cero); // ...finally it stops!
}

// ---
void PacManII::PacMan::whatToDoWhenMovementStatusIsRequested (const QGAMES::Vector& d)
{
	setOrientation (d); // Look to..
	setStateToMoveTo (orientation ()); // ..with the right aspect...
	setMove (orientation ()); // ...and moving also towards there...
	adaptSpeed (); // That depends on many many things...
}

// ---
void PacManII::PacMan::whatToDoOnCurrentPosition ()
{
	PacManII::TilePath* t = dynamic_cast <PacManII::TilePath*> 
		(pMap () -> tileAt (position (), pMap () -> mazeLayer ()));
	if (t == nullptr)
		return;

	if (t -> canBeEaten ())
	{
		if (!t -> alreadyEaten ())
		{
			PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
			assert (g != nullptr);

			t -> eaten (true); // Shoots whatever is needed in the pacman an others....

			game () -> sound (__PACMANII_SOUNDCHOMP__) -> play (__QGAMES_MAINCHARACTERSOUNDCHANNEL__); // Chomp...

			_hasEaten = true;
			adaptSpeed ();

			if (t -> hasPower ())
				setStatus (PacManII::PacMan::Status::_CHASING);

			setScore (score () + 
				(t -> hasPower () 
					? g -> dataGame ().levelDefinition (g -> level ()).pointsPowerBall ()
					: g -> dataGame ().levelDefinition (g -> level ()).pointsBall ()));
		}
		else
		{
			_hasEaten = false;
			adaptSpeed ();
		}
	}
}

// ---
void PacManII::PacMan::setStateToStandLookingTo (const QGAMES::Vector& d)
{
	if (d == QGAMES::Vector (__BD 1, __BD 0, __BD 0))
		setCurrentState (__PACMANII_PACMANSTATESTANDLOOKINGRIGHT__, true);
	else if (d == QGAMES::Vector (__BD -1, __BD 0, __BD 0))
		setCurrentState (__PACMANII_PACMANSTATESTANDLOOKINGLEFT__, true);
	else if (d == QGAMES::Vector (__BD 0, __BD -1, __BD 0))
		setCurrentState (__PACMANII_PACMANSTATESTANDLOOKINGUP__, true);
	else if (d == QGAMES::Vector (__BD 0, __BD 1, __BD 0))
		setCurrentState (__PACMANII_PACMANSTATESTANDLOOKINGDOWN__, true);
	else
		assert (false); // No other orientations are possible..
}

// ---
void PacManII::PacMan::setStateToMoveTo (const QGAMES::Vector& d)
{
	if (d == QGAMES::Vector (__BD 1, __BD 0, __BD 0))
		setCurrentState (__PACMANII_PACMANSTATEEATINGLOOKINGRIGHT__, true);
	else if (d == QGAMES::Vector (__BD -1, __BD 0, __BD 0))
		setCurrentState (__PACMANII_PACMANSTATEEATINGLOOKINGLEFT__, true);
	else if (d == QGAMES::Vector (__BD 0, __BD -1, __BD 0))
		setCurrentState (__PACMANII_PACMANSTATEEATINGLOOKINGUP__, true);
	else if (d == QGAMES::Vector (__BD 0, __BD 1, __BD 0))
		setCurrentState (__PACMANII_PACMANSTATEEATINGLOOKINGDOWN__, true);
	else
		assert (false); // No other orientations are possible...
}

// ---
void PacManII::PacMan::adaptSpeed ()
{
	if (!isMoving ())
		return;

	PacManII::Game* pG = dynamic_cast <PacManII::Game*> (game ());
	assert (pG != nullptr); // Just in case...
	PacManII::MazeMovement* mM = dynamic_cast <PacManII::MazeMovement*> (currentMovement ());
	assert (mM != nullptr); // is moving?

	const PacManII::DataGame::LevelDefinition& lD = pG -> dataGame ().levelDefinition (pG -> level ());
	mM -> setSpeed (_status == PacManII::PacMan::Status::_CHASING 
		? (_hasEaten ? __BD lD.pacmanSpeedWhenEatingFrightingDots () : __BD lD.pacmanSpeedWhenFrighting ()) 
		: (_hasEaten ? __BD lD.pacmanSpeedWhenEatingDots () : __BD lD.pacmanSpeed ()));
}

// ---
QGAMES::MazeModel::PathInMaze& PacManII::PacMan::recalculatePathInMazeAvoiding (const std::vector <QGAMES::Vector>& d)
{
	QGAMES::Vector dir = direction ();
	if (isMoving () && 
		_nextDirectionWhenPossible != QGAMES::Vector::_cero && 
		pMap () -> maze ().isPossibleToMoveTo (currentMazePosition (), _nextDirectionWhenPossible))
		dir = _nextDirectionWhenPossible;
	QGAMES::MazeModel::PathInMaze pth = pMap () -> maze ().limitPathFromFollowing (currentMazePosition (), dir);

	_pathInMaze = { currentMazePosition () };
	if (pth.size () > 1)
		_pathInMaze.push_back (pth [1]);

	return (_pathInMaze);
}

// ---
void PacManII::Monster::setStatus (const PacManII::Monster::Status& st)
{
	// In release compilation version all changes are possible,
	// but under debug mode the right combination is ckecked first!

	_status = st;

	switch (st)
	{
		case PacManII::Monster::Status::_NOTDEFINED:
		case PacManII::Monster::Status::_ATHOME:
			whatToDoWhenStopStatusIsRequested (orientation ());
			break;

		case PacManII::Monster::Status::_EXITINGHOME:
		case PacManII::Monster::Status::_CHASING:
		case PacManII::Monster::Status::_RUNNINGWAY:
		case PacManII::Monster::Status::_TOBEEATEN:
		case PacManII::Monster::Status::_BEINGEATEN:
			whatToDoWhenMovementStatusIsRequested (orientation ());
			break;

		default:
			assert (false); // It shouldn't be heerem but just in case...
	}
}

// ---
void PacManII::Monster::toStand ()
{
	// The orientation is not changed ever...
	if (!isStanding ())
		setStatus (PacManII::Monster::Status::_ATHOME);
	else
		setStatus (status ());
}

// ---
void PacManII::Monster::toMove (const QGAMES::Vector& d)
{
	// The orientation is changed always...
	setOrientation (d);

	if (!isMoving ())
		setStatus (PacManII::Monster::Status::_RUNNINGWAY);
	else
		setStatus (status ()); // Keeps the same...
}

// ---
void PacManII::Monster::initialize ()
{
	PacManII::Artist::initialize ();

	// Always at home initially...
	// The position is set by the scene up!
	setOrientation (QGAMES::Vector (__BD 0, __BD -1, __BD 0));
	setStatus (PacManII::Monster::Status::_NOTDEFINED);
}

// ---
void PacManII::Monster::updatePositions ()
{
	PacManII::Artist::updatePositions ();

	// TODO...
}

// ---
void PacManII::Monster::finalize ()
{
	PacManII::Artist::finalize ();

	toPursuit (nullptr); // Any more...
}

// ---
void PacManII::Monster::whatToDoWhenStopStatusIsRequested (const QGAMES::Vector& d)
{
	setOrientation (d);
	setStateToStandLookingTo (orientation ()); // Looking at the last direction...
	setMove (QGAMES::Vector::_cero); // ...but not moving... 
	_pathInMaze = { }; // ..and with no path to follow
	changeDirectionWhenPossibleTo (QGAMES::Vector::_cero); // ...finally it stops!
}

// ---
void PacManII::Monster::whatToDoWhenMovementStatusIsRequested (const QGAMES::Vector& d)
{
	setOrientation (d); // Look to..
	setStateToMoveTo (orientation ()); // ..with the right aspect...
	setMove (orientation ()); // ...and moving also towards there...
	recalculatePathInMazeAvoiding ({ }); // ...and with a new path
	adaptSpeed (); // That depends on many many things...
}

// ---
QGAMES::MazeModel::PositionInMaze PacManII::Monster::runAwayMazePosition () const
{
	return ((pMap () == nullptr) 
		? QGAMES::MazeModel::_noPosition : pMap () -> monsterRunAwayPosition (monsterNumber ())); 
}

// ---
void PacManII::StandardMonster::toPursuit (const PacManII::Artist* a)
{
	// Standard monsters only pursuit PacMan!
	PacManII::Monster::toPursuit (a);

	if (_toPursuit != nullptr)
	{
		assert (dynamic_cast <const PacManII::PacMan*> (a) != nullptr);
	}
}

// ---
QGAMES::MazeModel::PositionInMaze PacManII::StandardMonster::targetMazePosition () const
{
	QGAMES::MazeModel::PositionInMaze result = QGAMES::MazeModel::_noPosition;

	switch (status ())
	{
		case PacManII::Monster::Status::_NOTDEFINED:
		case PacManII::Monster::Status::_CHASING:
			assert (false); // Not possible at this point (_CHASING will depend on the type of monster)
	
		case PacManII::Monster::Status::_ATHOME:
			result = currentMazePosition (); 
			// Don't move (to a different tile i mean. Probably the could move up down a little bit)
			break;

		case PacManII::Monster::Status::_EXITINGHOME:
			result = pMap () -> monsterExitingHomePosition (); // The same for all...
			break;

		case PacManII::Monster::Status::_RUNNINGWAY:
		case PacManII::Monster::Status::_TOBEEATEN:
			result = runAwayMazePosition ();
			break;

		case PacManII::Monster::Status::_BEINGEATEN:
			result = pMap () -> monsterInitialPosition (monsterNumber ());
			break;

		// Chasing status will deepend on the specific monster...
	}

	return (result);
}

// ---
void PacManII::StandardMonster::setStateToStandLookingTo (const QGAMES::Vector& d)
{
	switch (status ())
	{
		case PacManII::Monster::Status::_NOTDEFINED:
		case PacManII::Monster::Status::_ATHOME:
			{
				if (d == QGAMES::Vector (__BD 0, __BD -1, __BD 0))
					setCurrentState (__PACMANII_MONSTERSTATEATHOMELOOKINGUP__, true);
				else if (d == QGAMES::Vector (__BD 0, __BD 1, __BD 0))
					setCurrentState (__PACMANII_MONSTERSTATEATHOMELOOKINGDOWN__, true);
				else
					assert (false); // No state possible fot other direction...
			}

			break;

		default:
			// To stand is not possible in other status than athome...
			assert (false);
	}
}

// ---
void PacManII::StandardMonster::setStateToMoveTo (const QGAMES::Vector& d)
{
	switch (status ())
	{
		// _ATHOME is considered "standing"

		case PacManII::Monster::Status::_EXITINGHOME:
		case PacManII::Monster::Status::_CHASING:
		case PacManII::Monster::Status::_RUNNINGWAY:
			{
				if (d == QGAMES::Vector (__BD 1, __BD 0, __BD 0))
					setCurrentState (__PACMANII_MONSTERSTATEMOVINGANDLOOKINGRIGHT__, true);
				else if (d == QGAMES::Vector (__BD -1, __BD 0, __BD 0))
					setCurrentState (__PACMANII_MONSTERSTATEMOVINGANDLOOKINGLEFT__, true);
				else if (d == QGAMES::Vector (__BD 0, __BD -1, __BD 0))
					setCurrentState (__PACMANII_MONSTERSTATEMOVINGANDLOOKINGUP__, true);
				else if (d == QGAMES::Vector (__BD 0, __BD 1, __BD 0))
					setCurrentState (__PACMANII_MONSTERSTATEMOVINGANDLOOKINGDOWN__, true);
				else
					assert (false); // No state possible for other direction...
			}

			break;

		case PacManII::Monster::Status::_TOBEEATEN:
			setCurrentState (__PACMANII_MONSTERSTATERUNNINGAWAY__, true);
			break;

		case PacManII::Monster::Status::_BEINGEATEN:
			setCurrentState (__PACMANII_MONSTERSTATEEATEN__, true);
			break;

		default:
			assert (false);
	}
}

// ---
void PacManII::StandardMonster::adaptSpeed ()
{
	if (!isMoving ())
		return;

	PacManII::Game* pG = dynamic_cast <PacManII::Game*> (game ());
	assert (pG != nullptr); // Just in case...
	PacManII::MazeMovement* mM = dynamic_cast <PacManII::MazeMovement*> (currentMovement ());
	assert (mM != nullptr); // is moving?

	const PacManII::DataGame::LevelDefinition& lD = pG -> dataGame ().levelDefinition (pG -> level ());
	mM -> setSpeed (status  () == PacManII::Monster::Status::_BEINGEATEN
		? __BD lD.ghostSpeedWhenBeingFrighten () 
		: __BD lD.ghostSpeed ());
}

// ---
void PacManII::Inky::setReferenceArtists (const std::vector <const PacManII::Artist*>& r)
{
	PacManII::Artist::setReferenceArtists (r);

	if (!_referenceArtists.empty ())
	{
		// It has to be only blinky...
		assert ((int) _referenceArtists.size () == 1 &&
				dynamic_cast <const PacManII::Blinky*> (_referenceArtists [0]) != nullptr);
	}
}

// ---
QGAMES::MazeModel::PositionInMaze PacManII::Inky::targetMazePosition () const
{
	QGAMES::MazeModel::PositionInMaze result = QGAMES::MazeModel::_noPosition;

	switch (status ())
	{
		case PacManII::Monster::Status::_NOTDEFINED:
			result = currentMazePosition ();
			break;

		case PacManII::Monster::Status::_CHASING:
			if (_toPursuit != nullptr && _referenceArtists [0] != nullptr)
				result = QGAMES::MazeModel::PositionInMaze 
					(2 * (_toPursuit -> nextMazePosition (2).asVector () - _referenceArtists [0] -> currentMazePosition ().asVector ()));
			break;

		// The target position for the rest of the status is calculated like the standard one...
		default:
			PacManII::StandardMonster::targetMazePosition ();
	}

	return (result);
}

// ---
QGAMES::MazeModel::PositionInMaze PacManII::Blinky::targetMazePosition () const
{
	QGAMES::MazeModel::PositionInMaze result = QGAMES::MazeModel::_noPosition;

	switch (status ())
	{
		case PacManII::Monster::Status::_NOTDEFINED:
			result = currentMazePosition ();
			break;

		case PacManII::Monster::Status::_CHASING:
			if (_toPursuit != nullptr)
				result = _toPursuit -> currentMazePosition ();
			break;

		// The target position for the rest of the status is calculated like the standard one...
		default:
			PacManII::StandardMonster::targetMazePosition ();
	}

	return (result);
}

// ---
QGAMES::MazeModel::PositionInMaze PacManII::Pinky::targetMazePosition () const
{
	QGAMES::MazeModel::PositionInMaze result = QGAMES::MazeModel::_noPosition;

	switch (status ())
	{
		case PacManII::Monster::Status::_NOTDEFINED:
			result = currentMazePosition ();
			break;

		case PacManII::Monster::Status::_CHASING:
			if (_toPursuit != nullptr)
				result = _toPursuit -> nextMazePosition (4);
			break;

		// The target position for the rest of the status is calculated like the standard one...
		default:
			PacManII::StandardMonster::targetMazePosition ();
	}

	return (result);
}

// ---
QGAMES::MazeModel::PositionInMaze PacManII::Clyde::targetMazePosition () const
{
	QGAMES::MazeModel::PositionInMaze result = QGAMES::MazeModel::_noPosition;

	switch (status ())
	{
		case PacManII::Monster::Status::_NOTDEFINED:
			result = currentMazePosition ();
			break;

		case PacManII::Monster::Status::_CHASING:
			if ((_toPursuit -> currentMazePosition ().asVector () - currentMazePosition ().asVector ()).module () > __BD 8)
				result = _toPursuit -> currentMazePosition ();
			else
				result = runAwayMazePosition ();
			break;

		// The target position for the rest of the status is calculated like the standard one...
		default:
			PacManII::StandardMonster::targetMazePosition ();
	}

	return (result);
}

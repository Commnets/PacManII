#include "Artists.hpp"
#include "Movements.hpp"
#include "Game.hpp"
#include "Worlds.hpp"

// ---
void PacManII::Artist::keepLastReferenceArtistsPosition ()
{
	for (auto i : _referenceArtists)
		_referenceArtistLastMazePositions [i -> id ()] = i -> currentMazePosition ();
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
bool PacManII::Artist::canMove (const QGAMES::Vector& d, const QGAMES::Vector& a)
{
	return (true);
}

// ---
void PacManII::Artist::calculatePathInMazeAvoiding (const std::vector <QGAMES::Vector>& d)
{
	_pathInMaze = pMap () -> maze ().closestAndShortestWayToGoTo (currentMazePosition (), targetMazePosition (), d);
}

// ---
void PacManII::Artist::extractStepsFromPath (int nS)
{
	if ((int) _pathInMaze.size () < nS)
		_pathInMaze = { };
	else
		for (int i = 0; i < nS; i++)
			_pathInMaze.erase (_pathInMaze.begin ());
}

// ---
QGAMES::MazeModel::PositionInMaze PacManII::Artist::mapPositionToMazePosition (const QGAMES::Position& p) const
{ 
	return ((pMap () == nullptr) 
		? QGAMES::MazeModel::_noPosition : pMap () -> mapPositionToMazePosition (p)); 
}

// ---
QGAMES::Position PacManII::Artist::mazePositionToMapPosition (const QGAMES::MazeModel::PositionInMaze& p) const
{ 
	return ((pMap () == nullptr) 
		? QGAMES::Position::_noPoint : pMap () -> mazePositionToMapPosition (p)); 
}

// ---
bool PacManII::Artist::positionMatchesTile (const QGAMES::Position& p) const
{ 
	if (pMap () == nullptr)
		return (false);

	QGAMES::Vector c (__BD (visualLength () >> 1), __BD (visualHeight () >> 1), __BD 0);
	return ((p + c) == mazePositionToMapPosition (mapPositionToMazePosition (p + c))); 
}

// ---
QGAMES::MazeModel::PositionInMaze PacManII::Artist::nextMazePosition (int n) const
{ 
	return (_pathInMaze.empty () 
		? currentMazePosition () : ((n < (int) _pathInMaze.size ()) ? _pathInMaze [n] : QGAMES::MazeModel::_noPosition)); 
}

// ---
void PacManII::Artist::initialize ()
{
	QGAMES::ArtistInATiledMap::initialize ();

	_lastMazePosition = QGAMES::MazeModel::_noPosition;

	changeDirectionWhenPossibleTo (QGAMES::Vector::_cero);

	anulePath ();

	_referenceArtists = { };

	_referenceArtistLastMazePositions = { };
}

// ---
void PacManII::Artist::updatePositions ()
{
	QGAMES::MazeModel::PositionInMaze lP = currentMazePosition ();

	QGAMES::ArtistInATiledMap::updatePositions ();

	if (currentMazePosition () != lP)
		_lastMazePosition = lP;

	if (positionMatchesTile (position ()))
	{
		if (isMoving ())
		{
			if (directionWhenPossible () == QGAMES::Vector::_cero)
				whatToDoWhenStopIsRequested ();
			else
			{
				if (directionWhenPossible () == direction ())
				{
					if (hasReferenceArtistsPositionChanged ())
						calculatePathInMazeAvoiding ({ direction () });
				}
				else
				if (directionWhenPossible () != direction ())
					whatToDoWhenMovementIsRequested (directionWhenPossible ());

				QGAMES::MazeModel::PositionInMaze nP = nextMazePosition (1);
				if (nP == QGAMES::MazeModel::_noPosition)
						whatToDoWhenStopIsRequested ();
				else
					changeDirectionWhenPossibleTo (nP.asVector () - currentMazePosition ().asVector ());
			}
		}
		else 
		if (isStanding ())
		{
			if (directionWhenPossible () != QGAMES::Vector::_cero)
				whatToDoWhenMovementIsRequested (directionWhenPossible ());
		}
	}

	keepLastReferenceArtistsPosition ();
}

// ---
void PacManII::Artist::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __PACMANII_LIMITMOVEMENTREACHED__)
		whatToDoWhenStopIsRequested ();
	
	QGAMES::ArtistInATiledMap::processEvent (evnt);
}

// ---
const PacManII::Map* PacManII::Artist::pMap () const
{ 
	return (dynamic_cast <const Map*> (map ())); 
}

// ---
PacManII::Map* PacManII::Artist::pMap ()
{ 
	return (dynamic_cast <Map*> (map ())); 
}

// ---
bool PacManII::PacMan::isStanding () const
{
	if (currentState () == nullptr)
		return (false);

	static std::vector <int> sts 
		{ __PACMANII_PACMANSTATESTANDLOOKINGRIGHT__,
		  __PACMANII_PACMANSTATESTANDLOOKINGLEFT__,
		  __PACMANII_PACMANSTATESTANDLOOKINGUP__,
		  __PACMANII_PACMANSTATESTANDLOOKINGDOWN__ };

	return (std::find (sts.begin (), sts.end (), currentState () -> id ()) != sts.end ());
}

// ---
bool PacManII::PacMan::isMoving () const
{
	if (currentState () == nullptr)
		return (false);

	static std::vector <int> sts 
		{ __PACMANII_PACMANSTATEEATINGLOOKINGRIGHT__,
		  __PACMANII_PACMANSTATEEATINGLOOKINGLEFT__,
		  __PACMANII_PACMANSTATEEATINGLOOKINGUP__,
		  __PACMANII_PACMANSTATEEATINGLOOKINGDOWN__ };

	return (std::find (sts.begin (), sts.end (), currentState () -> id ()) != sts.end ());
}

// ---
void PacManII::PacMan::setChasing (bool c)
{
	_chasing = c;

	// If it was moving, the speed should be adapted...
	if (isMoving ())
		adaptSpeed ();
}

// ---
void PacManII::PacMan::setScore (int s) 
{
	_score = s; 

	QGAMES::SetOfOpenValues oV;
	oV.addOpenValue (__PACMANII_SCOREVALUEPARAMETER__, QGAMES::OpenValue (_score));
	notify (QGAMES::Event (__PACMANII_SCOREACTUALIZED__, this, oV));
}

// ---
void PacManII::PacMan::toShoot (int f, const QGAMES::Vector& d)
{
	// TODO
}

// ---
void PacManII::Monster::setStatus (const PacManII::Monster::Status& st)
{
	if (_status == st)
		return;

	// In release compilation version all changes are possible,
	// but under debug mode the right combination is ckecked first!

	if (st == PacManII::Monster::Status::_ATHOME)
		assert (_status == PacManII::Monster::Status::_NOTDEFINED ||
				_status == PacManII::Monster::Status::_BEINGEATEN);
	else
	if (st == PacManII::Monster::Status::_EXITINGHOME)
		assert (_status == PacManII::Monster::Status::_ATHOME);
	else
	if (st == PacManII::Monster::Status::_CHASING)
		assert (_status == PacManII::Monster::Status::_EXITINGHOME ||
				_status == PacManII::Monster::Status::_RUNNINGWAY ||
				_status == PacManII::Monster::Status::_TOBEEATEN);
	else
	if (st == PacManII::Monster::Status::_RUNNINGWAY)
		assert (_status == PacManII::Monster::Status::_EXITINGHOME ||
				_status == PacManII::Monster::Status::_CHASING ||
				_status == PacManII::Monster::Status::_TOBEEATEN);
	else
	if (st == PacManII::Monster::Status::_TOBEEATEN)
		assert (_status == PacManII::Monster::Status::_EXITINGHOME ||
				_status == PacManII::Monster::Status::_CHASING ||
				_status == PacManII::Monster::Status::_RUNNINGWAY);
	else
	if (st == PacManII::Monster::Status::_BEINGEATEN)
		assert (_status == PacManII::Monster::Status::_TOBEEATEN);

	_status = st;
}

// ---
QGAMES::MazeModel::PositionInMaze PacManII::PacMan::targetMazePosition () const
{
	if (pMap () == nullptr)
		return (QGAMES::MazeModel::_noPosition);

	if (isStanding ())
		return (currentMazePosition ());

	QGAMES::MazeModel::PathInMaze pth = pMap () -> maze ().limitPathFromFollowing (currentMazePosition (), direction ());
	assert (!pth.empty ());
	return (pth [(int) pth.size () - 1]);
}

// ---
void PacManII::PacMan::initialize ()
{
	PacManII::Artist::initialize ();

	setCurrentState (__PACMANII_PACMANSTATESTANDLOOKINGRIGHT__); 

	PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
	assert (g != nullptr);
	_score = g -> score ();
}

// ---
void PacManII::PacMan::updatePositions ()
{
	PacManII::Artist::updatePositions ();

/*
	For testing purposes only:

	if (_counter++ == 400)
		notify (QGAMES::Event (__1IN2PROBABLE__ ? __PACMANII_PACMANREACHEDGOAL__ : __PACMANII_PACMANDESTROYED__, this));
		notify (QGAMES::Event (__PACMANII_PACMANDESTROYED__, this));
		notify (QGAMES::Event (__PACMANII_PACMANREACHEDGOAL__, this));
*/

	setScore (score () + 1);
}

// ---
void PacManII::PacMan::finalize ()
{
	PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
	assert (g != nullptr);
	g -> setScore (_score);

	PacManII::Artist::finalize ();
}

// ---
void PacManII::PacMan::setStateToStandLookingTo (const QGAMES::Vector& d)
{
	if (d == QGAMES::Vector (__BD 1, __BD 0, __BD 0))
		setCurrentState (__PACMANII_PACMANSTATESTANDLOOKINGRIGHT__);
	else if (d == QGAMES::Vector (__BD -1, __BD 0, __BD 0))
		setCurrentState (__PACMANII_PACMANSTATESTANDLOOKINGLEFT__);
	else if (d == QGAMES::Vector (__BD 0, __BD -1, __BD 0))
		setCurrentState (__PACMANII_PACMANSTATESTANDLOOKINGUP__);
	else if (d == QGAMES::Vector (__BD 0, __BD 1, __BD 0))
		setCurrentState (__PACMANII_PACMANSTATESTANDLOOKINGDOWN__);
	else
		assert (false);
}

// ---
void PacManII::PacMan::setStateToMoveTo (const QGAMES::Vector& d)
{
	if (d == QGAMES::Vector (__BD 1, __BD 0, __BD 0))
		setCurrentState (__PACMANII_PACMANSTATEEATINGLOOKINGRIGHT__);
	else if (d == QGAMES::Vector (__BD -1, __BD 0, __BD 0))
		setCurrentState (__PACMANII_PACMANSTATEEATINGLOOKINGLEFT__);
	else if (d == QGAMES::Vector (__BD 0, __BD -1, __BD 0))
		setCurrentState (__PACMANII_PACMANSTATEEATINGLOOKINGUP__);
	else if (d == QGAMES::Vector (__BD 0, __BD 1, __BD 0))
		setCurrentState (__PACMANII_PACMANSTATEEATINGLOOKINGDOWN__);
	else
		assert (false);
}

// ---
void PacManII::PacMan::whatToDoWhenStopIsRequested ()
{
	setStateToStandLookingTo (orientation ()); // Looking to the last direction...
	setMove (QGAMES::Vector::_cero); // ...but not moving... 
	anulePath (); // ..and with no path to follow

	// Here is is not needed to change th speed...
}

// ---
void PacManII::PacMan::whatToDoWhenMovementIsRequested (const QGAMES::Vector& d)
{
	setOrientation (d); // Look to..
	setStateToMoveTo (orientation ()); // ..with the right aspect...
	setMove (orientation ()); // ...and moving also towards there...
	adaptSpeed ();
	calculatePathInMazeAvoiding ({ -direction () }); // ...and with a new path, and at least avoiding the forbiden direction
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
	mM -> setSpeed (isChasing () ? __BD lD.pacmanSpeedWhenFrighting () : __BD lD.pacmanSpeed ());
}

// ---
QGAMES::MazeModel::PositionInMaze PacManII::Monster::runAwayMazePosition () const
{ 
	return ((pMap () == nullptr) 
		? QGAMES::MazeModel::_noPosition : pMap () -> monsterRunAwayPosition (monsterNumber ())); 
}

// ---
void PacManII::Monster::initialize ()
{
	PacManII::Artist::initialize ();

	// Always at home initially...
	// The position is set by the scene up!
	setStatus (Status::_ATHOME);
}

// ---
void PacManII::Monster::updatePositions ()
{
	PacManII::Artist::updatePositions ();
}

// ---
void PacManII::Monster::finalize ()
{
	PacManII::Artist::finalize ();

	toPursuit (nullptr); // Any more...
}

// ---
void PacManII::Monster::whatToDoWhenStopIsRequested ()
{
	// TODO	
	assert (false);
}

// ---
void PacManII::Monster::whatToDoWhenMovementIsRequested (const QGAMES::Vector& d)
{
	setOrientation (d); // Look to..
	setStateToMoveTo (orientation ()); // ..with the right aspect...
	setMove (orientation ()); // ...and moving also towards there...
	calculatePathInMazeAvoiding ({ }); // ...and with a new path
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
		case Status::_CHASING:
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
void PacManII::Inky::setStateToStandLookingTo (const QGAMES::Vector& d)
{
	switch (status ())
	{
		case PacManII::Monster::Status::_ATHOME:
			{
				if (d == QGAMES::Vector (__BD 0, __BD -1, __BD 0))
					setCurrentState (__PACMANII_INKYSTATEATHOMELOOKINGUP__);
				else if (d == QGAMES::Vector (__BD 0, __BD 1, __BD 0))
					setCurrentState (__PACMANII_INKYSTATEATHOMELOOKINGDOWN__);
				else
					assert (false); // No state possible fot other direction...
			}

			break;

		default:
			assert (false);
	}
}

// ---
void PacManII::Inky::setStateToMoveTo (const QGAMES::Vector& d)
{
	switch (status ())
	{
		// _ATHOME is considered "standing"

		case PacManII::Monster::Status::_EXITINGHOME:
		case PacManII::Monster::Status::_CHASING:
		case PacManII::Monster::Status::_RUNNINGWAY:
			{
				if (d == QGAMES::Vector (__BD 1, __BD 0, __BD 0))
					setCurrentState (__PACMANII_INKYSTATEMOVINGANDLOOKINGRIGHT__);
				else if (d == QGAMES::Vector (__BD -1, __BD 0, __BD 0))
					setCurrentState (__PACMANII_INKYSTATEMOVINGANDLOOKINGLEFT__);
				else if (d == QGAMES::Vector (__BD 0, __BD -1, __BD 0))
					setCurrentState (__PACMANII_INKYSTATEMOVINGANDLOOKINGUP__);
				else if (d == QGAMES::Vector (__BD 0, __BD 1, __BD 0))
					setCurrentState (__PACMANII_INKYSTATEMOVINGANDLOOKINGDOWN__);
				else
					assert (false); // No state possible for other direction...
			}

			break;

		case PacManII::Monster::Status::_TOBEEATEN:
			setCurrentState (__PACMANII_INKYSTATERUNNINGAWAY__);
			break;

		case PacManII::Monster::Status::_BEINGEATEN:
			setCurrentState (__PACMANII_INKYSTATEEATEN__);
			break;

		default:
			assert (false);
	}
}

// ---
QGAMES::MazeModel::PositionInMaze PacManII::Blinky::targetMazePosition () const
{
	QGAMES::MazeModel::PositionInMaze result = QGAMES::MazeModel::_noPosition;

	switch (status ())
	{
		case Status::_CHASING:
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
void PacManII::Blinky::setStateToStandLookingTo (const QGAMES::Vector& d)
{
	switch (status ())
	{
		case PacManII::Monster::Status::_ATHOME:
			{
				if (d == QGAMES::Vector (__BD 0, __BD -1, __BD 0))
					setCurrentState (__PACMANII_BLINKYSTATEATHOMELOOKINGUP__);
				else if (d == QGAMES::Vector (__BD 0, __BD 1, __BD 0))
					setCurrentState (__PACMANII_BLINKYSTATEATHOMELOOKINGDOWN__);
				else
					assert (false); // No state possible fot other direction...
			}

			break;

		default:
			assert (false);
	}
}

// ---
void PacManII::Blinky::setStateToMoveTo (const QGAMES::Vector& d)
{
	switch (status ())
	{
		// _ATHOME is considered "standing"

		case PacManII::Monster::Status::_EXITINGHOME:
		case PacManII::Monster::Status::_CHASING:
		case PacManII::Monster::Status::_RUNNINGWAY:
			{
				if (d == QGAMES::Vector (__BD 1, __BD 0, __BD 0))
					setCurrentState (__PACMANII_BLINKYSTATEMOVINGANDLOOKINGRIGHT__);
				else if (d == QGAMES::Vector (__BD -1, __BD 0, __BD 0))
					setCurrentState (__PACMANII_BLINKYSTATEMOVINGANDLOOKINGLEFT__);
				else if (d == QGAMES::Vector (__BD 0, __BD -1, __BD 0))
					setCurrentState (__PACMANII_BLINKYSTATEMOVINGANDLOOKINGUP__);
				else if (d == QGAMES::Vector (__BD 0, __BD 1, __BD 0))
					setCurrentState (__PACMANII_BLINKYSTATEMOVINGANDLOOKINGDOWN__);
				else
					assert (false); // No state possible for other direction...
			}

			break;

		case PacManII::Monster::Status::_TOBEEATEN:
			setCurrentState (__PACMANII_BLINKYSTATERUNNINGAWAY__);
			break;

		case PacManII::Monster::Status::_BEINGEATEN:
			setCurrentState (__PACMANII_BLINKYSTATEEATEN__);
			break;

		default:
			assert (false);
	}
}

// ---
QGAMES::MazeModel::PositionInMaze PacManII::Pinky::targetMazePosition () const
{
	QGAMES::MazeModel::PositionInMaze result = QGAMES::MazeModel::_noPosition;

	switch (status ())
	{
		case Status::_CHASING:
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
void PacManII::Pinky::setStateToStandLookingTo (const QGAMES::Vector& d)
{
	switch (status ())
	{
		case PacManII::Monster::Status::_ATHOME:
			{
				if (d == QGAMES::Vector (__BD 0, __BD -1, __BD 0))
					setCurrentState (__PACMANII_PINKYSTATEATHOMELOOKINGUP__);
				else if (d == QGAMES::Vector (__BD 0, __BD 1, __BD 0))
					setCurrentState (__PACMANII_PINKYSTATEATHOMELOOKINGDOWN__);
				else
					assert (false); // No state possible fot other direction...
			}

			break;

		default:
			assert (false);
	}
}

// ---
void PacManII::Pinky::setStateToMoveTo (const QGAMES::Vector& d)
{
	switch (status ())
	{
		// _ATHOME is considered "standing"

		case PacManII::Monster::Status::_EXITINGHOME:
		case PacManII::Monster::Status::_CHASING:
		case PacManII::Monster::Status::_RUNNINGWAY:
			{
				if (d == QGAMES::Vector (__BD 1, __BD 0, __BD 0))
					setCurrentState (__PACMANII_PINKYSTATEMOVINGANDLOOKINGRIGHT__);
				else if (d == QGAMES::Vector (__BD -1, __BD 0, __BD 0))
					setCurrentState (__PACMANII_PINKYSTATEMOVINGANDLOOKINGLEFT__);
				else if (d == QGAMES::Vector (__BD 0, __BD -1, __BD 0))
					setCurrentState (__PACMANII_PINKYSTATEMOVINGANDLOOKINGUP__);
				else if (d == QGAMES::Vector (__BD 0, __BD 1, __BD 0))
					setCurrentState (__PACMANII_PINKYSTATEMOVINGANDLOOKINGDOWN__);
				else
					assert (false); // No state possible for other direction...
			}

			break;

		case PacManII::Monster::Status::_TOBEEATEN:
			setCurrentState (__PACMANII_PINKYSTATERUNNINGAWAY__);
			break;

		case PacManII::Monster::Status::_BEINGEATEN:
			setCurrentState (__PACMANII_PINKYSTATEEATEN__);
			break;

		default:
			assert (false);
	}
}


// ---
QGAMES::MazeModel::PositionInMaze PacManII::Clyde::targetMazePosition () const
{
	QGAMES::MazeModel::PositionInMaze result = QGAMES::MazeModel::_noPosition;

	switch (status ())
	{
		case Status::_CHASING:
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

// ---
void PacManII::Clyde::setStateToStandLookingTo (const QGAMES::Vector& d)
{
	switch (status ())
	{
		case PacManII::Monster::Status::_ATHOME:
			{
				if (d == QGAMES::Vector (__BD 0, __BD -1, __BD 0))
					setCurrentState (__PACMANII_CLYDESTATEATHOMELOOKINGUP__);
				else if (d == QGAMES::Vector (__BD 0, __BD 1, __BD 0))
					setCurrentState (__PACMANII_CLYDESTATEATHOMELOOKINGDOWN__);
				else
					assert (false); // No state possible fot other direction...
			}

			break;

		default:
			assert (false);
	}
}

// ---
void PacManII::Clyde::setStateToMoveTo (const QGAMES::Vector& d)
{
	switch (status ())
	{
		// _ATHOME is considered "standing"

		case PacManII::Monster::Status::_EXITINGHOME:
		case PacManII::Monster::Status::_CHASING:
		case PacManII::Monster::Status::_RUNNINGWAY:
			{
				if (d == QGAMES::Vector (__BD 1, __BD 0, __BD 0))
					setCurrentState (__PACMANII_CLYDESTATEMOVINGANDLOOKINGRIGHT__);
				else if (d == QGAMES::Vector (__BD -1, __BD 0, __BD 0))
					setCurrentState (__PACMANII_CLYDESTATEMOVINGANDLOOKINGLEFT__);
				else if (d == QGAMES::Vector (__BD 0, __BD -1, __BD 0))
					setCurrentState (__PACMANII_CLYDESTATEMOVINGANDLOOKINGUP__);
				else if (d == QGAMES::Vector (__BD 0, __BD 1, __BD 0))
					setCurrentState (__PACMANII_CLYDESTATEMOVINGANDLOOKINGDOWN__);
				else
					assert (false); // No state possible for other direction...
			}

			break;

		case PacManII::Monster::Status::_TOBEEATEN:
			setCurrentState (__PACMANII_CLYDESTATERUNNINGAWAY__);
			break;

		case PacManII::Monster::Status::_BEINGEATEN:
			setCurrentState (__PACMANII_CLYDESTATEEATEN__);
			break;

		default:
			assert (false);
	}
}

#include "Monsters.hpp"
#include "Movements.hpp"
#include "Game.hpp"
#include "Maps.hpp"

// ---
PacManII::Monster::Monster (int cId, int mN, const QGAMES::Forms& f, const QGAMES::Entity::Data& d)
	: PacManII::Artist (cId, f, d),
	  _monsterNumber (mN),
	  _points (100), // By default...
	  _status (PacManII::Monster::Status::_NOTDEFINED),
	  _lastStatus (PacManII::Monster::Status::_NOTDEFINED)
{ 
	assert (_monsterNumber >= 0);

	// Adds a buoy to be able to "chase" out of the process event manipulation...
	assert (!buoys ().empty ()); // Just in case...
	buoys ().insert (QGAMES::Buoys::value_type
		(__PACMANII_TOCHASEBUOYID__, new PacManII::Monster::ToChaseBuoy ()));
}

// ---
bool PacManII::Monster::isEnemy (const PacmanElement* elmnt) const
{ 
	return (dynamic_cast <const PacMan*> (elmnt) != nullptr); 
}

// ---
bool PacManII::Monster::isAtHome () const
{
	return ((pMap () == nullptr) 
		? false : pMap () -> maze ().mazeZoneAt (currentMazePosition ()) == __PACMANII_MAZEMONSTERSHOMEZONE__);
}

// ---
bool PacManII::Monster::isInATunnelHall () const
{
	return ((pMap () == nullptr) 
		? false : pMap () -> maze ().mazeZoneAt (currentMazePosition ()) == __PACMANII_MAZETUNELLPATHZONE__);
}

// ---
QGAMES::Vector PacManII::Monster::directionToStartMovement () const
{
	if (pMap () == nullptr)
		return (QGAMES::Vector::_noPoint);

	QGAMES::MazeModel::PositionInMaze tP = QGAMES::MazeModel::_noPosition;
	if (isAtHome ())
	{
		assert (status () == PacManII::Monster::Status::_ATHOME); // Impossible to be in another ststus...

		tP = pMap () -> monsterExitingHomePosition ();
	}
	else
		tP = (status () == PacManII::Monster::Status::_ATHOME) // Blinky e.g is in this status but "out of home"
			? runAwayMazePosition () : targetMazePosition ();

	assert (currentMazePosition () != QGAMES::MazeModel::_noPosition && 
			tP != QGAMES::MazeModel::_noPosition);

	QGAMES::MazeModel::PathInMaze way = pMap () -> maze ().next2StepsToGoTo (currentMazePosition (), tP, { });

	assert (way.size () > 1);

	return (way [1].asVector () - way [0].asVector ());
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
	{
		if (isAtHome ())
			setStatus (PacManII::Monster::Status::_EXITINGHOME);
		else
			setStatus (PacManII::Monster::Status::_RUNNINGWAY);
	}
	else
		setStatus (status ()); // Keeps the same...
}

// ---
void PacManII::Monster::toChase (bool f)
{
	if (isStanding ())
		return; // When standing, it is at home, so impossible to be in this state!
	{
		if (f)
		{
			if (status () == PacManII::Monster::Status::_RUNNINGWAY ||
				status () == PacManII::Monster::Status::_EXITINGHOME)
				setStatus (PacManII::Monster::Status::_CHASING);
		}
		else
		{
			if (status () == PacManII::Monster::Status::_CHASING ||
				status () == PacManII::Monster::Status::_EXITINGHOME)
				setStatus (PacManII::Monster::Status::_RUNNINGWAY);
		}
	}
}

// ---
void PacManII::Monster::toBeThreaten (bool f)
{
	if (isStanding ())
		return; // When standing, it is at home, so impossible to be affected by this action!
	else
	{
		if (f)
		{
			// To be threaten is possible when moving through the maze...
			if (status () == PacManII::Monster::Status::_CHASING ||
				status () == PacManII::Monster::Status::_RUNNINGWAY)
				setStatus (PacManII::Monster::Status::_TOBEEATEN); // Notice that the orientation is not modifyed!
		}
		else
		{
			// And when the monster is not being threaten is chasing pacman!
			if (status () == PacManII::Monster::Status::_TOBEEATEN)
				setStatus (PacManII::Monster::Status::_CHASING); // The orientation is not modified either!
		}
	}
}

// ---
bool PacManII::Monster::canReverseMovement () const
{
	return (
		(
			(_lastStatus != _status) &&
		    (
				(_lastStatus == PacManII::Monster::Status::_ATHOME && _status == PacManII::Monster::Status::_EXITINGHOME) ||
				(_lastStatus == PacManII::Monster::Status::_ATHOME && _status == PacManII::Monster::Status::_RUNNINGWAY) ||
				(_lastStatus == PacManII::Monster::Status::_ATHOME && _status == PacManII::Monster::Status::_CHASING) ||
				(_lastStatus == PacManII::Monster::Status::_EXITINGHOME && _status == PacManII::Monster::Status::_RUNNINGWAY) ||
				(_lastStatus == PacManII::Monster::Status::_EXITINGHOME && _status == PacManII::Monster::Status::_CHASING) ||
				(_lastStatus == PacManII::Monster::Status::_CHASING && _status == PacManII::Monster::Status::_RUNNINGWAY) ||
				(_lastStatus == PacManII::Monster::Status::_RUNNINGWAY && _status == PacManII::Monster::Status::_CHASING) ||
				(_lastStatus == PacManII::Monster::Status::_CHASING && _status == PacManII::Monster::Status::_TOBEEATEN) ||
				(_lastStatus == PacManII::Monster::Status::_RUNNINGWAY && _status == PacManII::Monster::Status::_TOBEEATEN)
			)
		));
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
void PacManII::Monster::processEvent (const QGAMES::Event& evnt)
{
	// The monster reaches a limitation in the movement
	// (if the maze is well designed) either when arrives at home (after being eaten) or after exiting home
	if (evnt.code () == __PACMANII_LIMITMOVEMENTREACHED__)
	{
		// After being eaten, everything starts back (see the block controlling the monster)
		// Stopping the monsteer, it will be understood it is at home!
		if (status () == PacManII::Monster::Status::_BEINGEATEN)
			toStopDeferred ();
		// At any other circunstance, the movement can't stop, so continue the movement...
		else
			notify (QGAMES::Event (evnt.code (), this));
	}

	PacManII::Artist::processEvent (evnt);
}

// ---
void PacManII::Monster::whenCollisionWith (QGAMES::Entity* e)
{
	// TODO
}

// ---
void PacManII::Monster::setStatus (const PacManII::Monster::Status& st)
{
	// In release compilation version all changes are possible,
	// but under debug mode the right combination is ckecked first!

	_lastStatus = _status;
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
	QGAMES::Vector pD = 
		((isMoving () && _status != PacManII::Monster::Status::_EXITINGHOME) && 
			(_lastStatus == PacManII::Monster::Status::_EXITINGHOME || _lastStatus == PacManII::Monster::Status::_ATHOME))
			? preferredDirectionAtStartingToMove () : QGAMES::Vector::_noPoint;
	recalculatePathInMaze (pD); // ...and with a new path
	adaptSpeed (); // That depends on many many things...
}

// ---
void* PacManII::Monster::ToChaseBuoy::treatFor (QGAMES::Element* e)
{
	PacManII::Monster* mter = dynamic_cast <PacManII::Monster*> (e);
	assert (mter != nullptr);

	mter -> toChase (_chasing);

	return (this);
}

// ---
void PacManII::Monster::toChaseDeferred (bool o)
{
	PacManII::Monster::ToChaseBuoy* b = 
		dynamic_cast <PacManII::Monster::ToChaseBuoy*> (buoy (__PACMANII_TOCHASEBUOYID__));
	assert (b != nullptr); // Just in case...

	b -> setChasing (o);

	b -> active (true); 
}

// ---
QGAMES::MazeModel::PathInMaze& PacManII::Monster::recalculatePathInMaze (const QGAMES::Vector& mD)
{
	QGAMES::MazeModel::PositionInMaze cMP = currentMazePosition ();

	// When status changes to reverse th direction can be allowed in certain circunstances
	std::vector <QGAMES::Vector> fD = { }; 
	if (!canReverseMovement ()) fD = { -orientation () }; 
	_lastStatus = _status;

	// When the monster is alive...
	if (isAlive ())
	{
		// The monster cannot enter back home:
		// So, calculate whether the monster is or not at the entry of home
		// and if it is, what is the direction to enter, because it has to be avoided unless it is going back home...
		QGAMES::Vector dirMonstersHome = QGAMES::Vector::_noPoint; // By default it doesn't exit!
		if (cMP == pMap () -> monsterExitingHomePosition ())
		{
			int sz = 0;
			if ((sz = (int) pMap () -> maze ().positionsForZone (__PACMANII_MAZEMONSTERSHOMEZONE__).size ()) != 0)
			{
				QGAMES::MazeModel::PathInMaze pE = pMap () -> maze ().next2StepsToGoTo 
					(cMP, pMap () -> maze ().positionsForZone (__PACMANII_MAZEMONSTERSHOMEZONE__)[sz >> 1], { });
				if ((int) pE.size () > 1)
					dirMonstersHome = pE [1].asVector () - pE [0].asVector ();
			}

			if (dirMonstersHome != QGAMES::Vector::_noPoint) 
				fD.push_back (dirMonstersHome);
		}

		// ...and also there could be some restricted directions (when passing through an area of these)
		std::vector <QGAMES::Vector> rD = pMap () -> maze ().restrictedDirectionsAt (cMP);
		fD.insert (fD.end (), rD.begin (), rD.end ());
	}

	// There can be duplicate elements in fD...it doesn't matter

	return (_pathInMaze = pMap () -> maze ().next2StepsToGoTo (cMP, targetMazePosition (), fD, mD)); 
}

// ---
QGAMES::MazeModel::PositionInMaze PacManII::Monster::runAwayMazePosition () const
{
	return ((pMap () == nullptr) 
		? QGAMES::MazeModel::_noPosition : pMap () -> monsterRunAwayPosition (monsterNumber ())); 
}

// ---
void PacManII::StandardMonster::setReferenceArtists (const std::vector <PacManII::Artist*>& r)
{
	_referenceArtists = { nullptr }; 
	for (auto i : r)
		if (dynamic_cast <PacManII::PacMan*> (i) != nullptr) _referenceArtists [0] = i;

	assert (_referenceArtists [0] != nullptr);
}

// ---
void PacManII::StandardMonster::updatePositions ()
{
	PacManII::Monster::updatePositions ();

	// TODO...
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
	mM -> setSpeed 
		(status () == PacManII::Monster::Status::_BEINGEATEN
			? __BD lD.ghostSpeedWhenBeingFrighten () 
			: ((status () == PacManII::Monster::Status::_EXITINGHOME) 
				? __BD lD.ghostSpeedWhenExitingHome () : __BD lD.ghostSpeed ()));
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
			if (_referenceArtists [0] != nullptr)
				result = _referenceArtists [0] -> currentMazePosition ();
			// When there is no references enough...go home!
			else
				result = runAwayMazePosition (); 
			break;

		// The target position for the rest of the status is calculated like the standard one...
		default:
			result = PacManII::StandardMonster::targetMazePosition ();
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
			if (_referenceArtists [0] != nullptr)
				result = _referenceArtists [0] -> nextXGridPosition (4);
			// When there is no references enough...go home!
			else
				result = runAwayMazePosition (); 
			break;

		// The target position for the rest of the status is calculated like the standard one...
		default:
			result = PacManII::StandardMonster::targetMazePosition ();
	}

	return (result);
}

// ---
void PacManII::Inky::setReferenceArtists (const std::vector <PacManII::Artist*>& r)
{
	_referenceArtists = { nullptr /** The pacman. */, nullptr /** Blinky */ }; 
	for (auto i : r)
	{
		if (dynamic_cast <PacManII::PacMan*> (i) != nullptr) _referenceArtists [0] = i;
		else if (dynamic_cast <PacManII::Blinky*> (i) != nullptr) _referenceArtists [1] = i;
	}

	assert (_referenceArtists [0] != nullptr && _referenceArtists [1] != nullptr);
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
			if (_referenceArtists [0] != nullptr && _referenceArtists [1] != nullptr)
				result = QGAMES::MazeModel::PositionInMaze (
						_referenceArtists [1] -> currentMazePosition ().asVector () + 
							(__BD 2 * (_referenceArtists [0] -> nextXGridPosition (2).asVector () -
								  _referenceArtists [1] -> currentMazePosition ().asVector ())));
			// When there is no references enough...go home!
			else
				result = runAwayMazePosition (); 

			break;

		// The target position for the rest of the status is calculated like the standard one...
		default:
			result = PacManII::StandardMonster::targetMazePosition ();
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
			if (_referenceArtists [0] != nullptr)
			{
				if ((_referenceArtists [0] -> currentMazePosition ().asVector () - 
					 currentMazePosition ().asVector ()).module () > __BD 8)
					result = _referenceArtists [0] -> currentMazePosition ();
				else
					result = runAwayMazePosition ();
			}
			// When there is no references enough...go home!
			else
				result = runAwayMazePosition (); 
			break;

		// The target position for the rest of the status is calculated like the standard one...
		default:
			result = PacManII::StandardMonster::targetMazePosition ();
	}

	return (result);
}

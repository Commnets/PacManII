#include "Artists.hpp"
#include "Movements.hpp"
#include "Game.hpp"
#include "Worlds.hpp"

// ---
PacManII::Artist::Artist (int cId, const QGAMES::Forms& f, const QGAMES::Entity::Data& d)
	: QGAMES::ArtistInATiledMap (cId, f, d),
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
	{
		setMove (d);

		setStateToMoveTo (d); // It is important to notice that neither the aspect nor the movement
							  // has to change if the new state doesn't imply so!
	}
	else
		_nextDirectionWhenPossible = d;
}

// ---
void PacManII::Artist::initialize ()
{
	QGAMES::ArtistInATiledMap::initialize ();

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

	QGAMES::ArtistInATiledMap::updatePositions ();

	if (currentMazePosition () != lP)
		_lastMazePosition = lP;

	for (auto i : _referenceArtists)
		_referenceArtistLastMazePositions [i -> id ()] = i -> currentMazePosition ();
}

// ---
void PacManII::Artist::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __PACMANII_MOVEMENTDORIENTATIONCHANGED__)
	{
		QGAMES::SetOfOpenValues dC = evnt.values ();
		assert (dC.existOpenValue (0));
		QGAMES::Vector o (dC.openValue (0).strValue ());
		setOrientation (o);
		setStateToMoveTo (o);
	}
	else
	if (evnt.code () == __PACMANII_LIMITMOVEMENTREACHED__)
		toStopDeferred ();
	
	QGAMES::ArtistInATiledMap::processEvent (evnt);
}

// ---
void* PacManII::Artist::ToStopBuoy::treatFor (QGAMES::Element* e)
{
	PacManII::Artist* art = dynamic_cast <PacManII::Artist*> (e);
	assert (art);

	art -> whatToDoWhenStopIsRequested ();

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
QGAMES::MazeModel::PathInMaze& PacManII::Artist::recalculatePathInMazeAvoiding (const std::vector <QGAMES::Vector>& d)
{ 
	return (_pathInMaze = 
		pMap () -> maze ().next2StepsToGoTo (currentMazePosition (), targetMazePosition (), d)); 
}

// ---
bool PacManII::Artist::doesPositionMatchesTile (const QGAMES::Position& p) const
{ 
	if (pMap () == nullptr)
		return (false);

	QGAMES::Vector c (__BD (visualLength () >> 1), __BD (visualHeight () >> 1), __BD 0);
	return ((p + c) == mazePositionToMapPosition (mapPositionToMazePosition (p + c))); 
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

	if (doesPositionMatchesTile (position ()))
	{
		if (isStanding ())
		{
			if (_nextDirectionWhenPossible != QGAMES::Vector::_cero)
				whatToDoWhenMovementIsRequested (_nextDirectionWhenPossible);
		}
	}

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
		setCurrentState (__PACMANII_PACMANSTATEEATINGLOOKINGRIGHT__, true);
	else if (d == QGAMES::Vector (__BD -1, __BD 0, __BD 0))
		setCurrentState (__PACMANII_PACMANSTATEEATINGLOOKINGLEFT__, true);
	else if (d == QGAMES::Vector (__BD 0, __BD -1, __BD 0))
		setCurrentState (__PACMANII_PACMANSTATEEATINGLOOKINGUP__, true);
	else if (d == QGAMES::Vector (__BD 0, __BD 1, __BD 0))
		setCurrentState (__PACMANII_PACMANSTATEEATINGLOOKINGDOWN__, true);
	else
		assert (false);
}

// ---
void PacManII::PacMan::whatToDoWhenStopIsRequested ()
{
	setStateToStandLookingTo (orientation ()); // Looking to the last direction...
	setMove (QGAMES::Vector::_cero); // ...but not moving... 
	_pathInMaze = { }; // ..and with no path to follow
	changeDirectionWhenPossibleTo (QGAMES::Vector::_cero); // ...finally it stops!
}

// ---
void PacManII::PacMan::whatToDoWhenMovementIsRequested (const QGAMES::Vector& d)
{
	setOrientation (d); // Look to..
	setStateToMoveTo (orientation ()); // ..with the right aspect...
	setMove (orientation ()); // ...and moving also towards there...
	adaptSpeed (); // That depends on many many things...
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
	recalculatePathInMazeAvoiding ({ }); // ...and with a new path
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
					setCurrentState (__PACMANII_INKYSTATEMOVINGANDLOOKINGRIGHT__, true);
				else if (d == QGAMES::Vector (__BD -1, __BD 0, __BD 0))
					setCurrentState (__PACMANII_INKYSTATEMOVINGANDLOOKINGLEFT__, true);
				else if (d == QGAMES::Vector (__BD 0, __BD -1, __BD 0))
					setCurrentState (__PACMANII_INKYSTATEMOVINGANDLOOKINGUP__, true);
				else if (d == QGAMES::Vector (__BD 0, __BD 1, __BD 0))
					setCurrentState (__PACMANII_INKYSTATEMOVINGANDLOOKINGDOWN__, true);
				else
					assert (false); // No state possible for other direction...
			}

			break;

		case PacManII::Monster::Status::_TOBEEATEN:
			setCurrentState (__PACMANII_INKYSTATERUNNINGAWAY__, true);
			break;

		case PacManII::Monster::Status::_BEINGEATEN:
			setCurrentState (__PACMANII_INKYSTATEEATEN__, true);
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
					setCurrentState (__PACMANII_BLINKYSTATEMOVINGANDLOOKINGRIGHT__, true);
				else if (d == QGAMES::Vector (__BD -1, __BD 0, __BD 0))
					setCurrentState (__PACMANII_BLINKYSTATEMOVINGANDLOOKINGLEFT__, true);
				else if (d == QGAMES::Vector (__BD 0, __BD -1, __BD 0))
					setCurrentState (__PACMANII_BLINKYSTATEMOVINGANDLOOKINGUP__, true);
				else if (d == QGAMES::Vector (__BD 0, __BD 1, __BD 0))
					setCurrentState (__PACMANII_BLINKYSTATEMOVINGANDLOOKINGDOWN__, true);
				else
					assert (false); // No state possible for other direction...
			}

			break;

		case PacManII::Monster::Status::_TOBEEATEN:
			setCurrentState (__PACMANII_BLINKYSTATERUNNINGAWAY__, true);
			break;

		case PacManII::Monster::Status::_BEINGEATEN:
			setCurrentState (__PACMANII_BLINKYSTATEEATEN__, true);
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
					setCurrentState (__PACMANII_PINKYSTATEMOVINGANDLOOKINGRIGHT__, true);
				else if (d == QGAMES::Vector (__BD -1, __BD 0, __BD 0))
					setCurrentState (__PACMANII_PINKYSTATEMOVINGANDLOOKINGLEFT__, true);
				else if (d == QGAMES::Vector (__BD 0, __BD -1, __BD 0))
					setCurrentState (__PACMANII_PINKYSTATEMOVINGANDLOOKINGUP__, true);
				else if (d == QGAMES::Vector (__BD 0, __BD 1, __BD 0))
					setCurrentState (__PACMANII_PINKYSTATEMOVINGANDLOOKINGDOWN__, true);
				else
					assert (false); // No state possible for other direction...
			}

			break;

		case PacManII::Monster::Status::_TOBEEATEN:
			setCurrentState (__PACMANII_PINKYSTATERUNNINGAWAY__, true);
			break;

		case PacManII::Monster::Status::_BEINGEATEN:
			setCurrentState (__PACMANII_PINKYSTATEEATEN__, true);
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
					setCurrentState (__PACMANII_CLYDESTATEMOVINGANDLOOKINGRIGHT__, true);
				else if (d == QGAMES::Vector (__BD -1, __BD 0, __BD 0))
					setCurrentState (__PACMANII_CLYDESTATEMOVINGANDLOOKINGLEFT__, true);
				else if (d == QGAMES::Vector (__BD 0, __BD -1, __BD 0))
					setCurrentState (__PACMANII_CLYDESTATEMOVINGANDLOOKINGUP__, true);
				else if (d == QGAMES::Vector (__BD 0, __BD 1, __BD 0))
					setCurrentState (__PACMANII_CLYDESTATEMOVINGANDLOOKINGDOWN__, true);
				else
					assert (false); // No state possible for other direction...
			}

			break;

		case PacManII::Monster::Status::_TOBEEATEN:
			setCurrentState (__PACMANII_CLYDESTATERUNNINGAWAY__, true);
			break;

		case PacManII::Monster::Status::_BEINGEATEN:
			setCurrentState (__PACMANII_CLYDESTATEEATEN__, true);
			break;

		default:
			assert (false);
	}
}

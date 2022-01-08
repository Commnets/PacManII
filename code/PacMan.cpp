#include "PacMan.hpp"
#include "Movements.hpp"
#include "Game.hpp"
#include "Maps.hpp"

// ---
PacManII::PacMan::PacMan (int cId, const QGAMES::Forms& f, const QGAMES::Entity::Data& d)
	: PacManII::Artist (cId, f, d),
	  _alive (true),
	  _score (0),
	  _status (PacManII::PacMan::Status::_NOTDEFINED),
	  _lastStatus (PacManII::PacMan::Status::_NOTDEFINED),
	  _hasEaten (false),
	  _lastMulScoreNotified (0)
{
	// Adds a buoy to be able to "chase" out of the process event manipulation...
	assert (!buoys ().empty ()); // Just in case...
	buoys ().insert (QGAMES::Buoys::value_type
		(__PACMANII_TOCHASEBUOYID__, new PacManII::PacMan::ToChaseBuoy ()));
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
void PacManII::PacMan::toChaseDeferred (bool o)
{
	PacManII::PacMan::ToChaseBuoy* b = 
		dynamic_cast <PacManII::PacMan::ToChaseBuoy*> (buoy (__PACMANII_TOCHASEBUOYID__));
	assert (b != nullptr); // Just in case...

	b -> setChasing (o);

	b -> active (true); 
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
	assert (g != nullptr);
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
	assert (g != nullptr);
	g -> setFruitsEaten (_fruitsEaten);
}

// ---
void PacManII::PacMan::initialize ()
{
	PacManII::Artist::initialize ();

	setOrientation (QGAMES::Vector (__BD 1, __BD 0, __BD 0));
	toStand ();

	PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
	assert (g != nullptr);
	setScore (g -> score ());
	setFruitsEaten (g -> fruitsEaten ());

	_lastMulScoreNotified = _score / g -> dataGame ().everyToGetAnExtraLive ();
}

// ---
void PacManII::PacMan::updatePositions ()
{
	if (!isVisible ())
		return;

	PacManII::Artist::updatePositions ();

	if (doesPositionMatchesTile (position ()))
	{
		if (isStanding ())
		{
			if (_nextDirectionWhenPossible != QGAMES::Vector::_cero)
				toMove (_nextDirectionWhenPossible);
		}
	}
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
void PacManII::PacMan::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __PACMANII_LIMITMOVEMENTREACHED__)
		toStopDeferred ();

	PacManII::Artist::processEvent (evnt);
}

// ---
void PacManII::PacMan::whenCollisionWith (QGAMES::Entity* e)
{
	PacManII::PacmanElement* pE = dynamic_cast <PacManII::PacmanElement*> (e);
	if (e == nullptr)
		return;

	PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
	assert (g != nullptr);

	if (pE -> isAlive ())
	{
		if (isEnemy (pE))
		{
			PacManII::Monster* mter = dynamic_cast <PacManII::Monster*> (pE);
			if (mter != nullptr && 
				isNearOf (pE, __BD (visualLength () >> 1)))
			{
				if (mter -> isDangerous ())
				{
					if (!g -> passwordWellIntroduced ()) // Unlimited lives = password well introduced
						notify (QGAMES::Event (__PACMANII_PACMANDESTROYED__, this));
				}
				else
				{
					game () -> soundBuilder () -> sound (__PACMANII_SOUNDEATGHOST__) -> play (-1);

					// The points will be added in the scene obsrving pacman
					// This is because the number of points to add could depend on (e.g.) the number of eatn monsters.
					QGAMES::Event (__PACMANII_PACMANEATMONSTER__, mter);
				}
			}
		}
		else
		{
			PacManII::Fruit* frt = dynamic_cast <PacManII::Fruit*> (pE);
			if (frt != nullptr && 
				isNearOf (frt, __BD (visualLength () >> 1)))
			{
				setScore (score () + frt -> points ());

				addFruitEaten (frt -> type ());

				game () -> soundBuilder () -> sound (__PACMANII_SOUNDEATFRUIT__) -> play (-1);

				QGAMES::Event (__PACMANII_PACMANEATFRUIT__, frt);
			}
		}
		
	}
}

// ---
void PacManII::PacMan::setStatus (const PacManII::PacMan::Status& st)
{
	// In the case of pacman all changes among types of status are allowed
	// It is not the case in Monster...
	
	_lastStatus = st;
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

	QGAMES::Vector dir = direction (); // It is 0 when no moving...
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
	setMove (orientation ()); // ...but not moving... (although oriented to the last position)
	_pathInMaze = { }; // ..and with no path to follow
	changeDirectionWhenPossibleTo (QGAMES::Vector::_cero); // ...finally it stops!
}

// ---
void PacManII::PacMan::whatToDoWhenMovementStatusIsRequested (const QGAMES::Vector& d)
{
	setOrientation (d); // Look to..
	setStateToMoveTo (orientation ()); // ..with the right aspect...
	setMove (orientation ()); // ...and moving also towards there...
	recalculatePathInMaze (); // ...and with a new path
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
				toChaseDeferred (true);

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
void* PacManII::PacMan::ToChaseBuoy::treatFor (QGAMES::Element* e)
{
	PacManII::PacMan* pcm = dynamic_cast <PacManII::PacMan*> (e);
	assert (pcm != nullptr);

	pcm -> toChase (_chasing);

	return (this);
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
QGAMES::MazeModel::PathInMaze& PacManII::PacMan::recalculatePathInMaze (const QGAMES::Vector& mD)
{
	// The mandatory direction is not taken into account ever

	// Calculate whether the pacman is or not at the entry of monster's home
	// and if it is, what is the direction to enter, because it has to be avoided ever
	QGAMES::Vector dirMonstersHome = QGAMES::Vector::_noPoint; // By default it doesn't exit!
	if ((currentMazePosition () == pMap () -> monsterExitingHomePosition ()))
	{
		int sz = 0;
		if ((sz = (int) pMap () -> maze ().positionsForZone (__PACMANII_MAZEMONSTERSHOMEZONE__).size ()) != 0)
		{
			QGAMES::MazeModel::PathInMaze pE = pMap () -> maze ().next2StepsToGoTo 
				(currentMazePosition (), pMap () -> maze ().positionsForZone (__PACMANII_MAZEMONSTERSHOMEZONE__)[sz >> 1], { });
			if ((int) pE.size () > 1)
				dirMonstersHome = pE [1].asVector () - pE [0].asVector ();
		}
	}

	QGAMES::Vector dir = direction ();
	if (isMoving () && 
		_nextDirectionWhenPossible != QGAMES::Vector::_cero &&
		(dirMonstersHome == QGAMES::Vector::_noPoint || 
		 (dirMonstersHome != QGAMES::Vector::_noPoint && _nextDirectionWhenPossible != dirMonstersHome)) &&
		pMap () -> maze ().isPossibleToMoveTo (currentMazePosition (), _nextDirectionWhenPossible))
		dir = _nextDirectionWhenPossible;

	QGAMES::MazeModel::PathInMaze pth = pMap () -> maze ().limitPathFromFollowing (currentMazePosition (), dir);

	int i = 0;
	_pathInMaze = { currentMazePosition () };
	if (pth.size () > 1)
		for (QGAMES::MazeModel::PathInMaze::const_iterator j = ++pth.begin (); j != pth.end () && i++ < 5; j++)
			_pathInMaze.push_back ((*j)); // 5 positions in advance (if possible) for pacman...

	return (_pathInMaze);
}

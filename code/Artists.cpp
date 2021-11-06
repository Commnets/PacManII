#include "Artists.hpp"
#include "Game.hpp"
#include "Worlds.hpp"

// ---
QGAMES::SetOfOpenValues PacManII::Artist::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::ArtistInATiledMap::runtimeValues ();

	int lE = result.lastOpenValueId ();
	int lNE = result.lastNestedOpenValueId ();

	// TODO

	return (result);
}

// ---
void PacManII::Artist::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& oV)
{
	int lE = oV.lastOpenValueId ();
	int lNE = oV.lastNestedOpenValueId ();

	QGAMES::SetOfOpenValues cOv = oV;

	// TODO

	QGAMES::ArtistInATiledMap::initializeRuntimeValuesFrom (cOv);
}

// ---
QGAMES::MazeModel::PositionInMaze PacManII::Artist::currentMazePosition () const
{
	const PacManII::Map* mp = dynamic_cast <const PacManII::Map*> (map ());
	if (mp == nullptr)
		return QGAMES::MazeModel::_noPosition;

	return (mp -> mapPositionToMazePosition (position ()));
}

// ---
void PacManII::Artist::initialize ()
{
	QGAMES::ArtistInATiledMap::initialize ();

	_lastMazePosition = QGAMES::MazeModel::_noPosition;
}

// ---
void PacManII::Artist::updatePositions ()
{
	QGAMES::MazeModel::PositionInMaze lP = currentMazePosition ();

	// Here is where the artists moves...
	QGAMES::ArtistInATiledMap::updatePositions ();

	QGAMES::MazeModel::PositionInMaze cP = currentMazePosition ();
	if (cP != lP)
		_lastMazePosition = lP;
}

// ---
QGAMES::SetOfOpenValues PacManII::PacMan::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = PacManII::Artist::runtimeValues ();

	int lE = result.lastOpenValueId ();
	int lNE = result.lastNestedOpenValueId ();

	// TODO

	return (result);
}

// ---
void PacManII::PacMan::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& oV)
{
	int lE = oV.lastOpenValueId ();
	int lNE = oV.lastNestedOpenValueId ();

	QGAMES::SetOfOpenValues cOv = oV;

	// TODO

	PacManII::Artist::initializeRuntimeValuesFrom (cOv);
}

// ---
void PacManII::PacMan::initialize ()
{
	PacManII::Artist::initialize ();

	// TODO
	setCurrentState (__PACMANII_PACMANSTATESTANDLOOKINGRIGHT__); 
	_counter = 0;
	// TODO

	PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
	assert (g != nullptr);
	_score = g -> score ();
}

// ---
void PacManII::PacMan::updatePositions ()
{
	PacManII::Artist::updatePositions ();

	if (_counter++ == 400)
		notify (QGAMES::Event (__1IN2PROBABLE__ ? __PACMANII_PACMANREACHEDGOAL__ : __PACMANII_PACMANDESTROYED__, this));
//		notify (QGAMES::Event (__PACMANII_PACMANDESTROYED__, this));
//		notify (QGAMES::Event (__PACMANII_PACMANREACHEDGOAL__, this));

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
QGAMES::MazeModel::PositionInMaze PacManII::PacMan::targetMazePosition () const
{
	QGAMES::MazeModel::PositionInMaze result;

	return (result);
}

// ---
QGAMES::MazeModel::PositionInMaze PacManII::Monster::runAwayMazePosition () const
{
	const PacManII::Map* m = dynamic_cast <const PacManII::Map*> (map ());
	if (m == nullptr)
		return (QGAMES::MazeModel::_noPosition);

	return (m -> monsterRunAwayPosition (monsterNumber ()));
}

// ---
void PacManII::Monster::initialize ()
{
	PacManII::Artist::initialize ();

	setCurrentState (__PACMANII_INKYSTATECHASINGANDLOOKINGRIGHT__);

	// Initially...
	toChase (false);
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
void PacManII::StandardMonster::toPursuit (const PacManII::Artist* a)
{
	PacManII::Monster::toPursuit (a);

	if (_toPursuit != nullptr)
	{
		assert (dynamic_cast <const PacManII::PacMan*> (a) != nullptr);
	}
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

	if (chasing ())
	{
		if (_toPursuit != nullptr && _referenceArtists [0] != nullptr)
			result = QGAMES::MazeModel::PositionInMaze 
				(2 * (_toPursuit -> nextMazePosition (2).asVector () - _referenceArtists [0] -> currentMazePosition ().asVector ()));
	}
	else
		result = runAwayMazePosition ();

	return (result);
}

// ---
QGAMES::MazeModel::PositionInMaze PacManII::Blinky::targetMazePosition () const
{
	return (chasing () 
		? ((pursuit () != nullptr) ? pursuit () -> currentMazePosition () : QGAMES::MazeModel::_noPosition)
		: runAwayMazePosition ());
}

// ---
QGAMES::MazeModel::PositionInMaze PacManII::Pinky::targetMazePosition () const
{
	return (chasing () 
		? ((pursuit () != nullptr) ? pursuit () -> nextMazePosition (4) : QGAMES::MazeModel::_noPosition)
		: runAwayMazePosition ());
}

// ---
QGAMES::MazeModel::PositionInMaze PacManII::Clyde::targetMazePosition () const
{
	QGAMES::MazeModel::PositionInMaze result = QGAMES::MazeModel::_noPosition;

	if (chasing ())
	{
		if ((pursuit () -> currentMazePosition ().asVector () - currentMazePosition ().asVector ()).module () > __BD 8)
			result = pursuit () -> currentMazePosition ();
		else
			result = runAwayMazePosition ();
	}
	else
		result = runAwayMazePosition ();

	return (result);
}

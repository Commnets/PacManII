#include "Artists.hpp"
#include "Game.hpp"

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
	setCurrentState (0);
	_counter = 0;
	// TODO

	PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
	assert (g);
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
	assert (g);
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
void PacManII::PacMan::toMove (const QGAMES::Vector& d)
{
	// TODO
}

// ---
void PacManII::PacMan::toShoot (int f, const QGAMES::Vector& d)
{
	// TODO
}

// ---
void PacManII::Monster::initialize ()
{
	PacManII::Artist::initialize ();

	// TODO
	setCurrentState (0); 
	// TODO
}


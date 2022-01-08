#include "Artists.hpp"
#include "Movements.hpp"
#include "Game.hpp"
#include "Worlds.hpp"
#include "Scenes.hpp"
#include "Maps.hpp"

// ---
QGAMES::MazeModel::PositionInMaze PacManII::PacmanElement::nextXGridPosition (int x) const
{
	return ((pMap () == nullptr) 
		? QGAMES::MazeModel::_noPosition 
		: pMap () -> maze ().nextXGridPositionFollowing (currentMazePosition (), x, orientation (), true));
	// Notice that the orientation is taken into account 
	// and not the direction because the element could be already stopped
}

// ---
int PacManII::PacmanElement::currentMazeArea () const
{
	return ((pMap () == nullptr) ? -1 : pMap () -> maze ().mazeZoneAt (currentMazePosition ()));
}

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
PacManII::Artist::Artist (int cId, const QGAMES::Forms& f, const QGAMES::Entity::Data& d)
	: PacManII::PacmanElement (cId, f, d),
	  _lastMazePosition (QGAMES::MazeModel::_noPosition),
	  _pathInMaze (), // No defined by default...
	  _referenceArtists (),
	  _nextDirectionWhenPossible (QGAMES::Vector::_cero),
	  _referenceArtistLastMazePositions ()
{ 
	// Adds a buoy to be abble to "stop" out of the event manipulation process...
	assert (!buoys ().empty ()); // Just in case...
	buoys ().insert (QGAMES::Buoys::value_type
		(__PACMANII_TOSTOPBUOYID__, new PacManII::Artist::ToStopBuoy ()));
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
	if (!isVisible ())
		return;

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

	PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
	assert (g != nullptr);

	if (g -> drawArtistTarget ())
	{
		QGAMES::Position posI = mazePositionToMapPosition (currentMazePosition ());
		if (posI != QGAMES::Position::_noPoint)
		{
			QGAMES::Position posT = mazePositionToMapPosition (targetMazePosition ());
			for (int i = 0; i < 3; i++)
				scr -> drawCircle (posT, QGAMES::Vector::_zNormal, __BD (16 - i), __BD (16 - i), __QGAMES_REDCOLOR__, false);
			scr -> drawLine (posI, posT, __QGAMES_REDCOLOR__, 3);
		}
	}

#ifndef NDEBUG
	scr -> drawRectangle (collisionZone (), __QGAMES_YELLOWCOLOR__, false);
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
	
	PacManII::PacmanElement::processEvent (evnt);
}

// ---
void* PacManII::Artist::ToStopBuoy::treatFor (QGAMES::Element* e)
{
	PacManII::Artist* art = dynamic_cast <PacManII::Artist*> (e);
	assert (art != nullptr);

	art -> toStand ();

	return (this);
}

// ---
void PacManII::Artist::toStopDeferred ()
{
	PacManII::Artist::ToStopBuoy* b = 
		dynamic_cast <PacManII::Artist::ToStopBuoy*> (buoy (__PACMANII_TOSTOPBUOYID__));
	assert (b != nullptr); // Just in case...

	b -> active (true); 
}

// ---
QGAMES::MazeModel::PathInMaze& PacManII::Artist::recalculatePathInMaze (const QGAMES::Vector& mD)
{ 
	return (_pathInMaze = 
		pMap () -> maze ().next2StepsToGoTo (currentMazePosition (), targetMazePosition (), { })); 
}

// ---
bool PacManII::Artist::isArtistEnteringATunnel () const
{
	return ((pMap () == nullptr) 
		? false : pMap () -> maze ().isTunnelConnectionAt (currentMazePosition ()));
}

// ---
bool PacManII::Artist::hasReferenceArtistsPositionChanged () const
{
	bool result = false;

	for (std::vector <PacManII::Artist*>::const_iterator i = _referenceArtists.begin ();
			i != _referenceArtists.end () && !result; i++)
	{
		std::map <int, QGAMES::MazeModel::PositionInMaze>::const_iterator j = 
			_referenceArtistLastMazePositions.find ((*i) -> id ());
		if (j != _referenceArtistLastMazePositions.end () && (*i) -> currentMazePosition () != (*j).second)
			result = true;
	}

	return (result);
}

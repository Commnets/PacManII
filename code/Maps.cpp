#include "Maps.hpp"
#include "Game.hpp"

// ---
PacManII::Map::Map (int c, const QGAMES::Layers& l, int w, int h, int d, int tW, int tH, int tD, 
		const QGAMES::MapProperties& p)
	: QGAMES::TiledMap (c, l, w, h, d, tW, tH, tD, p),
	  _maze (PacManII::Maze::generateEmptyMaze (w, h)), // It will be assigned later, in a more complex formula...
	  _artistsLocationsLayer (nullptr),
	  _mazeLocationsLayer (nullptr),
	  _mazeZonesLayer (nullptr),
	  _directionsLayer (nullptr),
	  _mazeLayer (nullptr),
	  _backgroundLayer (nullptr),
	  _pacmanInitialPositions (), 
	  _monsterInitialPositions (), 
	  _monsterRunAwayPositions (),
	  _monsterExitingHomePosition (QGAMES::MazeModel::_noPosition),
	  _fruitPosition (QGAMES::MazeModel::_noPosition),
	  _mazeZones ()
{ 
	for (auto i : layers ())
	{
		assert (dynamic_cast <PacManII::ArtistsLocationsLayer*> (i) != nullptr || 
				dynamic_cast <PacManII::MazeLocationsLayer*> (i) != nullptr ||
				dynamic_cast <PacManII::MazeZonesLayer*> (i) != nullptr ||
				dynamic_cast <PacManII::DirectionsLayer*> (i) != nullptr || 
				dynamic_cast <PacManII::MazeLayer*> (i) != nullptr ||
				dynamic_cast <PacManII::BackgroundLayer*> (i) != nullptr);

		if (dynamic_cast <PacManII::ArtistsLocationsLayer*> (i) != nullptr)
		{
			assert (_artistsLocationsLayer == nullptr); // Only one...
			_artistsLocationsLayer = dynamic_cast <PacManII::ArtistsLocationsLayer*> (i);
		}
		else 
		if (dynamic_cast <PacManII::MazeLocationsLayer*> (i) != nullptr)
		{
			assert (_mazeLocationsLayer == nullptr); // Only one...
			_mazeLocationsLayer = dynamic_cast <PacManII::MazeLocationsLayer*> (i);
		}
		else 
		if (dynamic_cast <PacManII::MazeZonesLayer*> (i) != nullptr)
		{
			assert (_mazeZonesLayer == nullptr); // Only one...
			_mazeZonesLayer = dynamic_cast <PacManII::MazeZonesLayer*> (i);
		}
		else 
		if (dynamic_cast <PacManII::DirectionsLayer*> (i) != nullptr)
		{
			assert (_directionsLayer == nullptr); // Only one...
			_directionsLayer = dynamic_cast <PacManII::DirectionsLayer*> (i);
		}
		else 
		if (dynamic_cast <PacManII::MazeLayer*> (i) != nullptr)
		{
			assert (_mazeLayer == nullptr); // Only one...
			_mazeLayer = dynamic_cast <PacManII::MazeLayer*> (i);
		}
		else
		if (dynamic_cast <PacManII::BackgroundLayer*> (i) != nullptr)
		{
			assert (_backgroundLayer == nullptr); // Only one...
			_backgroundLayer = dynamic_cast <PacManII::BackgroundLayer*> (i);
		}
	}

	// ...and one of each...
	assert (_artistsLocationsLayer != nullptr && 
			_mazeLocationsLayer != nullptr &&
			_mazeZonesLayer != nullptr &&
			_directionsLayer != nullptr && 
			_mazeLayer != nullptr &&
			_backgroundLayer != nullptr); // There must be one of each minimum...

	// Generates the map...
	_maze = PacManII::Maze::generateMazeFrom (w, h, this);
}

// ---
QGAMES::SetOfOpenValues PacManII::Map::runtimeValues () const
{
	QGAMES::SetOfOpenValues result;

	// Layers has no status to keep in a pacman game...

	result.addOpenValue (0, QGAMES::OpenValue (ballsEatenStatus ()));

	return (result);
}

// ---
void PacManII::Map::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	assert (cfg.existOpenValue (0));

	setBallsEatenStatus (cfg.openValue (0).strValue ());
}

// ---
const QGAMES::MazeModel::PositionInMaze& PacManII::Map::pacmanInitialPosition (int nP) const
{
	assert (_artistsLocationsLayer != nullptr);

	std::map <int, QGAMES::MazeModel::PositionInMaze>::const_iterator pP;
	if ((pP = _pacmanInitialPositions.find (nP)) != _pacmanInitialPositions.end ())
		return ((*pP).second);

	return (_pacmanInitialPositions [nP] = mapPositionToMazePosition (_artistsLocationsLayer -> pacmanInitialPosition (nP)));
}

// ---
const QGAMES::MazeModel::PositionInMaze& PacManII::Map::monsterInitialPosition (int nM) const
{
	assert (_artistsLocationsLayer != nullptr);

	std::map <int, QGAMES::MazeModel::PositionInMaze>::const_iterator mP;
	if ((mP = _monsterInitialPositions.find (nM)) != _monsterInitialPositions.end ())
		return ((*mP).second);

	return (_monsterInitialPositions [nM] = mapPositionToMazePosition (_artistsLocationsLayer -> monsterInitialPosition (nM)));
}

// ---
const QGAMES::MazeModel::PositionInMaze& PacManII::Map::monsterRunAwayPosition (int nM) const
{
	assert (_artistsLocationsLayer != nullptr);

	std::map <int, QGAMES::MazeModel::PositionInMaze>::const_iterator mP;
	if ((mP = _monsterRunAwayPositions.find (nM)) != _monsterRunAwayPositions.end ())
		return ((*mP).second);

	return (_monsterRunAwayPositions [nM] = mapPositionToMazePosition (_artistsLocationsLayer -> monsterRunAwayPosition (nM)));
}

// ---
const QGAMES::MazeModel::PositionInMaze& PacManII::Map::monsterExitingHomePosition () const
{
	assert (_mazeLocationsLayer != nullptr);

	if (_monsterExitingHomePosition != QGAMES::MazeModel::_noPosition)
		return (_monsterExitingHomePosition);

	return (_monsterExitingHomePosition = mapPositionToMazePosition (_mazeLocationsLayer -> monsterExitingHomePosition ()));
}

// ---
const QGAMES::MazeModel::PositionInMaze& PacManII::Map::fruitPosition () const
{
	assert (_artistsLocationsLayer != nullptr);

	if (_fruitPosition != QGAMES::MazeModel::_noPosition)
		return (_fruitPosition);

	return (_fruitPosition = mapPositionToMazePosition (_artistsLocationsLayer -> fruitPosition ()));
}

// ---
void PacManII::Map::startBlinking (QGAMES::bdata bT, int nB)
{ 
	_mazeLayer -> startBlinking (bT, nB); 
}

// ---
bool PacManII::Map::isBlinking () const
{ 
	return (_mazeLayer -> isBlinking ()); 
}

// ---
void PacManII::Map::stopBlinking ()
{ 
	_mazeLayer -> stopBlinking (); 
}

// ---
int PacManII::Map::maxNumberBallsToEat () const
{ 
	return (_mazeLayer -> maxNumberBallsToEat ()); 
}

// ---
int PacManII::Map::numberBallsEaten () const
{ 
	return (_mazeLayer -> numberBallsEaten ()); 
}

// ---
std::string PacManII::Map::ballsEatenStatus () const
{
	return (_mazeLayer -> ballsEatenStatus ()); 
}

// ---
void PacManII::Map::setBallsEatenStatus (const std::string& st)
{
	_mazeLayer -> setBallsEatenStatus (st);
}

// ---
void PacManII::Map::initialize ()
{
	_mazeLayer -> setBallsEatenStatus (std::string (__NULL_STRING__)); // All points by default...
}

// ---
const std::map <int, QGAMES::Position>& PacManII::ArtistsLocationsLayer::pacmanInitialPositions () const
{
	if (!_pacmanInitialPositions.empty ())
		return (_pacmanInitialPositions);

	const PacManII::Game* g = dynamic_cast <const PacManII::Game*> (game ());
	assert (g != nullptr);
	const PacManII::TMXMapBuilder* mB = g -> tmxAddsOnMapBuilder ();
	assert (mB != nullptr);

	if (mB -> pacmanHomeFrames ().empty ())
		return (_pacmanInitialPositions);

	_pacmanInitialPositions = { };
	for (auto i : tiles ())
	{
		if (dynamic_cast <QGAMES::NullTile*> (i) == nullptr)
		{
			std::vector <int>::const_iterator j = 
				std::find (mB -> pacmanHomeFrames ().begin (), mB -> pacmanHomeFrames ().end (), i -> numberFrame ());
			if (j != mB -> pacmanHomeFrames ().end ())
				_pacmanInitialPositions [((*j) - mB -> pacmanHomeFrames () [0])] = tilePosition (i);
		}
	}

	return (_pacmanInitialPositions);
}

// ---
const QGAMES::Position& PacManII::ArtistsLocationsLayer::pacmanInitialPosition (int nP) const
{ 
	std::map <int, QGAMES::Position>::const_iterator i = pacmanInitialPositions ().find (nP);
	return ((i != pacmanInitialPositions ().end ()) ? (*i).second : QGAMES::Position::_noPoint);
}

// ---
const std::map <int, QGAMES::Position>& PacManII::ArtistsLocationsLayer::monsterInitialPositions () const
{
	if (!_monsterInitialPositions.empty ())
		return (_monsterInitialPositions);

	const PacManII::Game* g = dynamic_cast <const PacManII::Game*> (game ());
	assert (g != nullptr);
	const PacManII::TMXMapBuilder* mB = g -> tmxAddsOnMapBuilder ();
	assert (mB != nullptr);

	if (mB -> monsterHomeFrames ().empty ())
		return (_monsterInitialPositions);

	_monsterInitialPositions = { };
	for (auto i : tiles ())
	{
		if (dynamic_cast <QGAMES::NullTile*> (i) == nullptr)
		{
			std::vector <int>::const_iterator j = 
				std::find (mB -> monsterHomeFrames ().begin (), mB -> monsterHomeFrames ().end (), i -> numberFrame ());
			if (j != mB -> monsterHomeFrames ().end ())
				_monsterInitialPositions [((*j) - mB -> monsterHomeFrames () [0])] = tilePosition (i);
		}
	}

	return (_monsterInitialPositions);
}

// ---
const QGAMES::Position& PacManII::ArtistsLocationsLayer::monsterInitialPosition (int nM) const
{ 
	std::map <int, QGAMES::Position>::const_iterator i = monsterInitialPositions ().find (nM);
	return ((i != monsterInitialPositions ().end ()) ? (*i).second : QGAMES::Position::_noPoint);
}

// ---
const std::map <int, QGAMES::Position>& PacManII::ArtistsLocationsLayer::monsterRunAwayPositions () const
{
	if (!_monsterRunAwayPositions.empty ())
		return (_monsterRunAwayPositions);

	const PacManII::Game* g = dynamic_cast <const PacManII::Game*> (game ());
	assert (g != nullptr);
	const PacManII::TMXMapBuilder* mB = g -> tmxAddsOnMapBuilder ();
	assert (mB != nullptr);

	if (mB -> monsterRunAwayFrames ().empty ())
		return (_monsterRunAwayPositions);

	_monsterRunAwayPositions = { };
	for (auto i : tiles ())
	{
		if (dynamic_cast <QGAMES::NullTile*> (i) == nullptr)
		{
			std::vector <int>::const_iterator j = 
				std::find (mB -> monsterRunAwayFrames ().begin (), mB -> monsterRunAwayFrames ().end (), i -> numberFrame ());
			if (j != mB -> monsterRunAwayFrames ().end ())
				_monsterRunAwayPositions [((*j) - mB -> monsterRunAwayFrames () [0])] = tilePosition (i);
		}
	}

	return (_monsterRunAwayPositions);
}

// ---
const QGAMES::Position& PacManII::ArtistsLocationsLayer::monsterRunAwayPosition (int nM) const
{ 
	std::map <int, QGAMES::Position>::const_iterator i = monsterRunAwayPositions ().find (nM);
	return ((i != monsterRunAwayPositions ().end ()) ? (*i).second : QGAMES::Position::_noPoint);
}

// ---
const QGAMES::Position& PacManII::ArtistsLocationsLayer::fruitPosition () const
{
	if (_fruitPosition != QGAMES::Position::_noPoint)
		return (_fruitPosition);

	const PacManII::Game* g = dynamic_cast <const PacManII::Game*> (game ());
	assert (g != nullptr);
	const PacManII::TMXMapBuilder* mB = g -> tmxAddsOnMapBuilder ();
	assert (mB != nullptr);

	for (QGAMES::Tiles::const_iterator i = tiles ().begin (); 
		i != tiles ().end () && _fruitPosition == QGAMES::Position::_noPoint; i++)
		if ((*i) -> numberFrame () == mB -> fruitPositionFrame ())
			_fruitPosition = tilePosition ((*i));

	return (_fruitPosition);
}

// ---
const QGAMES::Position& PacManII::MazeLocationsLayer::monsterExitingHomePosition () const
{
	if (_monsterExitingHomePosition != QGAMES::Position::_noPoint)
		return (_monsterExitingHomePosition);

	const PacManII::Game* g = dynamic_cast <const PacManII::Game*> (game ());
	assert (g != nullptr);
	const PacManII::TMXMapBuilder* mB = g -> tmxAddsOnMapBuilder ();
	assert (mB != nullptr);

	for (QGAMES::Tiles::const_iterator i = tiles ().begin (); 
		i != tiles ().end () && _monsterExitingHomePosition == QGAMES::Position::_noPoint; i++)
		if ((*i) -> numberFrame () == mB -> monsterExitingHomeFrame ())
			_monsterExitingHomePosition = tilePosition ((*i));

	return (_monsterExitingHomePosition);
}

// ---
PacManII::MazeLayer::MazeLayer (int c, const std::string& n, const QGAMES::Tiles& t, QGAMES::Map* m, const QGAMES::LayerProperties& p)
	: QGAMES::TileLayer (c, n, t, m, QGAMES::TileLayer::_ORTHOGONAL, p, false)
{
#ifndef NDEBUG
	for (auto i : tiles ())
		assert (dynamic_cast <QGAMES::NullTile*> (i) != nullptr ||
				dynamic_cast <PacManII::TileLimit*> (i) != nullptr ||
				dynamic_cast <PacManII::TilePath*> (i) != nullptr);
#endif

	// Only the tiles type path are observed if they can be eaten...
	for (auto i : tiles ())
	{
		PacManII::TilePath* tP = dynamic_cast <PacManII::TilePath*> (i);
		if (tP != nullptr && tP -> canBeEaten ())
			observe (i); // The observation stops when the layer is destroyed, if any...
	}
}

// ---
void PacManII::MazeLayer::BlinkingMaze::initialize ()
{
	_counterBlinkingTime.initialize ();
	_counterNumberBlinks.initialize ();
	_switchBlinkOn.initialize ();

	_active = true;

	// All must bright...
	for (auto i : (dynamic_cast <PacManII::MazeLayer*> (_layer)) -> tiles ())
	{
		PacManII::TileLimit* tL = nullptr;
		if ((tL = dynamic_cast <PacManII::TileLimit*> (i)) != nullptr)
			tL -> setBright (true);
	}

	_switchBlinkOn.set (true);
}

// ---
void PacManII::MazeLayer::BlinkingMaze::updatePositions ()
{
	if (!_active)
		return;

	if (_counterBlinkingTime.isEnd ())
	{
		_switchBlinkOn.set (!_switchBlinkOn.isOn ());

		for (auto i : (dynamic_cast <TileLayer*> (_layer)) -> tiles ())
		{
			PacManII::TileLimit* tL = nullptr;
			if ((tL = dynamic_cast <PacManII::TileLimit*> (i)) != nullptr)
				tL -> setBright (_switchBlinkOn.isOn ());
		}

		if (!_switchBlinkOn.isOn () && _counterNumberBlinks.isEnd ())
			_active = false; // No more blinks...
	}
}

// ---
void PacManII::MazeLayer::BlinkingMaze::finalize ()
{
	_switchBlinkOn.set (false);

	for (auto i : (dynamic_cast <TileLayer*> (_layer)) -> tiles ())
	{
		PacManII::TileLimit* tL = nullptr;
		if ((tL = dynamic_cast <PacManII::TileLimit*> (i)) != nullptr)
			tL -> setBright (false);
	}

	_active = false; // No more blinks...
}

// ---
void PacManII::MazeLayer::startBlinking (QGAMES::bdata bT, int nB)
{
	setVisualEffect (new PacManII::MazeLayer::BlinkingMaze (this, bT, nB));
}

// ---
bool PacManII::MazeLayer::isBlinking () const
{
	return ((visualEffect () != nullptr) 
		? (dynamic_cast <const PacManII::MazeLayer::BlinkingMaze*> (visualEffect ())) -> isActive () : false);  
}

// ---
void PacManII::MazeLayer::stopBlinking ()
{
	setVisualEffect (nullptr);
}

// ---
int PacManII::MazeLayer::maxNumberBallsToEat () const
{
	int result = 0;
	for (auto i : tiles ())
	{
		const PacManII::TilePath* tl = dynamic_cast <const PacManII::TilePath*> (i); 
		if (tl != nullptr && tl -> canBeEaten ())
			result++;
	}

	return (result);
}

// ---
int PacManII::MazeLayer::numberBallsEaten () const
{
	int result = 0;
	for (auto i : tiles ())
	{
		const PacManII::TilePath* tl = dynamic_cast <const PacManII::TilePath*> (i); 
		if (tl != nullptr && tl -> alreadyEaten ())
			result++;
	}

	return (result);
}

// ---
std::string PacManII::MazeLayer::ballsEatenStatus () const
{
	std::string result (__NULL_STRING__);

	for (auto i : tiles ())
	{
		const PacManII::TilePath* tl = dynamic_cast <const PacManII::TilePath*> (i);
		result += (tl == nullptr) ? std::string ("_") : std::to_string ((tl -> alreadyEaten () ? 0 : 1));
	}

	return (result);
}

// ---
void PacManII::MazeLayer::setBallsEatenStatus (const std::string& st)
{
	if (st == std::string (__NULL_STRING__))
	{
		for (auto i : tiles ())
		{
			PacManII::TilePath* tP = dynamic_cast <PacManII::TilePath*> (i);
			if (tP != nullptr)
				tP -> eaten (false);
		}
	}
	else
	{
		int j = 0;
		for (auto i : tiles ())
		{
			if (st [j] == '_')
			{
				// Just to be sure that the status requiered represents the map...
				assert (dynamic_cast <PacManII::TilePath*> (i) == nullptr);
			}
			else
			{
				PacManII::TilePath* tP = dynamic_cast <PacManII::TilePath*> (i);
				assert (tP != nullptr);
				tP -> eaten (st [j] == '1' ? false : true);
			}

			j++;
		}
	}
}

// ---
void PacManII::TileLimit::setBright (bool b)
{
	PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
	assert (g != nullptr); // It can only be made within a PacManII game, that it should be the case...
	PacManII::TMXMapBuilder* mB = g -> tmxAddsOnMapBuilder ();
	assert (mB != nullptr); // Just in case...
	// Reley in the builder to determine the equivalent frame...

	setForm (form (), 
		(b) ? mB -> brightFrameFor (numberFrame ()) : mB -> darkFrameFor (numberFrame ()));
}

// ---
bool PacManII::TilePath::eaten (bool s)
{ 
	bool result = false;

	if (canBeEaten ())
	{
		PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
		assert (g != nullptr); // It can only be made within a PacManII game, that it should be the case...
		PacManII::TMXMapBuilder* mB = g -> tmxAddsOnMapBuilder ();
		assert (mB != nullptr); // Just in case...

		if (s && _type != PacManII::TilePath::Type::_EMPTY)
		{
			_type = PacManII::TilePath::Type::_EMPTY; // No more

			setForm (form (), mB -> frameForEmptyPath ());

			notify (QGAMES::Event (__PACMANII_BALLEATEN__, this));
		}
		else
		{
			_type = _originalType;

			setForm (form (), _originalFrame);
		}

		result = true;
	}

	return (result);
}

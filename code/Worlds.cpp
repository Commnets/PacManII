#include "Worlds.hpp"
#include "Game.hpp"

// ---
PacManII::World::World (int c, const QGAMES::Scenes& s, const QGAMES::WorldProperties& p)
	: QGAMES::World (c, s, p)
{
#ifndef NDEBUG
	for (auto i : scenes ())
		assert (dynamic_cast <PacManII::Scene*> (i.second) != nullptr);
#endif
}

// ---		
QGAMES::SetOfOpenValues PacManII::World::runtimeValues () const
{ 
	const PacManII::Game* dG = dynamic_cast <const PacManII::Game*> (game ());
	assert (dG); // Just in case...

	// Remember that world's runtime values are kept per player
	// Remember also that the runtime info of the characters and entities of a scene are not saved
	// But blocks do, and dragonwind blocks save also in many ocassions entities' info
	// So, e.g, if playing in the scene 2 when the runtime values of the blocks of the scene 1 are got
	// The info of the characters will be the one of the scene 2. This is not consistent
	QGAMES::SetOfOpenValues pOV = currentPlayerRuntimeValues ();

	// If there is no active scene then the info is simply returned
	if (activeScene () == NULL)
	{
		_fullConfPerPlayer.addSetOfOpenValues (dG -> currentPlayer (), pOV);
		return (_fullConfPerPlayer);
	}

	// Get the scenes info of the current runtime values...
	// Only the one for the active scene will be usefull...
	int lNE1 = pOV.lastNestedOpenValueId (); // It is supossed it is the scenes info...
	QGAMES::SetOfOpenValues pyScnOv1 = pOV.setOfOpenValues (lNE1);
	assert (pyScnOv1.name () == std::string (__QGAMES_RUNTIMEVALUESSCENESTYPE__)); // ...but double check just in case...
	
	// If it is the first time the player enters the world, there is nothing else to do...
	// Except removing the info not related with this specific scene!
	if (!_fullConfPerPlayer.existSetOfOpenValues (dG -> currentPlayer ()))
	{
		for (QGAMES::Scenes::const_iterator i = scenes ().begin (); i != scenes ().end (); i++)
		{
			if ((*i).first != activeScene () -> id () &&
				pyScnOv1.existSetOfOpenValues ((*i).first))
				pyScnOv1.removeSetOfOpenValues ((*i).first);
		}
	}

	// Otherwise the info of the previous visits have to be recovered!
	// From the info already saved in _fullConfPlayer!
	else
	{
		QGAMES::SetOfOpenValues pyWOv = _fullConfPerPlayer.setOfOpenValues (dG -> currentPlayer ());
		int lNE2 = pyWOv.lastNestedOpenValueId (); // It is supossed it is the scenes info...
		QGAMES::SetOfOpenValues pyScnOv2 = pyWOv.setOfOpenValues (lNE2);
		assert (pyScnOv2.name () == std::string (__QGAMES_RUNTIMEVALUESSCENESTYPE__)); // ...but double check just in case...

		// The older scenes' info is recovered...
		// ..and the information of the current scene is maintained!
		for (QGAMES::Scenes::const_iterator i = scenes ().begin (); i != scenes ().end (); i++)
		{
			if ((*i).first != activeScene () -> id ())
			{
				if (pyScnOv2.existSetOfOpenValues ((*i).first)) 
					pyScnOv1.addSetOfOpenValues ((*i).first, pyScnOv2.setOfOpenValues ((*i).first)); // Replace...
				else
				if (pyScnOv1.existSetOfOpenValues ((*i).first))
					pyScnOv1.removeSetOfOpenValues ((*i).first); // It is not necessary...
			}
		}
	}

	pOV.addSetOfOpenValues (lNE1, pyScnOv1);

	// Finally creates the info per player, and returns it...
	_fullConfPerPlayer.addSetOfOpenValues (dG -> currentPlayer (), pOV); // Replace the previous one...
	return (_fullConfPerPlayer);
}

// ---
void PacManII::World::startBlinking (QGAMES::bdata bT, int nB)
{
	// Only ober the active scene if any...
	PacManII::Scene* scn = nullptr;
	if ((scn = dynamic_cast <PacManII::Scene*> (activeScene ())) != nullptr)
		scn -> startBlinking (bT, nB);
}

// ---
bool PacManII::World::isBlinking () const
{
	// Only over the active scene if any...
	const PacManII::Scene* scn = dynamic_cast <const PacManII::Scene*> (activeScene ());
	return ((scn != nullptr) ? scn -> isBlinking () : false);
}

// ---
void PacManII::World::stopBlinking ()
{
	// Only over the active scene if any...
	PacManII::Scene* scn = nullptr;
	if ((scn = dynamic_cast <PacManII::Scene*> (activeScene ())) != nullptr)
		scn -> stopBlinking ();
}

// ---
int PacManII::World::maxNumberBallsToEat () const
{
	// Only over the active scene if any...
	const PacManII::Scene* scn = dynamic_cast <const PacManII::Scene*> (activeScene ());
	return ((scn != nullptr) ? scn -> maxNumberBallsToEat () : 0);
}

// ---
int PacManII::World::numberBallsEaten () const
{
	// Only over the active scene if any...
	const PacManII::Scene* scn = dynamic_cast <const PacManII::Scene*> (activeScene ());
	return ((scn != nullptr) ? scn -> numberBallsEaten () : 0);
}

// ---
std::string PacManII::World::ballsEatenStatus () const
{
	// Only over the active scene if any...
	const PacManII::Scene* scn = dynamic_cast <const PacManII::Scene*> (activeScene ());
	return ((scn != nullptr) ? scn -> ballsEatenStatus () : std::string (__NULL_STRING__));
}

// ---
void PacManII::World::setBallsEatenStatus (const std::string& st)
{
	// Only over the active scene if any...
	PacManII::Scene* scn = nullptr;
	if ((scn = dynamic_cast <PacManII::Scene*> (activeScene ())) != nullptr)
		scn -> setBallsEatenStatus (st);
}

// ---
void PacManII::World::playSiren (bool f)
{
	// Only over the active scene if any...
	PacManII::Scene* scn = nullptr;
	if ((scn = dynamic_cast <PacManII::Scene*> (activeScene ())) != nullptr)
		scn -> playSiren (f);
}

// ---
void PacManII::World::stopSiren ()
{
	// Only over the active scene if any...
	PacManII::Scene* scn = nullptr;
	if ((scn = dynamic_cast <PacManII::Scene*> (activeScene ())) != nullptr)
		scn -> stopSiren ();
}

// ---
void PacManII::World::initialize ()
{
	QGAMES::World::initialize ();

	PacManII::Game* dG = dynamic_cast <PacManII::Game*> (game ());
	assert (dG);
	PacManII::Game::Conf* cfg = dynamic_cast <PacManII::Game::Conf*> (dG -> configuration ());
	assert (cfg); // Just in case...

	// Gets back the information kept for that world, if it exists
	// Otherwise an empty element is set up...
	// The world keeps different information per player (id)
	_fullConfPerPlayer = QGAMES::SetOfOpenValues (std::string (__QGAMES_RUNTIMEVALUESELMNTTYPE__));
	if (cfg -> existConfValueForElement (id ()))
		_fullConfPerPlayer = cfg -> confValueForElement (id ());
	
	// Gets back the specific information of the world for the current player, if it exists
	// That information won't exist if the player hasn't ever played in this world!
	// So, in that situation there won't be anything to configure beyond the work
	// already done at initialization time...
	if (_fullConfPerPlayer.existSetOfOpenValues (dG -> currentPlayer ()))
	{
		// Remember that the characters and entities used in the scene are reused in other and with other players
		// On the other hand, the world is initialized any time a scene starts back
		// So the information kept must be used to initialize just the current scene forgetting the rest...
		QGAMES::SetOfOpenValues pyWOv = _fullConfPerPlayer.setOfOpenValues (dG -> currentPlayer ());
		int lNE = pyWOv.lastNestedOpenValueId (); // It is supossed it is the scenes info...
		QGAMES::SetOfOpenValues pyScnOv = pyWOv.setOfOpenValues (lNE);
		assert (pyScnOv.name () == std::string (__QGAMES_RUNTIMEVALUESSCENESTYPE__)); // ...but double check just in case...

		for (QGAMES::Scenes::const_iterator i = scenes ().begin (); i != scenes ().end (); i++)
			if ((*i).first != activeScene () -> id () && pyScnOv.existSetOfOpenValues ((*i).first))
				pyScnOv.removeSetOfOpenValues ((*i).first); 

		pyWOv.addSetOfOpenValues (lNE, pyScnOv);
		
		// The initialization then takes place...
		QGAMES::World::initializeRuntimeValuesFrom (pyWOv); 
	}
}

// ---
void PacManII::World::finalize ()
{
	// The current status of the game is stored...
	runtimeValues ();

	// ...and put it back into the configuration object...
	PacManII::Game::Conf* cfg = dynamic_cast <PacManII::Game::Conf*> (game () -> configuration ());
	assert (cfg); // Just in case...
	cfg -> setConfValueForElement (id (), _fullConfPerPlayer);

	QGAMES::World::finalize ();
}

// ---
PacManII::Scene::Scene (int c, const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn, 
		const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
	: QGAMES::Scene (c, m, cn, p, ePL),
	  _pacman (nullptr),
	  _inky (nullptr), _blinky (nullptr), _pinky (nullptr), _clyde (nullptr),
	  _percentageCleaned (__BD 0)
{
#ifndef NDEBUG
	for (auto i : maps ())
		assert (dynamic_cast <PacManII::Map*> (i.second) != nullptr);
#endif
}

// ---
void PacManII::Scene::startBlinking (QGAMES::bdata bT, int nB)
{
	// Only over the actve map if any...
	PacManII::Map* mp = nullptr;
	if ((mp = dynamic_cast <PacManII::Map*> (activeMap ())) != nullptr)
		mp -> startBlinking (bT, nB);
}

// ---
bool PacManII::Scene::isBlinking () const
{
	// Only over the actve map if any...
	const PacManII::Map* mp = dynamic_cast <const PacManII::Map*> (activeMap ());
	return ((mp != nullptr) ? mp -> isBlinking () : false);
}

// ---
void PacManII::Scene::stopBlinking ()
{
	// Only over the actve map if any...
	PacManII::Map* mp = nullptr;
	if ((mp = dynamic_cast <PacManII::Map*> (activeMap ())) != nullptr)
		mp -> stopBlinking ();
}

// ---
int PacManII::Scene::maxNumberBallsToEat () const
{
	// Only over the actve map if any...
	const PacManII::Map* mp = dynamic_cast <const PacManII::Map*> (activeMap ());
	return ((mp != nullptr) ? mp -> maxNumberBallsToEat () : 0);
}

// ---
int PacManII::Scene::numberBallsEaten () const
{
	// Only over the actve map if any...
	const PacManII::Map* mp = dynamic_cast <const PacManII::Map*> (activeMap ());
	return ((mp != nullptr) ? mp -> numberBallsEaten () : 0);
}

// ---
std::string PacManII::Scene::ballsEatenStatus () const
{
	// Only over the actve map if any...
	const PacManII::Map* mp = dynamic_cast <const PacManII::Map*> (activeMap ());
	return ((mp != nullptr) ? mp -> ballsEatenStatus () : std::string (__NULL_STRING__));
}

// ---
void PacManII::Scene::setBallsEatenStatus (const std::string& st)
{
	// Only over the actve map if any...
	PacManII::Map* mp = nullptr;
	if ((mp = dynamic_cast <PacManII::Map*> (activeMap ())) != nullptr)
		mp -> setBallsEatenStatus (st);
}

// ---
void PacManII::Scene::playSiren (bool f)
{ 
	SirenRate sR = SirenRate::_NORMAL;
	if (_percentageCleaned > 30 && _percentageCleaned < 60) sR = SirenRate::_FAST;
	if (_percentageCleaned > 60) sR = SirenRate::_VERYFAST;
	if (sR != _sirenRate || f)
	{
		_sirenRate = sR;

		game () -> sound (__PACMANII_SOUNDSIREN__ + static_cast <int> (sR)) -> play (__QGAMES_BACKGROUNDSOUNDCHANNEL__); 
	}
}

// ---
void PacManII::Scene::stopSiren ()
{
	QGAMES::SoundSystem::system () -> pauseChannel (__QGAMES_BACKGROUNDSOUNDCHANNEL__);
}

// ---
void PacManII::Scene::initialize ()
{
	// This is the common scene and it is made of 4 characters
	// In other scenes more evolutionated, the scene can be made of many more monsters

	// Load and add the characters to the scene...
	addCharacter (_pacman = dynamic_cast <PacManII::PacMan*> (game () -> character (__PACMANII_PACMANBASEENTITYID__)));
	addCharacter (_inky = dynamic_cast <PacManII::Inky*> (game () -> character (__PACMANII_INKYBASEENTITYID__)));
	addCharacter (_blinky = dynamic_cast <PacManII::Blinky*> (game () -> character (__PACMANII_BLINKYBASEENTITYID__)));
	addCharacter (_pinky = dynamic_cast <PacManII::Pinky*> (game () -> character (__PACMANII_PINKYBASEENTITYID__)));
	addCharacter (_clyde = dynamic_cast <PacManII::Clyde*> (game () -> character (__PACMANII_CLYDEBASEENTITYID__)));
	assert (_pacman != nullptr && 
			_inky != nullptr && _blinky != nullptr && _pinky != nullptr && _clyde != nullptr); // It shouldn't but just in case...

	// The scene has to be initialized just after characters are added...
	QGAMES::Scene::initialize ();

	// Set the initial state for each
	_pacman -> setCurrentState (__PACMANII_PACMANSTATESTANDLOOKINGRIGHT__);
	_inky -> setCurrentState (__PACMANII_INKYSTATEATHOMELOOKINGUP__);
	_blinky -> setCurrentState (__PACMANII_BLINKYSTATEATHOMELOOKINGDOWN__);
	_pinky -> setCurrentState (__PACMANII_PINKYSTATEATHOMELOOKINGUP__);
	_clyde -> setCurrentState (__PACMANII_CLYDESTATEATHOMELOOKINGDOWN__);

	// Set the characters in the right position of the map...
	PacManII::Map* aM = dynamic_cast <PacManII::Map*> (activeMap ());
	assert (aM);
	_pacman -> setPosition (aM -> pacmanInitialPosition () - 
		QGAMES::Vector (__BD (_pacman -> visualLength () >> 1), __BD (_pacman -> visualHeight () >> 1), __BD 0));
	_inky -> setPosition (aM -> monsterInitialPosition (0) - 
		QGAMES::Vector (__BD (_inky -> visualLength () >> 1), __BD (_inky -> visualHeight () >> 1), __BD 0));
	_blinky -> setPosition (aM -> monsterInitialPosition (1) - 
		QGAMES::Vector (__BD (_blinky -> visualLength () >> 1), __BD (_blinky -> visualHeight () >> 1), __BD 0));
	_pinky -> setPosition (aM -> monsterInitialPosition (2) - 
		QGAMES::Vector (__BD (_pinky -> visualLength () >> 1), __BD (_pinky -> visualHeight () >> 1), __BD 0));
	_clyde -> setPosition (aM -> monsterInitialPosition (3) - 
		QGAMES::Vector (__BD (_clyde -> visualLength () >> 1), __BD (_clyde -> visualHeight () >> 1), __BD 0));

	_percentageCleaned = __BD numberBallsEaten () / __BD maxNumberBallsToEat ();
}

// ---
void PacManII::Scene::finalize ()
{
	QGAMES::Scene::finalize ();

	removeCharacter (game () -> character (__PACMANII_PACMANBASEENTITYID__));
	removeCharacter (game () -> character (__PACMANII_INKYBASEENTITYID__));
	removeCharacter (game () -> character (__PACMANII_BLINKYBASEENTITYID__));
	removeCharacter (game () -> character (__PACMANII_PINKYBASEENTITYID__));
	removeCharacter (game () -> character (__PACMANII_CLYDEBASEENTITYID__));

	_pacman = nullptr;
	_inky = nullptr; 
	_blinky = nullptr;
	_pinky = nullptr;
	_clyde = nullptr;
}

// ---
void PacManII::Scene::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __PACMANII_BALLEATEN__)
	{
		game () -> sound (__PACMANII_SOUNDCHOMP__) -> play (-1); // Chomp...

		_percentageCleaned = __BD numberBallsEaten () / __BD maxNumberBallsToEat ();
	}
	else
		notify (QGAMES::Event (evnt.code (), this, evnt.values ()));
}

// ---
PacManII::Map::Map (int c, const QGAMES::Layers& l, int w, int h, int d, int tW, int tH, int tD, const QGAMES::MapProperties& p)
	: QGAMES::TiledMap (c, l, w, h, d, tW, tH, tD, p)
{ 
#ifndef NDEBUG
	for (auto i : layers ())
		assert (dynamic_cast <PacManII::TileLayer*> (i) != nullptr);
#endif
}

// ---
void PacManII::Map::startBlinking (QGAMES::bdata bT, int nB)
{
	for (auto i : layers ())
	{
		PacManII::TileLayer* tL = nullptr;
		if ((tL = dynamic_cast <PacManII::TileLayer*> (i)) != nullptr)
			tL -> startBlinking (bT, nB);
	}
}

// ---	
bool PacManII::Map::isBlinking () const
{
	bool result = false;
	for (auto i : layers ())
	{
		const PacManII::TileLayer* tL = dynamic_cast <const PacManII::TileLayer*> (i);
		result |= (tL != nullptr) ? tL -> isBlinking () : false;
	}

	return (result);
}

// ---
void PacManII::Map::stopBlinking ()
{
	for (auto i : layers ())
	{
		PacManII::TileLayer* tL = nullptr;
		if ((tL = dynamic_cast <PacManII::TileLayer*> (i)) != nullptr)
			tL -> stopBlinking ();
	}
}

// ---
int PacManII::Map::maxNumberBallsToEat () const
{
	int result = 0;
	for (auto i : layers ())
	{
		const PacManII::TileLayer* tL = dynamic_cast <const PacManII::TileLayer*> (i);
		result += (tL != nullptr) ? tL -> maxNumberBallsToEat () : 0;
	}

	return (result);
}

// ---
int PacManII::Map::numberBallsEaten () const
{
	int result = 0;
	for (auto i : layers ())
	{
		const PacManII::TileLayer* tL = dynamic_cast <const PacManII::TileLayer*> (i);
		result += (tL != nullptr) ? tL -> numberBallsEaten () : 0;
	}

	return (result);

}

// ---
std::string PacManII::Map::ballsEatenStatus () const
{
	std::string result (__NULL_STRING__);
	for (auto i : layers ())
	{
		const PacManII::TileLayer* tL = dynamic_cast <const PacManII::TileLayer*> (i);
		result += ((tL != nullptr) ? tL -> ballsEatenStatus () : std::string (__NULL_STRING__)) + std::string ("@"); // @ to separate
	}

	return (result);
}

// ---
void PacManII::Map::setBallsEatenStatus (const std::string& st)
{
	if (st == std::string (__NULL_STRING__))
		return;

	std::string stC = st;
	for (auto i : layers ())
	{
		PacManII::TileLayer* tL = dynamic_cast <PacManII::TileLayer*> (i);
		assert (st.find ('@') != -1); // One per tile layer...mandatory
		stC = stC.substr (0, stC.find ('@', 0));
		if (tL != nullptr)
			tL -> setBallsEatenStatus (stC);
	}
}

// ---
QGAMES::Position PacManII::StandardMap::pacmanInitialPosition () const
{
	return (QGAMES::Position (__BD ((10 * tileWidth ()) + (tileWidth () >> 1)), 
							  __BD ((12 * tileHeight ()) + (tileHeight () >> 1)), __BD 0));
}

// ---
QGAMES::Position PacManII::StandardMap::monsterInitialPosition (int nP) const
{
	QGAMES::Position cPos (__BD ((10 * tileWidth ()) + (tileWidth () >> 1)), 
						   __BD ((10 * tileHeight ()) + (tileHeight () >> 1)), __BD 0);

	return (cPos - QGAMES::Vector (__BD ((tileWidth () >> 1) + tileWidth () * (1 - nP)), __BD 0, __BD 0));
}

// ---
PacManII::TileLayer::TileLayer (int c, const std::string& n, const QGAMES::Tiles& t, QGAMES::Map* m, const QGAMES::LayerProperties& p)
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
void PacManII::TileLayer::BlinkingMaze::initialize ()
{
	_counterBlinkingTime.initialize ();
	_counterNumberBlinks.initialize ();
	_switchBlinkOn.initialize ();

	_active = true;

	// All must bright...
	for (auto i : (dynamic_cast <PacManII::TileLayer*> (_layer)) -> tiles ())
	{
		PacManII::TileLimit* tL = nullptr;
		if ((tL = dynamic_cast <PacManII::TileLimit*> (i)) != nullptr)
			tL -> setBright (true);
	}

	_switchBlinkOn.set (true);
}

// ---
void PacManII::TileLayer::BlinkingMaze::updatePositions ()
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
void PacManII::TileLayer::BlinkingMaze::finalize ()
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
void PacManII::TileLayer::startBlinking (QGAMES::bdata bT, int nB)
{
	setVisualEffect (new PacManII::TileLayer::BlinkingMaze (this, bT, nB));
}

// ---
bool PacManII::TileLayer::isBlinking () const
{
	return ((visualEffect () != nullptr) 
		? (dynamic_cast <const PacManII::TileLayer::BlinkingMaze*> (visualEffect ())) -> isActive () : false);  
}

// ---
void PacManII::TileLayer::stopBlinking ()
{
	setVisualEffect (nullptr);
}

// ---
int PacManII::TileLayer::maxNumberBallsToEat () const
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
int PacManII::TileLayer::numberBallsEaten () const
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
std::string PacManII::TileLayer::ballsEatenStatus () const
{
	std::string result (__NULL_STRING__);

	for (auto i : tiles ())
	{
		const PacManII::TilePath* tl = dynamic_cast <const PacManII::TilePath*> (i);
		result += std::to_string ((int) tl -> type ());
	}

	return (result);
}

// ---
void PacManII::TileLayer::setBallsEatenStatus (const std::string& st)
{
}

// ---
void PacManII::TileLimit::setBright (bool b)
{
	PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
	assert (g); // It can only be made within a PacManII game, that it should be the case...
	PacManII::TMXMapBuilder* mB = g -> tmxAddsOnMapBuilder ();
	assert (mB); // Just in case...
	// Reley in the builder to determine the equivalent frame...

	setForm (form (), 
		(b) ? mB -> brightFrameFor (numberFrame ()) : mB -> darkFrameFor (numberFrame ()));
}

// ---
bool PacManII::TilePath::eaten ()
{ 
	bool result = false;

	if (canBeEaten () && 
		_type != PacManII::TilePath::Type::_EMPTY) // Only if it is possible...
	{
		PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
		assert (g); // It can only be made within a PacManII game, that it should be the case...
		PacManII::TMXMapBuilder* mB = g -> tmxAddsOnMapBuilder ();
		assert (mB); // Just in case...

		_type = PacManII::TilePath::Type::_EMPTY; // No more
		setForm (form (), mB -> frameForEmptyPath ());

		notify (QGAMES::Event (__PACMANII_BALLEATEN__, this));

		result = true;
	}

	return (result);
}

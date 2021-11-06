#include "Game.hpp"
#include "Worlds.hpp"
#include "ScoreObjects.hpp"

// ---
PacManII::Game::Conf::Conf (int nP, int mL, int dL)
	: QGAMES::AdvancedArcadeGame::Conf (nP),
	  _maxLives (mL),
	  _defaultLevel (dL),
	  _playerName (),
	  _score (), 
	  _playerSeconds (),
	  _level (),
	  _levelCompleted (),
	  _triesOnLevel (),
	  _levelBallsEaten ()
{ 
	assert (_numberPlayers > 0);
	assert (_maxLives > 1);
	assert (_defaultLevel > 0);

	adjustToPlayers (_numberPlayers);
}

// ---
void PacManII::Game::Conf::actualizeHallOfFame ()
{
	int iS = (int) _hallOfFame.size ();
	for (int i = 0; i < _numberPlayers; i++)
	{
		QGAMES::OpenValues vals; 
		vals.insert (QGAMES::OpenValues::value_type (0, QGAMES::OpenValue (_score [i])));
		vals.insert (QGAMES::OpenValues::value_type (1, QGAMES::OpenValue (_playerName [i])));
		_hallOfFame.insert (std::multimap <std::string, QGAMES::SetOfOpenValues>::value_type 
			(std::to_string (i + iS), QGAMES::SetOfOpenValues (vals)));
	}
}

// ---
void PacManII::Game::Conf::adjustToPlayers (int nP)
{
	QGAMES::AdvancedArcadeGame::Conf::adjustToPlayers (nP);

	// Readjust the number of lives
	_numberLives = std::vector <int> (nP, _maxLives);

	_playerName = std::vector <std::string> (nP, std::string (__NULL_STRING__));
	_score = std::vector <int> (nP, 0);
	_playerSeconds = std::vector <int> (nP, 0);
	_level = std::vector <int> (nP, _defaultLevel); 
	_levelCompleted = std::vector <std::map <int, bool>> (nP, std::map <int, bool> ());
	_triesOnLevel = std::vector <std::map <int, int>> (nP, std::map <int, int> ());
	_levelBallsEaten = std::vector <std::map <int, std::string>> (nP, std::map <int, std::string> ()); 
	// no ball in none level eaten so far

	// When the players are adjusted, the information kept in the worlds have to be deleted to...
	// Remember that this information is kept by player, and...
	for (QGAMES::Worlds::const_iterator i = (__AGM game ()) -> worlds ().begin (); 
			i != (__AGM game ()) -> worlds ().end (); i++)
	{
		((PacManII::World*) (*i).second) -> initializeRuntimeValuesFrom (QGAMES::SetOfOpenValues ());
		if (existConfValueForElement ((*i).second -> id ())) // Just if the element exists
			setConfValueForElement ((*i).second -> id (), QGAMES::SetOfOpenValues ());
	}
}

// ---
void PacManII::Game::Conf::cfgToStream (std::ostringstream& oS)
{
	QGAMES::AdvancedArcadeGame::Conf::cfgToStream (oS);

	for (int i = 0; i < _numberPlayers; i++)
		oS << _playerName [i] << std::endl;
	for (int i = 0; i < _numberPlayers; i++)
		oS << _score [i] << std::endl;
	for (int i = 0; i < _numberPlayers; i++)
		oS << _playerSeconds [i] << std::endl;
	for (int i = 0; i < _numberPlayers; i++)
		oS << _level [i] << std::endl;
	
	for (int i = 0; i < _numberPlayers; i++)
	{
		oS << _levelCompleted [i].size () << std::endl;
		for (std::map <int, bool>::const_iterator j = _levelCompleted [i].begin (); 
				j != _levelCompleted [i].end (); j++)
			oS << (*j).first << std::endl << (*j).second << std::endl;
	}
	
	for (int i = 0; i < _numberPlayers; i++)
	{
		oS << _triesOnLevel [i].size () << std::endl;
		for (std::map <int, int>::const_iterator j = _triesOnLevel [i].begin (); 
				j != _triesOnLevel [i].end (); j++)
			oS << (*j).first << std::endl << (*j).second << std::endl;
	}
	
	for (int i = 0; i < _numberPlayers; i++)
	{
		oS << _levelBallsEaten [i].size () << std::endl;
		for (std::map <int, std::string>::const_iterator j = _levelBallsEaten [i].begin (); 
				j != _levelBallsEaten [i].end (); j++)
			oS << (*j).first << std::endl << (*j).second << std::endl;
	}
}

// ---
void PacManII::Game::Conf::cfgFromStream (std::istringstream& iS)
{
	QGAMES::AdvancedArcadeGame::Conf::cfgFromStream (iS);

	_playerName.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _playerName [i];
	_score.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _score [i];
	_playerSeconds.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _playerSeconds [i];
	_level.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _level [i];
	
	_levelCompleted.resize (_numberPlayers);
	int nE, nSC; bool sC;
	for (int i = 0; i < _numberPlayers; i++)
	{
		iS >> nE;
		for (int j = 0; j < nE; j++)
		{
			iS >> nSC; iS >> sC;
			_levelCompleted [i][nSC] = sC;
		}
	}
	
	_triesOnLevel.resize (_numberPlayers);
	int nT;
	for (int i = 0; i < _numberPlayers; i++)
	{
		iS >> nE;
		for (int j = 0; j < nE; j++)
		{
			iS >> nSC; iS >> nT;
			_triesOnLevel [i][nSC] = nT;
		}
	}
	
	_levelBallsEaten.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
	{
		iS >> nE;
		for (int j = 0; j < nE; j++)
		{
			iS >> nSC; iS >> sC;
			_levelBallsEaten [i][nSC] = sC;
		}
	}
}

// ---
void PacManII::Game::setLevel (int l, int nP)
{
	setWorld (dataGame ().levelDefinition (l).worldTypeId ());
	activeWorld () -> setScene (dataGame ().levelDefinition (l).sceneTypeId ());
	activeWorld () -> activeScene () -> setMap (dataGame ().levelDefinition (l).mapTypeId ());
	activeWorld () -> initialize ();
	dynamic_cast <PacManII::Map*> (activeWorld () -> activeScene () -> activeMap ()) -> 
		setBallsEatenStatus (levelBallsEaten (level ())); // sets the balls eaten of the level fixed above...

	// To maintain the coherence with the conf inherited!
	// Here the configuracion method is invoked instead to avoid recursive!
	setCurrentWorld (activeWorld () -> id (), nP);
	setCurrentScene (activeWorld () -> activeScene () -> id (), nP);
	((PacManII::Game::Conf*) configuration ()) -> setLevel ((nP == -1) ? currentPlayer () : nP, l);
}

// ---
bool PacManII::Game::stopsAfterCurrentLevel () const
{
	return (true);
//	return (level () % 3 == 0);
}

// ---
bool PacManII::Game::isCurrentLevelLast () const
{
	return (level () == _theDataGame.numberLevels ());
}

// ---
void PacManII::Game::setShowFPS (bool s)
{
	_showFPS = s;

	QGAMES::AdvancedGameStateControl* gCS = 
		dynamic_cast <QGAMES::AdvancedGameStateControl*> 
			(gameStateBuilder () -> gameState (__PACMANII_STANDARDCONTROLGAMESTATE__));
	assert (gCS != nullptr);

	gCS -> setOverlay 
		(_showFPS ? game () -> state (__QGAMES_GAMESTATEFPSNAME__) : nullptr); 
}

// ---
void PacManII::Game::addScoreObjects ()
{
	// It is done one by one, 
	// because depending on the final game implementation different score objects could be avoided

	if (!_lives) 
	{
		addScoreObject (_lives = new PacManII::Lives ()); 
		_lives -> setLives (lives ());
		_lives -> setPosition (
			QGAMES::Position (__BD 10, 
							  __BD (mainScreen () -> visualHeight () - 10 - _lives -> visualHeight ()), 
							  __BD 0));

		_lives -> observe (pacman ());
	}

	if (!_timeCounter)
	{
		addScoreObject (_timeCounter = new PacManII::TimeCounter ());
		_timeCounter -> setSeconds (0); // Starting...
		_timeCounter -> setPosition (QGAMES::Position (__BD 10, __BD 10, __BD 0));
		// Any time the time changes, the time counter is actualized...
	}

	if (!_scoreCounter) 
	{
		addScoreObject (_scoreCounter = new PacManII::ScoreCounter ());
		_scoreCounter -> setNumber (score ());
		_scoreCounter -> setPosition (
			QGAMES::Position (__BD (mainScreen () -> visualWidth () - _scoreCounter -> visualLength () - 20), 
							  __BD 10, 
							  __BD 0));

		_scoreCounter -> observe (pacman ()); // Only for the one playing
	}

	if (!_playerName)
	{
		addScoreObject (_playerName = new PacManII::FixText (0, playerName ())); // First fixed text
		_playerName -> setPosition (QGAMES::Position (
			_lives -> position ().posX (),
			_lives -> position ().posY () - __BD (_playerName -> visualHeight () + 10),
			__BD 0));
		_playerName -> setAlphaLevel (100); // A little bit faded, because it is on the playing zone!
	}
}

// ---
void PacManII::Game::removeScoreObjects ()
{
	PacManII::PacMan* pc = pacman ();
	assert (pc != nullptr); // It has to exist at this point...

	// It is done one by one, 
	// because depending on the final game different score objects could be avoided

	if (_lives) 
	{
		assert (_lives -> isObserving (pc));
		_lives -> unObserve (pc); 
		delete (_lives); _lives = nullptr; 
	}
	
	delete (_timeCounter); _timeCounter = NULL;
	
	if (_scoreCounter) 
	{
		assert (_scoreCounter -> isObserving (pc));
		_scoreCounter -> unObserve (pc); 
		delete (_scoreCounter); _scoreCounter = nullptr; 
	}

	delete (_playerName); _playerName = nullptr;

	QGAMES::AdvancedArcadeGame::removeScoreObjects ();
}

// ---
void PacManII::Game::pauseGame ()
{
	QGAMES::AdvancedArcadeGame::pauseGame ();

	// Stop the seconds...
	loopCounter (_COUNTERSECONDS) -> stop ();
}

// ---
void PacManII::Game::continueGame ()
{
	QGAMES::AdvancedArcadeGame::continueGame ();

	// The seconds continue...
	loopCounter (_COUNTERSECONDS) -> run ();
}

// ---
void PacManII::Game::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __QGAMES_KEYBOARDEVENT__)
	{
		QGAMES::KeyBoardEventData* dt = (QGAMES::KeyBoardEventData*) evnt.data ();
		if (dt -> _internalCode == QGAMES::KeyCode::QGAMES_F1 && !dt -> _on)
			setShowFPS (!_showFPS); 
	}

	QGAMES::AdvancedArcadeGame::processEvent (evnt);
}

// ---
void PacManII::Game::detectCollisions ()
{
	if (_detectCollisions)
		QGAMES::AdvancedArcadeGame::detectCollisions ();
}

// ---
void PacManII::Game::initializeEntitiesAndWorlds ()
{
	// Add the worlds and scenes used in the game...
	std::vector <int> wLst = worldBuilder () -> resourcesId ();
	for (int i = 0; i < (int) wLst.size (); addWorld (wLst [i++]));
	// Add the artists used in the game 
	std::vector <int> eLst = entityBuilder () -> resourcesId ();
	for (int i = 0; i < (int) eLst.size (); i++)
	{
		if (dynamic_cast <QGAMES::Artist*> (entityBuilder () -> entity (eLst [i])))
			addArtist (eLst [i]); // Only artists...and all (none visible at that point, remember!)
	}

	// All the worlds and scenes (and their action blocks too) in the game 
	// are considered as elements which information have to be saved.
	// The entities are not considered in the list of information to be kept
	// Because their status is already part of the general configuration.
	// IMPORTANT: Worlds, Scenes and Action Blocks have to have different ids from each other
	// to guaranttee that they are kept right.
	// Only worlds are added, because when they runtimeValues are got everything nested inside is also got.
	PacManII::Game::Conf* cfg = dynamic_cast <PacManII::Game::Conf*> (configuration ());
	assert (cfg != nullptr);
	QGAMES::Elements elmts;
	for (int i = 0; i < (int) wLst.size (); i++)
		elmts.push_back (world (wLst [i]));

	cfg -> setElementsToKeep (elmts); // They will be also kept (or reload) 
									  // when configuration is kept (or reload)!

	// The previous instruction will have set a not valid initial value for the worlds
	// It could cause an error when the game starts
	// So, it is better to set back those values to nothing!
	for (QGAMES::Worlds::const_iterator i = worlds ().begin (); i != worlds ().end (); i++)
		cfg -> setConfValueForElement ((*i).second -> id (), QGAMES::SetOfOpenValues ());
}

// ---
QGAMES::GameStateBuilder* PacManII::Game::createGameStateBuilder ()
{
	QGAMES::GameStateBuilder* result = QGAMES::AdvancedArcadeGame::createGameStateBuilder ();
	result -> addAddsOn (new PacManII::ForInitializePacManIIGameStateAddOn (result));
	result -> addAddsOn (new PacManII::ForShowingMonsterMovingGameStateAddOn (result));
	result -> addAddsOn (new PacManII::ForShowingPlayerNameGameStateAddOn (result));
	result -> addAddsOn (new PacManII::ForShowingLevelGameStateAddOn (result));
	result -> addAddsOn (new PacManII::ForPlayingControlGameStateAddOn (result));
	result -> addAddsOn (new PacManII::ForPlayingGameStateAddOn (result));
	result -> addAddsOn (new PacManII::ForBlinkMazePlayingGameStateAddOn (result));
	result -> addAddsOn (new PacManII::ForOptionsWhenPlayingGameStateAddOn (result));
	result -> addAddsOn (new PacManII::ForShowingHallOfFameGameStateAddOn (result));
	result -> addAddsOn (new PacManII::ForGameStateControlAddon (result));

	return (result);
}

// ---
QGAMES::MapBuilder* PacManII::Game::createMapBuilder ()
{
	QGAMES::MapBuilder* result = new QGAMES::MapBuilder (parameter (__GAME_MAPSOBJECTSFILE__));
	result -> addAddsOn (_tmxAddsOnMapBuilder = new PacManII::TMXMapBuilder ((QGAMES::Sprite2DBuilder*) formBuilder ()));

	return (result);
}

// ---
QGAMES::Game::Configuration* PacManII::Game::createConfiguration ()
{ 
	return (new Conf 
		(1 /** Default players */, 
		 std::atoi (parameter (__PACMANII_MAXNUMBEROFLIVESPARAMETER__).c_str ()),
		 std::atoi (parameter (__PACMANII_LEVELPARAMETER__).c_str ()))); 
}

// ---
std::string PacManII::Game::defaultParameter (const std::string& p) const
{
	std::string result (__NULL_STRING__);
	if (p == std::string (__PACMANII_MAXNUMBEROFLIVESPARAMETER__))
		result = std::string (__PACMANII_DEFAULTMAXNUMBEROFLIVESPARAMETER__);
	else if (p == std::string (__PACMANII_LEVELPARAMETER__))
		result = std::string (__PACMANII_DEFAULTLEVELPARAMETER__);
	else if (p == std::string (__PACMANII_SHOWPFSPARAMETER__))
		result = std::string (__PACMANII_DEFAULTSHOWFPSPARAMETER__);
	else result = QGAMES::AdvancedArcadeGame::defaultParameter (p);
	return (result);
}

// ---
void PacManII::Game::initialize ()
{
	if (isInitialized ())
		return; // Only once...

	QGAMES::AdvancedArcadeGame::initialize ();

	if (!isInitialized ())
		return;

	// It is not needed to relocate th main screen, because it is alocated at the centr of the creen by default!

	// Add all game states from the definition file...
	addMainStatesFromBuilder (); 
	// ...and start to play with the main one...
	QGAMES::AdvancedGameStateControl* gCS = 
		dynamic_cast <QGAMES::AdvancedGameStateControl*> 
			(gameStateBuilder () -> gameState (__PACMANII_STANDARDCONTROLGAMESTATE__));
	assert (gCS != nullptr);

#ifndef NDEBUG
	setShowFPS (true);
#else
	std::string sfps = parameter (std::string (__PACMANII_SHOWPFSPARAMETER__));
	setShowFPS (QGAMES::toUpper (QGAMES::trim (sfps)) == std::string (__YES_STRING__));
#endif

	setState (gCS); // To start...

	observe (inputHandler ());
}

// ---
void PacManII::Game::finalize ()
{
	unObserve (inputHandler ());

	QGAMES::AdvancedArcadeGame::finalize ();
}

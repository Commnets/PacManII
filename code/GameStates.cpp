#include "GameStates.hpp"
#include "Game.hpp"

// ---
void PacManII::InitializePacManIIGameState::onEnter ()
{
	QGAMES::GameState::onEnter ();

	PacManII::Game* aG = dynamic_cast <PacManII::Game*> (game ());
	assert (aG); // Just in case...

	aG -> initializeEntitiesAndWorlds ();
}

// ---
void PacManII::InitializePacManIIGameState::updatePositions ()
{
	notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this));
}

// ---
PacManII::MonsterMovingGameState::Properties::Properties (const std::map <std::string, std::string>& prps)
	: _fade (255),
	  _position (QGAMES::Position (__BD 0.5, __BD 0.5, __BD 0)), // Center position in a 2D screen...
	  _iterations (-1),
	  _bigPacman (false)
{
	if (prps.find (__PACMANII_GAMESTATEMOVMONSTERATTRFADE__) != prps.end ()) 
		_fade = std::atoi ((*prps.find (__PACMANII_GAMESTATEMOVMONSTERATTRFADE__)).second.c_str ());
	if (prps.find (__PACMANII_GAMESTATEMOVMONSTERATTRPOSITION__) != prps.end ()) 
		_position = QGAMES::Position ((*prps.find (__PACMANII_GAMESTATEMOVMONSTERATTRPOSITION__)).second);
	if (prps.find (__PACMANII_GAMESTATEMOVMONSTERATTRITERATIONS__) != prps.end ()) 
		_iterations = std::atoi ((*prps.find (__PACMANII_GAMESTATEMOVMONSTERATTRITERATIONS__)).second.c_str ());
	if (prps.find (__PACMANII_GAMESTATEMOVMONSTERATTRBIGPACMAN__) != prps.end ()) 
		_bigPacman = (*prps.find (__PACMANII_GAMESTATEMOVMONSTERATTRBIGPACMAN__)).second == std::string (__YES_STRING__) ? true : false;

	PacManII::MonsterMovingGameState::Properties::Properties (_fade, _position, _iterations, _bigPacman);
}

// ---
PacManII::MonsterMovingGameState::~MonsterMovingGameState ()
{
	for (int i = 0; i < (int) _artists.size (); i++)
	{
		_artists [i] -> addControlStepsMonitor (nullptr);
		_artists [i] = nullptr;
	}
}

// ---
void PacManII::MonsterMovingGameState::onEnter ()
{
	QGAMES::GameState::onEnter ();

	// Get the monsters and pacman...
	_artists [0] = dynamic_cast <PacManII::Artist*> (game () -> entity (__PACMANII_PACMANBASEENTITYID__));
	_artists [1] = dynamic_cast <PacManII::Artist*> (game () -> entity (__PACMANII_INKYBASEENTITYID__));
	_artists [2] = dynamic_cast <PacManII::Artist*> (game () -> entity (__PACMANII_PINKYBASEENTITYID__));
	_artists [3] = dynamic_cast <PacManII::Artist*> (game () -> entity (__PACMANII_BLINKYBASEENTITYID__));
	_artists [4] = dynamic_cast <PacManII::Artist*> (game () -> entity (__PACMANII_CLYDEBASEENTITYID__));
#ifndef _NDEBUG
	for (int i = 0; i < (int) _artists.size (); i++)
		assert (_artists [i]);
#endif

	// All of them are set in the state running to the right...
	_artists [0] -> setCurrentState (__PACMANII_PACMANSTATEEATINGLOOKINGRIGHT__);
	_artists [1] -> setCurrentState (__PACMANII_INKYSTATECHASINGANDLOOKINGRIGHT__);
	_artists [2] -> setCurrentState (__PACMANII_BLINKYSTATECHASINGANDLOOKINGRIGHT__);
	_artists [3] -> setCurrentState (__PACMANII_PINKYSTATECHASINGANDLOOKINGRIGHT__);
	_artists [4] -> setCurrentState (__PACMANII_CLYDESTATECHASINGANDLOOKINGRIGHT__);

	reStartAllCounters ();
	counter (_COUNTERITERATIONS) -> initialize 
		((_properties._iterations == -1) ? __MAXINT__ : _properties._iterations, 0, true, false);

	reStartAllOnOffSwitches ();

	QGAMES::Screen* scr = game () -> mainScreen  ();
	setInitialPositionFrom (
		QGAMES::Position (
			__BD scr -> visualWidth () * _properties._position.posX () * __BD -0.4,
			__BD scr -> visualHeight () * _properties._position.posY (),
			__BD 0));

	for (auto i : _artists)
		i -> setVisible (true);
}

// ---
void PacManII::MonsterMovingGameState::updatePositions ()
{
	QGAMES::GameState::updatePositions ();

	QGAMES::Screen* scr = game () -> mainScreen ();

	for (auto i : _artists)
	{
		i -> inEveryLoop ();
		i -> updatePositions ();
	}

	if (counter (_COUNTERMOVE) -> isEnd ())
	{
		QGAMES::bdata r = __BD (onOffSwitch (_SWITCHRIGHT) -> isOn () ? 1 : -1); 
		for (auto i : _artists)
		{
			QGAMES::bdata inc = __BD (((i != _artists [0] && r == __BD 1) || (i == _artists [0] && r != __BD 1)) ? 1.05 : 1);
			i -> setPosition (i -> position () + (__BD 2.5 * r * QGAMES::Vector (__BD inc, __BD 0, __BD 0)));
		}

		bool cd = false;
		if (_artists [0] -> centerPosition ().posX () > (__BD scr -> visualWidth () * __BD 1.4))
		{
			_artists [0] -> setCurrentState (_properties._bigPacman 
				? __PACMANII_BIGPACMANSTATEEATINGLOOKINGLEFT__ : __PACMANII_PACMANSTATEEATINGLOOKINGLEFT__);
			_artists [1] -> setCurrentState (__PACMANII_INKYSTATERUNNINGAWAY__);
			_artists [2] -> setCurrentState (__PACMANII_BLINKYSTATERUNNINGAWAY__);
			_artists [3] -> setCurrentState (__PACMANII_PINKYSTATERUNNINGAWAY__);
			_artists [4] -> setCurrentState (__PACMANII_CLYDESTATERUNNINGAWAY__);

			onOffSwitch (_SWITCHRIGHT) -> set (false); 

			setInitialPositionFrom (
				QGAMES::Position (
					__BD scr -> visualWidth () * __BD 1.4,
					__BD scr -> visualHeight () * _properties._position.posY (),
					__BD 0));
		}
		else
		if (_artists [0] -> centerPosition ().posX () < (__BD scr -> visualWidth () * __BD -0.4))
		{
			// After the defined number of iterations (if any) the nested game state finishes...
			if (counter (_COUNTERITERATIONS) -> isEnd ())
				notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this));
			// ...otherwise starts back again...
			else
			{
				_artists [0] -> setCurrentState (__PACMANII_PACMANSTATEEATINGLOOKINGRIGHT__);
				_artists [1] -> setCurrentState (__PACMANII_INKYSTATECHASINGANDLOOKINGRIGHT__);
				_artists [2] -> setCurrentState (__PACMANII_BLINKYSTATECHASINGANDLOOKINGRIGHT__);
				_artists [3] -> setCurrentState (__PACMANII_PINKYSTATECHASINGANDLOOKINGRIGHT__);
				_artists [4] -> setCurrentState (__PACMANII_CLYDESTATECHASINGANDLOOKINGRIGHT__);

				onOffSwitch (_SWITCHRIGHT) -> set (true); 

				setInitialPositionFrom (
					QGAMES::Position (
						__BD scr -> visualWidth () * __BD -0.4,
						__BD scr -> visualHeight () * _properties._position.posY (),
						__BD 0));
			}
		}
	}
}

// ---
void PacManII::MonsterMovingGameState::drawOn (QGAMES::Screen* scr)
{
	QGAMES::GameState::drawOn (scr);

	for (auto i : _artists)
		i -> drawOn (scr);
}

// ---
void PacManII::MonsterMovingGameState::onExit ()
{
	QGAMES::GameState::onExit ();

	for (int i = 0; i < 5; i++)
	{
		_artists [i] -> setVisible (true);
		_artists [i] = nullptr;
	}
}

// ---
__IMPLEMENTCOUNTERS__ (PacManII::MonsterMovingGameState::Counters)
{
	addCounter (new QGAMES::Counter (PacManII::MonsterMovingGameState::_COUNTERMOVE, 1, 0, true, true));
	// This last will be redefined later...
	addCounter (new QGAMES::Counter (PacManII::MonsterMovingGameState::_COUNTERITERATIONS, 0, 0, true, true));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (PacManII::MonsterMovingGameState::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch (PacManII::MonsterMovingGameState::_SWITCHRIGHT, true));
}

// ---		
void PacManII::MonsterMovingGameState::setInitialPositionFrom (const QGAMES::Position& iPos)
{
	QGAMES::Position p = iPos;

	_artists [0] -> setPosition (p - _artists [0] -> currentForm () -> centerPosition (_artists [0] -> currentAspect ()));
	p -= ((!_properties._bigPacman || 
		   (_properties._bigPacman && onOffSwitch (_SWITCHRIGHT) -> isOn ())) ? __BD 2.4 : __BD 1.0) * 
		QGAMES::Vector (__BD _artists [0] -> visualLength (), __BD 0, __BD 0);

	for (std::vector <PacManII::Artist*>::const_reverse_iterator i = _artists.rbegin (); i != _artists.rend (); i++)
	{
		if ((*i) != _artists [0])
		{
			(*i) -> setPosition (p - (*i) -> currentForm () -> centerPosition ((*i) -> currentAspect ()));
			p -= (__BD 8 / __BD 7) * QGAMES::Vector (__BD (*i) -> visualLength (), __BD 0, __BD 0);
		}
	}
}

// ---
void PacManII::ShowingCurrentPlayerNameGameState::onEnter ()
{
	QGAMES::ShowingStandardTextGameState::onEnter ();

	// If there is no player name already introduced, then 
	// a GUI system is started to ask for it...
	PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
	assert (g); // Just in case...

	if (g -> playerName () == std::string (__NULL_STRING__))
	{
		_guiSystem = game () -> guiSystemBuilder () -> system (__PACMANII_PLAYERNAMEGUISYSTEM__);
		QGAMES::CompositeWidget* cW = dynamic_cast <QGAMES::CompositeWidget*> (_guiSystem -> mainWidget ());
		assert (cW); // Just in case...
		QGAMES::IntroWidget* iW = 
			dynamic_cast <QGAMES::IntroWidget*> (cW -> widget (__PACMANII_PLAYERNAMEOPTIONID__));
		assert (iW); // Just in case...

		// The position of the main widget is defined with the same
		// attributes used when the name has already been introduced!
		int tW = iW -> width (); int tH = iW -> height ();
		QGAMES::Vector adj = QGAMES::Vector::_cero;
		if (_properties._reference == QGAMES::ShowingTextGameState::Properties::PosReference::_CENTER)
			adj = QGAMES::Vector (__BD (tW >> 1), __BD (tH >> 1), __BD 0);
		else if (_properties._reference == QGAMES::ShowingTextGameState::Properties::PosReference::_BOTTONRIGHT)
			adj = QGAMES::Vector (__BD tW, __BD tH, __BD 0);
		_guiSystem -> mainWidget () -> setPosition (QGAMES::Position (
				__BD game () -> mainScreen () -> visualWidth () * _properties._position.posX (),
				__BD game () -> mainScreen () -> visualHeight () * _properties._position.posY (),
				__BD 0) - adj); // The position declared under the properties structure is always a percentage...

		observe (_guiSystem -> mainWidget ());

		// Just to know when the intro is pressed to finish the state
		// and then continue with the game...
		observe (game () -> inputHandler ());
	}
}

// ---
void PacManII::ShowingCurrentPlayerNameGameState::onExit ()
{
	QGAMES::ShowingStandardTextGameState::onExit ();

	if (_guiSystem)
	{
		unObserve (game () -> inputHandler ());

		PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
		assert (g); // Just in case...
		QGAMES::CompositeWidget* cW = dynamic_cast <QGAMES::CompositeWidget*> (_guiSystem -> mainWidget ());
		assert (cW);

		// Get the name introduced...
		std::string txt = ((QGAMES::IntroWidget*) cW -> widget (__PACMANII_PLAYERNAMEOPTIONID__)) -> text ();
		QGAMES::trim (txt); // Erase the spaces...if any
		g -> setPlayerName ((txt == std::string (__NULL_STRING__)) ? std::string ("AAA") : txt); 
		// Sets the name introduced or a default one if none...

		// ...and finally the GUI system is deleted...
		deleteGUISystem ();
	}
}

// ---
void PacManII::ShowingCurrentPlayerNameGameState::processEvent (const QGAMES::Event& evnt)
{
	QGAMES::ShowingStandardTextGameState::processEvent (evnt);

	// When the GUI system is active, and the INTRO key is pressed, the state finishes only
	// when the name of the player is not null...
	if (_guiSystem)
	{
		if (evnt.code () == __QGAMES_KEYBOARDEVENT__ &&
			((QGAMES::KeyBoardEventData*) evnt.data ()) -> _internalCode == QGAMES::KeyCode::QGAMES_RETURN &&
			!((QGAMES::KeyBoardEventData*) evnt.data ()) -> _on && // The key has been released!!
			((QGAMES::IntroWidget*) ((QGAMES::CompositeWidget*) _guiSystem -> mainWidget ()) -> 
				widget (__PACMANII_PLAYERNAMEOPTIONID__)) -> text () != std::string (__NULL_STRING__))
			notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this));
	}
}

// ---
void PacManII::ShowingCurrentPlayerNameGameState::actualizeText ()
{
	QGAMES::ShowingStandardTextGameState::actualizeText ();

	if (!_guiSystem)
	{
		assert (dynamic_cast <PacManII::Game*> (game ()));

		std::string str = _textString + std::string (" ") + // An space after...
			((PacManII::Game*) game ()) -> playerName ();
		_text -> setMaxTextLength (str.length ()); _text -> setText (str);
	}
	else
	{
		_text -> setMaxTextLength (1); // Can't be 0...
		_text -> setText (std::string (__NULL_STRING__));
	}
}

// ---
void PacManII::ShowingCurrentPlayerNameGameState::deleteGUISystem ()
{
	if (_guiSystem)
		unObserve (_guiSystem -> mainWidget ());

	delete (_guiSystem);
	_guiSystem = nullptr;
}


// ---
void PacManII::ShowingCurrentLevelGameState::actualizeText ()
{
	PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
	assert (g);

	QGAMES::ShowingTextGameState::actualizeText ();
	std::string str = _textString + std::string (" " /** a separation. */) + std::to_string (g -> level ());
	_text -> setMaxTextLength (str.length ()); _text -> setText (str);
}

// ---
std::multimap <std::string, QGAMES::SetOfOpenValues> PacManII::ShowingHallOfFameGameState::orderHallOfFame 
	(const std::multimap <std::string, QGAMES::SetOfOpenValues>& hF)
{
	// The result is almost what is being received...
	// But the index of the map has to be the score instead the number of the player...
	std::multimap <int, QGAMES::SetOfOpenValues, std::greater <int>> pR;
	for (std::multimap <std::string, QGAMES::SetOfOpenValues>::const_iterator i = hF.begin ();
			i != hF.end (); i++)
		pR.insert (std::multimap <int, QGAMES::SetOfOpenValues, std::greater <int>>::value_type 
			((*i).second.openValue (0).intValue (), (*i).second)); // The score in the openValu number 0..
	// Because the mulmap is ordered using the score...

	// The result is reasy to translate into...
	std::multimap <std::string, QGAMES::SetOfOpenValues> result;
	for (std::multimap <int, QGAMES::SetOfOpenValues, std::greater <int>>::const_iterator i = pR.begin (); i != pR.end (); i++)
		result.insert (std::multimap <std::string, QGAMES::SetOfOpenValues>::value_type (std::to_string ((*i).first), (*i).second));

	return (result);
}

// ---
QGAMES::ScoreObjectText* PacManII::ShowingHallOfFameGameState::createTextFor 
	(const std::string& eK, const QGAMES::SetOfOpenValues& sV)
{
	std::string scr = std::to_string (sV.openValue (0).intValue ());
	if (scr.length () < 5) 
		scr = std::string ("00000").substr (0, 5 - scr.length ()) + scr;
	return (game () -> textBuilder () -> createText 
		(properties ()._font, (scr + std::string ("  ") + sV.openValue (1).strValue ())));
}

// ---
PacManII::PlayingGameState* PacManII::PlayingControlGameState::playingState ()
{
	PacManII::PlayingGameState* result = nullptr;
	for (QGAMES::GameStates::const_iterator i = states ().begin (); i != states ().end () && !result; i++)
		if (dynamic_cast <PacManII::PlayingGameState*> ((*i).second))
			result = (PacManII::PlayingGameState*) ((*i).second);
	return (result);
}

// ---
void PacManII::PlayingControlGameState::onEnter ()
{
	QGAMES::GameStateControl::onEnter ();

	PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
	assert (g); // Just in case...
	_pacman = g -> pacman ();
	assert (_pacman);

	g -> addScoreObjects ();

	// This instruction sets the world and the scene accoinding to the level defined in DataGame
	g -> setLevel (g -> level ());

	g -> setSeconds (0);

	g -> mainScreen () -> setPosition (QGAMES::Position (__BD 32, __BD 0, __BD 0));
}

// ---
void PacManII::PlayingControlGameState::onExit ()
{
	QGAMES::GameStateControl::onExit ();

	PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
	assert (g); // Just in case...

	// Just to maintain the compatibility with the old conf...
	g -> setSceneVisited (g -> activeWorld () -> activeScene () -> id ()); 

	g -> activeWorld () -> finalize ();
	// When the world is fanalized, the scene will be too, and the list of the eaten balls will save also into conf.

	g -> setWorld (__MININT__);

	g -> removeScoreObjects ();

	g -> mainScreen () -> setPosition (QGAMES::Position::_cero);

	_pacman = nullptr;
}

// ---
int PacManII::PlayingControlGameState::circunstanceWhen (QGAMES::GameState* st, const QGAMES::Event& evnt)
{
	int result = 0;

	if (evnt.code () == __QGAMES_GAMENESTEDSTATEFINISHED__)
	{
		switch (st -> type ())
		{
			case __PACMANII_PLAYINGGAMESTATE__:
				{
					PacManII::PlayingGameState* gS = dynamic_cast <PacManII::PlayingGameState*> (st);
					assert (gS);

					switch (gS -> lastMainCharacterState ())
					{
						case PacManII::PlayingGameState::MainCharacterState::_DIED:
							result = 0; // What a pitty!!
							break;

						case PacManII::PlayingGameState::MainCharacterState::_REACHGOAL:
							result = 1; // All energy balls have been eaten!
							break;

						case PacManII::PlayingGameState::MainCharacterState::_OPTIONS:
							result = 2; // Asking for options to continue...
							break;
						
						default:
							// No other situation of the character is tken into account so far!
							assert (0);
							break;
					}
				}

				break;

			case __PACMANII_OPTIONSWHENPLAYINGGAMESTATE__:
				{
					PacManII::OptionsWhenPlayingGameState* gS =
						dynamic_cast <PacManII::OptionsWhenPlayingGameState*> (st);
					assert (gS);

					switch (gS -> lastOptionSelected ())
					{
						case __PACMANII_GUISYSTEMEXITGAMEWDT__:
							result = 1; // To exit has been selected...
							break;

						default:
							result = 0; // Other option has been selected, but the code will necer reach this position!
							break; 
					}
				}

				break;

			default:
				result = 0;
				break; 
		}
	}

	return (result);
}

// ---
void PacManII::PlayingGameState::onEnter ()
{
	QGAMES::GameState::onEnter ();

	PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
	assert (g); // Just in case...
	_pacman = g -> pacman ();
	assert (_pacman); // Just in case...

	observe (_pacman);

	_lastMainCharacterState = PacManII::PlayingGameState::MainCharacterState::_MOVING;

	observe (g -> inputHandler ());
	
	// The _COUNTERSECONDS loop (created at construction time of any AdvacedArcadeGame)
	// will also be used to update the number of seconds the main entity stands in the scene (@see processEvent method in this class)
	// That number of seconds is kept in the configuration object of the game, 
	// and it is set to 0 when that object is instatiated first time or cleaned (@see @adjustPlayers in PacManII::Game)
	observe (g -> loopCounter (PacManII::Game::_COUNTERSECONDS));
}

// ---
void PacManII::PlayingGameState::onExit ()
{
	QGAMES::GameState::onExit ();

	PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
	assert (g); // Just in case...

	unObserve (g -> loopCounter (PacManII::Game::_COUNTERSECONDS));

	unObserve (g -> inputHandler ());
	
	unObserve (_pacman);

	_pacman = nullptr;
}

// ---
void PacManII::PlayingGameState::processEvent (const QGAMES::Event& evnt)
{
	switch (evnt.code ())
	{
		// When the main character says that he is dead!!
		case __PACMANII_PACMANDESTROYED__:
			{
				if (evnt.data () == (void*) _pacman)
				{
					_lastMainCharacterState = PacManII::PlayingGameState::MainCharacterState::_DIED;
					notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this)); // To finish the state...
				}
			}

			break;

		// When the main character has reached his goal!!
		case __PACMANII_PACMANREACHEDGOAL__:
			{
				if (evnt.data () == (void*) _pacman)
				{
					_lastMainCharacterState = PacManII::PlayingGameState::MainCharacterState::_REACHGOAL;
					notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this)); // To finish the state...
				}
			}

			break;

		// When a key has been pressed in the input handler...
		// For something to happen, that key has to be either ESC (then the options pannel appears but nothings stops)
		// or the pause / continue key (then the game stops or continue)
		case __QGAMES_KEYBOARDEVENT__:
			{
				QGAMES::AdvancedArcadeGame::Conf* gCfg = 
					dynamic_cast <QGAMES::AdvancedArcadeGame::Conf*> (game () -> configuration ());
				assert (gCfg); // Just in case...(to look for the right key later)

				QGAMES::KeyBoardEventData* dt = (QGAMES::KeyBoardEventData*) evnt.data ();
				if (dt -> _internalCode == QGAMES::KeyCode::QGAMES_ESCAPE && !dt -> _on) // ESC for options
				{
					if (_pacman -> isAlive ()) // Only if moving is valid...
					{
						_lastMainCharacterState = PacManII::PlayingGameState::MainCharacterState::_OPTIONS;

						notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this)); // This state has to finish...
					}
				}
				else
				if (dt -> _internalCode == gCfg -> controlKeys ()[__QGAMES_KEYPAUSE__] && !dt -> _on) // PAUSE to pause
				{
					// Either stops or continue the game...
					if (game () -> isGamePaused ()) 
						game () -> continueGame ();
					else 
						game () -> pauseGame ();
				}
				else
				if (dt -> _internalCode == gCfg -> controlKeys ()[__QGAMES_KEYSAVE__] && !dt -> _on) // Save to save...
				{
					PacManII::Game::Conf* conf = 
						dynamic_cast <PacManII::Game::Conf*> (game () -> configuration ());
					assert (conf); // It must be a BATTLESHIP
					conf -> save (game () -> parameter (__GAME_DATADIRPROPERTYNAME__) + 
						std::string (__PATH_SEPARATOR__) + QGAMES::StandardInitialSelectionOptionsGameState::_CONFFILENAME);
				}
			}

			break;

		// ---
		// Now a set of events comming from the specialized input event handler...
		// Now the group of events related with how to control the main artist...
		case __PACMANII_MOVINGUPEVENT__:
			_pacman -> toMove (QGAMES::Vector (__BD 0, __BD -1, __BD 0));
			break;

		case __PACMANII_MOVINGRIGHTEVENT__:
			_pacman -> toMove (QGAMES::Vector (__BD 1, __BD 0, __BD 0));
			break;

		case __PACMANII_MOVINGUPRIGHTEVENT__:
			_pacman -> toMove (QGAMES::Vector (__BD 1, __BD -1, __BD 0));
			break;

		case __PACMANII_MOVINGDOWNEVENT__:
			_pacman -> toMove (QGAMES::Vector (__BD 0, __BD 1, __BD 0));
			break;

		case __PACMANII_MOVINGDOWNRIGHTEVENT__:
			_pacman -> toMove (QGAMES::Vector (__BD 1, __BD 1, __BD 0));
			break;

		case __PACMANII_MOVINGLEFTEVENT__:
			_pacman -> toMove (QGAMES::Vector (__BD -1, __BD 0, __BD 0));
			break;

		case __PACMANII_MOVINGUPLEFTEVENT__:
			_pacman -> toMove (QGAMES::Vector (__BD -1, __BD -1, __BD 0));
			break;

		case __PACMANII_MOVINGDOWNLEFTEVENT__:
			_pacman -> toMove (QGAMES::Vector (__BD -1, __BD 1, __BD 0));
			break;

		case __PACMANII_NOMOVINGEVENT__:
			_pacman -> toMove (QGAMES::Vector::_cero);
			break;

		case __PACMANII_FIREEVENT__:
			{
				PacManII::InputHandlerBehaviour::NotificationData* dt = 
					(PacManII::InputHandlerBehaviour::NotificationData*) evnt.data ();
				_pacman -> toShoot (1 + (int) (__BD dt -> _force / __BD 10), QGAMES::Vector (__BD 0, __BD -1, __BD 0));
			}

			break;
		// ---

		// Observing the loop just to add seconds to the time that the player has been playing
		// When this happen, then a notification happens there an the score could be actualized!
		case __QGAMES_LOOPCOUNTEREVENT__:
			{
				PacManII::Game* dG = dynamic_cast <PacManII::Game*> (game ());
				assert (dG); // Just in case...
				dG -> addToPlayerSeconds (1); // To the current player...
			}

			break;

		default:
			QGAMES::GameState::processEvent (evnt);
			break;
	}
}

// ---
PacManII::BlinkMazePlayingGameState::Properties::Properties (const std::map <std::string, std::string>& prps)
	: _blinkingTime (__BD 0.25),
	  _numberBlinks (5)
{
	if (prps.find (__PACMANII_GAMESTATEBLINKMAZEATTRBLINKTIME__) != prps.end ()) 
		_blinkingTime = __BD (std::atof ((*prps.find (__PACMANII_GAMESTATEBLINKMAZEATTRBLINKTIME__)).second.c_str ()));
	if (prps.find (__PACMANII_GAMESTATEBLINKMAZEATTRNUMBERBLINKS__) != prps.end ()) 
		_numberBlinks = std::atoi ((*prps.find (__PACMANII_GAMESTATEBLINKMAZEATTRNUMBERBLINKS__)).second.c_str ());

	PacManII::BlinkMazePlayingGameState::Properties (_blinkingTime, _numberBlinks);
}

// ---
void PacManII::BlinkMazePlayingGameState::onEnter ()
{
	QGAMES::GameState::onEnter ();

	PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
	assert (g); // Just in case...
	if ((_mazeWorld = dynamic_cast <PacManII::World*> (g -> activeWorld ())) != nullptr)
		_mazeWorld -> startBlinking  (_properties._blinkingTime, _properties._numberBlinks); // Start blinking...
	else
		notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this)); // The state has to finish if there is no a tiles map...
}

// ---
void PacManII::BlinkMazePlayingGameState::updatePositions ()
{
	QGAMES::GameState::updatePositions ();

	if (!_mazeWorld -> isBlinking ())
		notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this));
}

// ---
void PacManII::BlinkMazePlayingGameState::onExit ()
{
	QGAMES::GameState::onExit ();

	if (_mazeWorld != nullptr)
		_mazeWorld ->  stopBlinking ();
	_mazeWorld = nullptr;
}

// ---
void PacManII::OptionsWhenPlayingGameState::onEnter ()
{
	QGAMES::SelectionOptionsGameState::onEnter ();

	observe (game () -> inputHandler ());
}

// ---
void PacManII::OptionsWhenPlayingGameState::onExit ()
{
	unObserve (game () -> inputHandler ());

	QGAMES::SelectionOptionsGameState::onExit ();
}

// ---
void PacManII::OptionsWhenPlayingGameState::processEvent (const QGAMES::Event& evnt)
{
	QGAMES::SelectionOptionsGameState::processEvent (evnt);

	// If the ESC key is press, the the state finishes...
	if (evnt.code () == __QGAMES_KEYBOARDEVENT__)
	{
		QGAMES::KeyBoardEventData* dt = (QGAMES::KeyBoardEventData*) evnt.data ();
		if (dt -> _internalCode == QGAMES::KeyCode::QGAMES_ESCAPE && !dt -> _on)
			onOffSwitch (_SWITCHFINISH) -> set (true);
	}
}

// ---
QGAMES::SelectionOptionsGameState::Menus PacManII::OptionsWhenPlayingGameState::createMenus ()
{
	QGAMES::SelectionOptionsGameState::Menus result;
	result.insert (QGAMES::SelectionOptionsGameState::Menus::value_type 
		(__PACMANII_OPTIONWHENPLAYINGGUISYSTEM__, QGAMES::SelectionOptionsGameState::Menu 
			(__PACMANII_OPTIONWHENPLAYINGGUISYSTEM__, QGAMES::Position (__BD 10, __BD 10, __BD 0))));
	return (result);
}

// ---
int PacManII::OptionsWhenPlayingGameState::SelectionHandler::whenWidgetSelected (QGAMES::Widget* wdt)
{
	PacManII::Game::Conf* conf = 
		dynamic_cast <PacManII::Game::Conf*> (owner () -> game () -> configuration ());
	assert (conf); // It must be a BATTLESHIP

	int result = 0; // To continue...

	switch (wdt -> id ())
	{
		case __PACMANII_GUISYSTEMEXITGAMEWDT__:
		case __PACMANII_GUISYSTEMCONTINUEGAMEWDT__:
			result = wdt -> id (); // Time to exit the menu...with the option selected.
			break;

		case __PACMANII_GUISYSTEMSAVEGAMEWDT__:
			conf -> save (owner () -> game () -> parameter (__GAME_DATADIRPROPERTYNAME__) + 
				std::string (__PATH_SEPARATOR__) + QGAMES::StandardInitialSelectionOptionsGameState::_CONFFILENAME);
			// ...but it continues in the menu...
			owner () -> game () -> sound (__QGAMES_SAVEWAVSOUND__) -> play (__QGAMES_GAMESTATESOUNDCHANNEL__);
			break;

		default:
			assert (0); // It shouldn't be here...It means there is options not controlled!!
			break;
	}

	return (result);
}

// ---
int PacManII::GameStateControl::circunstanceWhen (QGAMES::GameState* st, const QGAMES::Event& evnt)
{
	int result = 0;

	if (evnt.code () == __QGAMES_GAMENESTEDSTATEFINISHED__)
	{
		PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
		assert (g);

		switch (st -> type ())
		{
			// When the state has finishes because an options has been selected...
			case __PACMANII_MENUGAMESTATE__:
				{
					QGAMES::StandardInitialSelectionOptionsGameState* gSS =
						dynamic_cast <QGAMES::StandardInitialSelectionOptionsGameState*> (st);
					assert (gSS); // Just in case there is mistake at some point of the code not totally controlled...
					switch (gSS -> lastOptionSelected ())
					{
						case __QGAMES_STDGUISYSTEMSTARTGAMEWDT__: 
							result = 0; // Start the game...
							break;

						case __QGAMES_STDGUISYSTEMEXITWDT__: 
							result = 1; // Exit the game...
							break;

						case __QGAMES_STDGUISYSTEMHALLOFFAMEWDT__: 
							result = 2; // Hall of fame...
							break;

						default:
							assert (0); // It shouldn't be here...It means there is an option not well managed!!
							break;
					}
				}

				break;

			// When the playing control finishes...
			case __PACMANII_CONTROLPLAYINGGAMESTATE__:
				{
					// The playing has finished, but depending on its final state
					// What to do will be different...
					PacManII::PlayingControlGameState* cPS = 
						dynamic_cast <PacManII::PlayingControlGameState*> (st);
					assert (cPS); // Just in case...it shouldn't happen but who knows!
					PacManII::PlayingGameState* pS = cPS -> playingState ();
					assert (pS); // Same!...

					switch (pS -> lastMainCharacterState ())
					{
						case PacManII::PlayingGameState::MainCharacterState::_DIED:
							result = 0; // Situation when the pacman dies...
							break;

						case PacManII::PlayingGameState::MainCharacterState::_REACHGOAL:
							// When the pacman reaches the goal, either the game finishes when the level is the last possible (4)
							// or stops (a pacman is threaten by a monster) for a while if the level requieres it, or
							// just moves to the next level possible...
							result = (g -> stopsAfterCurrentLevel () ? (g -> isCurrentLevelLast () ? 4 : 3) : 1);
							g -> setScore (g -> pacman () -> score ());
							g -> setNextLevel ();
							break;

						case PacManII::PlayingGameState::MainCharacterState::_OPTIONS:
							result = 2; // The main character is asking for options to continue...
							break;

						default:
							assert (0); // It shouldn't be here. It means there is an state not weel managed!!
							break;
					}
				}

				break;

			// When the state showing that player has died finishes, it needed to take a decision...
			case __PACMANII_PLAYERDIEDGAMESTATE__:
				{
					// If the player is the last and there is no more player to play...
					// Then the game finishes...
					int cPlayer = g -> currentPlayer ();
					g -> setLives (g -> lives () - 1);
					do g -> setNextPlayer (); 
					while (g -> lives () == 0 && cPlayer != g ->  currentPlayer ());

					result = (g -> lives () == 0 && cPlayer == g -> currentPlayer ()) ? 1 : 0;
					// 1 finishes the game, 0 starts back again with a new player...
				}

				break;

			// When the state of "game over" finished...
			// It is time to actualize the hall of fame!
			// The playing state will have finished and also the ninja and then the score will have been actualized!
			case __PACMANII_PLAYERGAMEOVERGAMESTATE__:
				{
					g -> actualizeHallOfFame ();

					result = 0;
				}

				break;

			default:
				// Standard result is enough...
				break;
		}
	}

	return (result);
}

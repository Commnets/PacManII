#include "SceneActionBlocks.hpp"
#include "Game.hpp"
#include "Worlds.hpp"
#include "Scenes.hpp"
#include "Maps.hpp"

// ---
PacManII::MonsterSceneActionBlock::Properties::Properties (const QGAMES::SceneActionBlockProperties& prps)
	: _entityId (0), _numberInMap (0)
{
	if (prps.find (std::string (__PACMANII_MONSTERSCENEBLOCKENTITYIDATTR__)) != prps.end ())
		_entityId = std::atoi ((*prps.find (std::string (__PACMANII_MONSTERSCENEBLOCKENTITYIDATTR__))).second.c_str ());
	if (prps.find (std::string (__PACMANII_MONSTERSCENEBLOCKNUMBEERMONSTERATTR__)) != prps.end ())
		_numberInMap = std::atoi ((*prps.find (std::string (__PACMANII_MONSTERSCENEBLOCKNUMBEERMONSTERATTR__))).second.c_str ());
	if (prps.find (std::string (__PACMANII_MONSTERSCENEBLOCKPOINTSATTR__)) != prps.end ())
		_points = std::atoi ((*prps.find (std::string (__PACMANII_MONSTERSCENEBLOCKPOINTSATTR__))).second.c_str ());

	PacManII::MonsterSceneActionBlock::Properties (_entityId, _numberInMap, _points);
}

// ---
bool PacManII::MonsterSceneActionBlock::timeToStart (int nM)
{
	assert (_monster != nullptr);

	bool result = false;
	if (_monster -> monsterNumber () == nM && !onOffSwitch (_SWITCHTOSTART) -> isOn ())
		onOffSwitch (_SWITCHTOSTART) -> set (result = true);

	return (result);
}

// ---
void PacManII::MonsterSceneActionBlock::initialize ()
{
	QGAMES::SceneActionBlock::initialize ();

	_monster = dynamic_cast <PacManII::Monster*> (game () -> character (_properties._entityId));
	assert (_monster != nullptr);

	scene () -> addCharacter (_monster);

	PacManII::Map* aM = dynamic_cast <PacManII::Map*> (scene () -> activeMap ());
	assert (aM != nullptr);
	_monster -> setMap (aM);

	_monster -> initialize ();

	_monster -> setPoints (_properties._points);

	_monster -> setOrientation (QGAMES::Vector (__BD 0, __BD -1, __BD 0));
	_monster -> toStand ();

	_monster -> setVisible (true);

	_monster -> setPosition (aM -> mazePositionToMapPosition (aM -> monsterInitialPosition (_properties._numberInMap)) - 
		QGAMES::Vector (__BD (_monster -> visualHeight () >> 1), __BD (_monster -> visualHeight () >> 1), __BD 0));

	observe (_monster);

	reStartAllOnOffSwitches ();

	PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
	assert (g != nullptr);
}

// ---
void PacManII::MonsterSceneActionBlock::updatePositions ()
{
	QGAMES::SceneActionBlock::updatePositions ();

	PacManII::Scene* scn = dynamic_cast <PacManII::Scene*> (scene ());
	assert (scn != nullptr);
	if (!scn -> clapperBoard ())
		return;

	assert (_monster != nullptr);

	if (onOffSwitch (_SWITCHMOVING) -> isOn ())
	{
		if (_monster -> status () == PacManII::Monster::Status::_ATHOME)
			onOffSwitch (_SWITCHMOVING) -> set (false);
	}
	else
	{
		if (_monster -> isAtHome ())
		{
			// There could be intrnal conditions to push monster to start the movement..
			if (!onOffSwitch (_SWITCHTOSTART) -> isOn () && readyToStart ())
				onOffSwitch (_SWITCHTOSTART) -> set (true);

			if (onOffSwitch (_SWITCHTOSTART) -> isOn ())
			{
				_monster -> toMove (_monster -> directionToStartMovement ()); // Never chase...

				onOffSwitch (_SWITCHMOVING) -> set (true);
			}
		}
		else
		{
			_monster -> toMove (_monster -> directionToStartMovement ()); // Never chase...
			if (scn -> chasingMode () == true)
				_monster -> toChase (true);

			onOffSwitch (_SWITCHTOSTART) -> set (true);
			onOffSwitch (_SWITCHMOVING) -> set (true);
		}

		if (onOffSwitch (_SWITCHMOVING) -> isOn ())
			notify (QGAMES::Event (__PACMANII_MONSTERSTARTEDTOMOVE__, this));
	}
}

//
void PacManII::MonsterSceneActionBlock::finalize ()
{
	QGAMES::SceneActionBlock::finalize ();

	unObserve (_monster);

	_monster -> setMap (nullptr);

	scene () -> removeCharacter (_monster);

	_monster = nullptr; 
}

// ---
void PacManII::MonsterSceneActionBlock::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __PACMANII_LIMITMOVEMENTREACHED__)
	{
		assert (evnt.data () == static_cast <void*> (_monster));

		PacManII::Scene* scn = dynamic_cast <PacManII::Scene*> (scene ());
		assert (scn != nullptr);

		_monster -> toChaseDeferred (scn -> chasingMode ());
	}

	QGAMES::SceneActionBlock::processEvent (evnt);
}

// ---
bool PacManII::MonsterSceneActionBlock::readyToStart ()
{
	PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
	assert (g != nullptr);
	PacManII::Scene* scn = dynamic_cast <PacManII::Scene*> (scene ());
	assert (scn != nullptr);

	return (scn -> numberBallsEatenRound () >= g -> dataGame ().levelDefinition 
		(g -> level ()).leaveHomeConditions ().dotsToLeaveFor (_monster -> monsterNumber (), scn -> isFirstRound ()));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (PacManII::MonsterSceneActionBlock::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch (PacManII::MonsterSceneActionBlock::_SWITCHTOSTART, false));
	addOnOffSwitch (new QGAMES::OnOffSwitch (PacManII::MonsterSceneActionBlock::_SWITCHMOVING, false));
}

// ---
PacManII::FruitSceneActionBlock::Properties::Properties (const QGAMES::SceneActionBlockProperties& prps)
	: _entityId (0),
	  _type (0), _points (200),
	  _secondsToAppear (__BD 7), _secondsToDisappear (__BD 7)
{
	if (prps.find (std::string (__PACMANII_FRUITSCENEBLOCKENTITYIDATTR__)) != prps.end ())
		_entityId = std::atoi ((*prps.find (std::string (__PACMANII_FRUITSCENEBLOCKENTITYIDATTR__))).second.c_str ());
	if (prps.find (std::string (__PACMANII_FRUITSCENEBLOCKTYPEATTR__)) != prps.end ())
		_type = std::atoi ((*prps.find (std::string (__PACMANII_FRUITSCENEBLOCKTYPEATTR__))).second.c_str ());
	if (prps.find (std::string (__PACMANII_FRUITSCENEBLOCKPOINTSATTR__)) != prps.end ())
		_points = std::atoi ((*prps.find (std::string (__PACMANII_FRUITSCENEBLOCKPOINTSATTR__))).second.c_str ());
	if (prps.find (std::string (__PACMANII_FRUITSCENEBLOCKSECONDSTOAPPEARATTR__)) != prps.end ())
		_secondsToAppear = __BD (std::atof ((*prps.find (std::string (__PACMANII_FRUITSCENEBLOCKSECONDSTOAPPEARATTR__))).second.c_str ()));
	if (prps.find (std::string (__PACMANII_FRUITSCENEBLOCKSECONDSTODISAPPEARATTR__)) != prps.end ())
		_secondsToDisappear = __BD (std::atof ((*prps.find (std::string (__PACMANII_FRUITSCENEBLOCKSECONDSTODISAPPEARATTR__))).second.c_str ()));

	PacManII::FruitSceneActionBlock::Properties (_entityId, _type, _points, _secondsToAppear, _secondsToDisappear);
}

// ---
void PacManII::FruitSceneActionBlock::initialize ()
{
	QGAMES::SceneActionBlock::initialize ();

	_fruit = dynamic_cast <PacManII::Fruit*> (game () -> character (_properties._entityId));
	assert (_fruit != nullptr);

	scene () -> addCharacter (_fruit);

	PacManII::Map* aM = dynamic_cast <PacManII::Map*> (scene () -> activeMap ());
	assert (aM != nullptr);
	_fruit -> setMap (aM);

	_fruit -> initialize ();

	_fruit -> setType (_properties._type);
	_fruit -> setPoints (_properties._points);

	_fruit -> setStatus (PacManII::Fruit::Status::_NOTDEFINED);

	_fruit -> setVisible (false);

	reStartAllCounters ();
	reStartAllOnOffSwitches ();

	counter (_COUNTERTOAPPEARFRUIT) -> 
		initialize ((int) (_properties._secondsToAppear * __BD QGAMES::Game::game () -> framesPerSecond ()), 0, true, false); 
	counter (_COUNTERTODISAPPEARFRUIT) ->  
		initialize ((int) (_properties._secondsToDisappear * __BD QGAMES::Game::game () -> framesPerSecond ()), 0, true, false); 
}

// ---
void PacManII::FruitSceneActionBlock::updatePositions ()
{
	QGAMES::SceneActionBlock::updatePositions ();

	PacManII::Scene* scn = dynamic_cast <PacManII::Scene*> (scene ());
	if (!scn -> clapperBoard ())
		return;

	if (onOffSwitch (_SWITCHFRUITSHOWN) -> isOn ())
		return; // Nothing else to do one the fruit has been shown...

	// When there is no fruit in the scene...
	if (!onOffSwitch (_SWITCHFRUITONSCENE) -> isOn ())
	{
		// ...and it is time for it....
		if (counter (_COUNTERTOAPPEARFRUIT) -> isEnd ())
		{
			PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
			assert (g != nullptr);

			_fruit -> setType (g -> levelDefinition (g -> level ()).bonusSymbolId ());
			_fruit -> setStatus (PacManII::Fruit::Status::_SHOWN);

			onOffSwitch (_SWITCHFRUITONSCENE) -> set (true);

			counter (_COUNTERTODISAPPEARFRUIT) -> initialize ();
		}
	}
	// ...and when no
	else
	{
		bool tD = false;
		if (_fruit -> isAlive ()) // It could be aten whilst it is on the screen...
		{
			if (counter (_COUNTERTODISAPPEARFRUIT) -> isEnd ())
			{
				_fruit -> setStatus (PacManII::Fruit::Status::_NOTDEFINED);
				
				tD = true;
			}
		}
		else
			tD = true;

		if (tD)
		{
			onOffSwitch (_SWITCHFRUITONSCENE) -> set (false);
			onOffSwitch (_SWITCHFRUITSHOWN) -> set (true);
		}
	}
}

//
void PacManII::FruitSceneActionBlock::finalize ()
{
	QGAMES::SceneActionBlock::finalize ();

	_fruit -> setMap (nullptr);

	scene () -> removeCharacter (_fruit);

	_fruit = nullptr; 
}

// ---
__IMPLEMENTCOUNTERS__ (PacManII::FruitSceneActionBlock::Counters)
{
	addCounter (new QGAMES::Counter 
		(PacManII::FruitSceneActionBlock::_COUNTERTOAPPEARFRUIT, 0, 0 , true, false));
	addCounter (new QGAMES::Counter 
		(PacManII::FruitSceneActionBlock::_COUNTERTODISAPPEARFRUIT, 0, 0, true, false)); 
	// Both will be initialized later...
}

// ---
__IMPLEMENTONOFFSWITCHES__ (PacManII::FruitSceneActionBlock::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(PacManII::FruitSceneActionBlock::_SWITCHFRUITONSCENE, false));
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(PacManII::FruitSceneActionBlock::_SWITCHFRUITSHOWN, false));
}

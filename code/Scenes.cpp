#include "Scenes.hpp"
#include "SceneActionBlocks.hpp"
#include "Maps.hpp"
#include "Game.hpp"

// ---
PacManII::Scene::Scene (int c, const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn, 
		const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
	: QGAMES::Scene (c, m, cn, p, ePL),
	  _clapperBoard (false),
	  _percentageCleaned (__BD 0),
	  _totalNumberBallsToEat (-1), // initialized later
	  _totalNumberBallsEaten (-1), // initialized later
	  _numberBallsEaten (-1), // initialized later
	  _firstTimeUpdateMethod (true),
	  _firstRound (true),
	  _monsterActionBlocks (),
	  _numberMonstersMoving ()
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
	if (_totalNumberBallsToEat != -1)
		return (_totalNumberBallsToEat);

	// Only over the actve map if any...
	const PacManII::Map* mp = dynamic_cast <const PacManII::Map*> (activeMap ());
	return (_totalNumberBallsToEat = (mp != nullptr) ? mp -> maxNumberBallsToEat () : 0);
}

// ---
int PacManII::Scene::numberBallsEaten () const
{
	if (_totalNumberBallsEaten != -1)
		return (_totalNumberBallsEaten);

	// Only over the actve map if any...
	const PacManII::Map* mp = dynamic_cast <const PacManII::Map*> (activeMap ());
	return (_totalNumberBallsEaten = (mp != nullptr) ? mp -> numberBallsEaten () : 0);
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
	if (_percentageCleaned > 0.30 && _percentageCleaned < 0.60) sR = SirenRate::_FAST;
	if (_percentageCleaned > 0.60) sR = SirenRate::_VERYFAST;
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
	// The scene has to be initialized just after characters are added...
	QGAMES::Scene::initialize ();

	_totalNumberBallsToEat = maxNumberBallsToEat ();
	_totalNumberBallsEaten = numberBallsEaten ();
	_numberBallsEaten = 0;
	_percentageCleaned = __BD _totalNumberBallsEaten / __BD _totalNumberBallsToEat;
	_firstRound = _totalNumberBallsEaten == 0; // In other circunstances it is not the first round...

	reStartAllCounters ();
	reStartAllOnOffSwitches ();

	PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
	assert (g != nullptr);
	const PacManII::DataGame::LevelDefinition& lD = g -> levelDefinition (g -> level ());
	const PacManII::DataGame::LevelDefinition::ScatterChaseCycle& sCC = 
		lD.scatterChaseCycle (counter (_COUNTERMONSTERCHASINGCYCLES) -> value ());
	const PacManII::DataGame::LevelDefinition::LeaveHomeConditions& lHC = lD.leaveHomeConditions ();
	counter (_COUNTERMONSTERSCHASING) -> initialize 
		((int) (sCC.secondsChase () * __BD g -> framesPerSecond ()), 0, true, false);
	counter (_COUNTERMONSTERSRUNNINGAWAY) -> initialize 
		((int) (sCC.secondsScatter () * __BD g -> framesPerSecond ()), 0, true, false);
	counter (_COUNTERMONSTERSTIMETOLEAVEHOME) -> initialize 
		((int) (lHC.maxSecondsToLeave () * __BD g -> framesPerSecond ()), 0, true, true);

	_firstTimeUpdateMethod = true;
}

// ---
void PacManII::Scene::updatePositions ()
{
	QGAMES::Scene::updatePositions ();

	if (_firstTimeUpdateMethod)
	{
		_firstTimeUpdateMethod = false;

		// Transmit to all monsters reference to th others
		// Some of thm could need those for e.g. calculate the target position
		std::vector <PacManII::Artist*> rf = { };
		for (auto i : characters ())
			if (dynamic_cast <PacManII::Artist*> (i.second) != nullptr)
				rf.push_back (static_cast <PacManII::Artist*> (i.second));
		for (auto i : rf) // All are transmited to the rest...and they will decide whether to accept them, part or none...
			i -> setReferenceArtists (rf);

		_monsterActionBlocks = { };
		_numberMonstersMoving = { };
		for (auto i : actionBlocks ())
			if (dynamic_cast <PacManII::MonsterSceneActionBlock*> (i) != nullptr)
				_monsterActionBlocks.push_back (static_cast <PacManII::MonsterSceneActionBlock*> (i));
	}

	if (!_clapperBoard)
		return; // It is not still working...

	// This piece of code is to control when and who (monster) has to start the movement
	// There is a general timer
	// Any time, the timer gets the limit, the next monster is launched
	// The value of this counter is determined by the level!
	// When all monsters are moving, then this piece of code is jumped!
	if (!onOffSwitch (_SWITCHALLMONSTERSMOVING) -> isOn () && counter (_COUNTERMONSTERSTIMETOLEAVEHOME) -> isEnd ())
		onOffSwitch (_SWITCHALLMONSTERSMOVING) -> set (!launchNextMonster ());

	// Deal with the thraten state!
	if (onOffSwitch (_SWITCHMONSTERSBEINGTHREATEN) -> isOn () &&
		counter (_COUNTERMONSTERSBEINGTHREATEN) -> isEnd ())
	{
		onOffSwitch (_SWITCHMONSTERSBEINGTHREATEN) -> set (false);

		setMonstersBeingThreaten (false);

		game () -> soundBuilder () -> sound (__PACMANII_SOUNDTHREATING__) -> stop ();
	}

	// Deal with the chasing state!
	if (onOffSwitch (_SWITCHMONSTERSCHASING) -> isOn ())
	{
		if (counter (_COUNTERMONSTERSCHASING) -> isEnd ())
		{
			onOffSwitch (_SWITCHMONSTERSCHASING) -> set (false);

			setMonstersChasing (false);

			counter (_COUNTERMONSTERCHASINGCYCLES) -> isEnd (); // To add 1...

			PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
			assert (g != nullptr);
			const PacManII::DataGame::LevelDefinition::ScatterChaseCycle& sCC = 
				g -> levelDefinition (g -> level ()).scatterChaseCycle (counter (_COUNTERMONSTERCHASINGCYCLES) -> value ());
			counter (_COUNTERMONSTERSCHASING) -> initialize 
				((int) (sCC.secondsChase () * __BD g -> framesPerSecond ()), 0, true, false);
			counter (_COUNTERMONSTERSRUNNINGAWAY) -> initialize 
				((int) (sCC.secondsScatter () * __BD g -> framesPerSecond ()), 0, true, false);
		}
	}
	else
	{
		if (counter (_COUNTERMONSTERSRUNNINGAWAY) -> isEnd ())
		{
			onOffSwitch (_SWITCHMONSTERSCHASING) -> set (true);

			setMonstersChasing (true);
		}
	}
}

// ---
void PacManII::Scene::finalize ()
{
	QGAMES::Scene::finalize ();

	if (onOffSwitch (_SWITCHMONSTERSBEINGTHREATEN) -> isOn ())
		game () -> soundBuilder () -> sound (__PACMANII_SOUNDTHREATING__) -> stop ();
}

// ---
void PacManII::Scene::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __PACMANII_BALLEATEN__)
	{
		PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
		assert (g != nullptr);
		assert (evnt.data () != nullptr); // It shouldn't but just in case...

		counter (_COUNTERMONSTERSTIMETOLEAVEHOME) -> initialize (); // Starts back any time a boll is eaten...

		_totalNumberBallsEaten++; 
		_numberBallsEaten++;
		_percentageCleaned = __BD _totalNumberBallsEaten / __BD _totalNumberBallsToEat;

		playSiren (); // The sound of the siren can change...

		// If the ball eaten has power, the status of the scsne changes!
		if (static_cast <PacManII::TilePath*> (evnt.data ()) -> hasPower ()) 
		{
			onOffSwitch (_SWITCHMONSTERSBEINGTHREATEN) -> set (true);

			counter (_COUNTERMONSTERSBEINGTHREATEN) -> initialize  
				((int) (g -> levelDefinition ().secondsChasing () * __BD g -> currentLoopsPerSecond ()), 0, true, false);

			setMonstersBeingThreaten (true);

			game () -> soundBuilder () -> sound (__PACMANII_SOUNDTHREATING__) -> play (__QGAMES_GAMESOUNDCHANNEL__);
		}
	}
	else
	if (evnt.code () == __PACMANII_MONSTERSTARTEDTOMOVE__)
		_numberMonstersMoving.push_back (static_cast <PacManII::MonsterSceneActionBlock*> (evnt.data ()) -> monsterNumber ());
	else
		notify (QGAMES::Event (evnt.code (), this, evnt.values ()));

	QGAMES::Scene::processEvent (evnt);
}

// ---
__IMPLEMENTCOUNTERS__ (PacManII::Scene::Counters)
{
	// Many of them are better initialized along the game...
	addCounter (new QGAMES::Counter (_COUNTERMONSTERSCHASING, 1, 0, true, false));
	addCounter (new QGAMES::Counter (_COUNTERMONSTERSRUNNINGAWAY, 1, 0, true, false));
	addCounter (new QGAMES::Counter (_COUNTERMONSTERCHASINGCYCLES, 999999 /** Almost infinite. */, 0, true, false));
	addCounter (new QGAMES::Counter (_COUNTERMONSTERSBEINGTHREATEN, 1, 0, true, false));
	addCounter (new QGAMES::Counter (_COUNTERMONSTERSTIMETOLEAVEHOME, 1, 0, true, false));
	addCounter (new QGAMES::Counter (_COUNTERMONSTERTOLEAVE, 999999 /** Never ends. */, 0, true, false));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (PacManII::Scene::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch (_SWITCHMONSTERSCHASING, false));
	addOnOffSwitch (new QGAMES::OnOffSwitch (_SWITCHMONSTERSBEINGTHREATEN, false));
	addOnOffSwitch (new QGAMES::OnOffSwitch (_SWITCHALLMONSTERSMOVING, false));
}

// ---
void PacManII::Scene::setMonstersChasing (bool c)
{
	for (auto i : characters ())
		if (dynamic_cast <PacManII::Monster*> (i.second) != nullptr)
			(static_cast <PacManII::Monster*> (i.second)) -> toChase (c);
}

// ---
void PacManII::Scene::setMonstersBeingThreaten (bool o)
{
	for (auto i : characters ())
		if (dynamic_cast <PacManII::Monster*> (i.second) != nullptr)
			(static_cast <PacManII::Monster*> (i.second)) -> toBeThreaten (o);
}

// ---
bool PacManII::Scene::launchNextMonster ()
{
	int nMN = -1;
	do
	{
		nMN = counter (_COUNTERMONSTERTOLEAVE) -> value ();
		counter (_COUNTERMONSTERTOLEAVE) -> isEnd ();
	} while (std::find (_numberMonstersMoving.begin (), _numberMonstersMoving.end (), nMN) != _numberMonstersMoving.end ());

	bool atLeastOne = false;
	for (auto i : _monsterActionBlocks)
		atLeastOne |= i -> timeToStart (nMN /** adds one to the number of monster. */);

	return (atLeastOne);
}

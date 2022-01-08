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
	  _numberRound (0),
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
QGAMES::SetOfOpenValues PacManII::Scene::runtimeValues () const
{
	QGAMES::SetOfOpenValues result;

	result.addOpenValue (0, QGAMES::OpenValue (_numberRound));
	result.addOpenValue (1, QGAMES::OpenValue (_totalNumberBallsToEat));
	result.addOpenValue (2, QGAMES::OpenValue (_totalNumberBallsEaten));
	result.addOpenValue (3, QGAMES::OpenValue (_numberBallsEaten));
	result.addOpenValue (4, QGAMES::OpenValue (_percentageCleaned));
	result.addSetOfOpenValues (0,
		(activeMap () == nullptr) ? QGAMES::SetOfOpenValues () : activeMap () -> runtimeValues ());

	// Th information of the action blocks has no status to keep in a pacman game...

	return (result);
}

// ---
void PacManII::Scene::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	assert (cfg.existOpenValue (0) && cfg.existOpenValue (1) && 
			cfg.existOpenValue (2) && cfg.existOpenValue (3) && cfg.existOpenValue (4) &&
			cfg.existSetOfOpenValues (0));

	_numberRound = cfg.openValue (0).intValue ();
	PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
	assert (g != nullptr);
	const PacManII::DataGame::LevelDefinition& lD = g -> levelDefinition (g -> level ());
	counter (_COUNTERMONSTERSTIMETOLEAVEHOME) -> initialize 
		((int) (__BD lD.leaveHomeCondition (_numberRound).maxSecondsToLeave () * __BD g -> framesPerSecond ()), 0, true, true);

	_totalNumberBallsToEat = cfg.openValue (1).intValue ();
	_totalNumberBallsEaten = cfg.openValue (2).intValue ();
	_numberBallsEaten = cfg.openValue (3).intValue ();
	_percentageCleaned = cfg.openValue (4).bdataValue ();

	if (activeMap ())
		activeMap () -> initializeRuntimeValuesFrom (cfg.setOfOpenValues (0));
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
int PacManII::Scene::numberMonsters () const
{
	int result = 0;
	for (auto i : characters ())
		if (dynamic_cast <PacManII::Monster*> (i.second) != nullptr)
			result++;
	return (result);
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
void PacManII::Scene::stopAllElements ()
{
	for (auto i : characters ())
		if (dynamic_cast <PacManII::Artist*> (i.second))
			static_cast <PacManII::Artist*> (i.second) -> toStand ();
}

// ---
void PacManII::Scene::hideAllMonsters ()
{
	for (auto i : characters ())
		if (dynamic_cast <PacManII::Monster*> (i.second))
			static_cast <PacManII::Monster*> (i.second) -> setVisible (false);
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

	// This variable is saved, so it could different than 0 later...
	_numberRound = 0; 

	reStartAllCounters ();
	reStartAllOnOffSwitches ();

	PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
	assert (g != nullptr);
	const PacManII::DataGame::LevelDefinition& lD = g -> levelDefinition (g -> level ());
	const PacManII::DataGame::LevelDefinition::ScatterChaseCycle& sCC = 
		lD.scatterChaseCycle (counter (_COUNTERMONSTERCHASINGCYCLES) -> value ());
	counter (_COUNTERMONSTERSCHASING) -> initialize 
		((int) (sCC.secondsChase () * __BD g -> framesPerSecond ()), 0, true, false);
	counter (_COUNTERMONSTERSRUNNINGAWAY) -> initialize 
		((int) (sCC.secondsScatter () * __BD g -> framesPerSecond ()), 0, true, false);
	counter (_COUNTERMONSTERSTIMETOLEAVEHOME) -> initialize 
		((int) (__BD lD.leaveHomeCondition (_numberRound).maxSecondsToLeave () * __BD g -> framesPerSecond ()), 0, true, true);

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
	// There is a general timer defined at configuration level:
	// Any time, that timer gets the limit, the next monster is launched
	// The value of this counter is determined by the level!
	// When all monsters are moving, then this piece of code is jumped!
	// The monster scene block controls also when a monster is launched. 
	// In that case, it will depend on the number of ball eaten by pacman.
	if (!onOffSwitch (_SWITCHALLMONSTERSMOVING) -> isOn () && counter (_COUNTERMONSTERSTIMETOLEAVEHOME) -> isEnd ())
		onOffSwitch (_SWITCHALLMONSTERSMOVING) -> set (!launchNextMonster ());

	// Deal with the threaten state!
	// Is it on?
	if (onOffSwitch (_SWITCHMONSTERSBEINGTHREATEN) -> isOn ())
	{
		// Is it time to finish the state?
		if (counter (_COUNTERMONSTERSBEINGTHREATEN) -> isEnd ())
		{
			onOffSwitch (_SWITCHMONSTERSBEINGTHREATEN) -> set (false);

			onOffSwitch (_SWITCHMONSTERSBLINKING) -> set (false);

			setMonstersBlinking (false, 0 /** not used. */);

			setMonstersBeingThreaten (false);

			game () -> soundBuilder () -> sound (__PACMANII_SOUNDTHREATING__) -> stop ();
		}
		// If not...
		else
		{
			// ...and the time passed is half the total, the monsters start to blink
			if (!onOffSwitch (_SWITCHMONSTERSBLINKING) -> isOn () &&
				counter (_COUNTERMONSTERSBEINGTHREATEN) -> value () >= 
					(counter (_COUNTERMONSTERSBEINGTHREATEN) -> limitValue () >> 1))
			{
				onOffSwitch (_SWITCHMONSTERSBLINKING) -> set (true);

				setMonstersBlinking (true, 
					counter (_COUNTERMONSTERSBEINGTHREATEN) -> value () >> 3 /** 1/8 per change, 4 cycles */);
			}
		}
	}

	// Deal with the chasing state!
	// Are they?
	if (onOffSwitch (_SWITCHMONSTERSCHASING) -> isOn ())
	{
		// is it the end of the scatter-chase cycle?
		if (counter (_COUNTERMONSTERSCHASING) -> isEnd ())
		{
			onOffSwitch (_SWITCHMONSTERSCHASING) -> set (false);

			// back to the scatter situation...
			setMonstersChasing (false);

			counter (_COUNTERMONSTERCHASINGCYCLES) -> isEnd (); // To add 1 to the number of cycles excuted and

			// ...preparing the context for the next cycle...
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
	// If not...
	else
	{
		// ..is it time to start chasing?
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

	_numberRound++;
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
	addOnOffSwitch (new QGAMES::OnOffSwitch (_SWITCHMONSTERSBLINKING, false));
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
void PacManII::Scene::setMonstersBlinking (bool b, int bp)
{
	for (auto i : characters ())
		if (dynamic_cast <PacManII::Monster*> (i.second) != nullptr)
			(static_cast <PacManII::Monster*> (i.second)) -> toBlink (b, bp);
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

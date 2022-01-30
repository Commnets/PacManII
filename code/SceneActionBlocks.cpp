#include "SceneActionBlocks.hpp"
#include "Defs.hpp"

const int PacManII::ElementToAppearSceneActionBlock::_FADE [14] =
	{ 0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240, 255 };

// ---
PacManII::ElementToAppearSceneActionBlock::Properties::Properties (const QGAMES::SceneActionBlockProperties& prps)
	: _entityId (0),
	  _position (QGAMES::MazeModel::_noPosition),
	  _ballsEatenToAppear (40),
	  _secondsMaxToAppear (__BD 2.0),
	  _otherProperties (prps)
{
	if (prps.find (std::string (__PACMANII_ELMNTAPPRSCENEBLOCKENTITYIDATTR__)) != prps.end ())
		_entityId = std::atoi ((*prps.find (std::string (__PACMANII_ELMNTAPPRSCENEBLOCKENTITYIDATTR__))).second.c_str ());
	if (prps.find (std::string (__PACMANII_ELMNTAPPRSCENEBLOCKPOSITIONATTR__)) != prps.end ())
		_position = QGAMES::MazeModel::PositionInMaze 
			(QGAMES::Vector ((*prps.find (std::string (__PACMANII_ELMNTAPPRSCENEBLOCKPOSITIONATTR__))).second.c_str ()));
	if (_position == QGAMES::MazeModel::PositionInMaze (-1, -1, 0))
		_position = QGAMES::MazeModel::_noPosition;
	if (prps.find (std::string (__PACMANII_ELMNTAPPRSCENEBLOCKBALLSEATENTOAPPEARATTR__)) != prps.end ())
		_ballsEatenToAppear = std::atoi ((*prps.find (std::string (__PACMANII_ELMNTAPPRSCENEBLOCKBALLSEATENTOAPPEARATTR__))).second.c_str ());
	if (prps.find (std::string (__PACMANII_ELMNTAPPRSCENEBLOCKSECONDSTOAPPEARATTR__)) != prps.end ())
		_secondsMaxToAppear = __BD std::atof ((*prps.find (std::string (__PACMANII_ELMNTAPPRSCENEBLOCKSECONDSTOAPPEARATTR__))).second.c_str ());

	PacManII::ElementToAppearSceneActionBlock::Properties (_entityId, _position, _ballsEatenToAppear, _secondsMaxToAppear, prps);
}

// ---
QGAMES::SetOfOpenValues PacManII::ElementToAppearSceneActionBlock::runtimeValues () const
{
	QGAMES::SetOfOpenValues result;

	// counters and onOffSwitches will return the object already created that at this point could be nullptr
	result.addOpenValue (0, (onOffSwitches () == nullptr) 
		? -1 : (QGAMES::OpenValue (onOffSwitch (_SWITCHBLOCKACTIVE) -> isOn () ? 1 : 0)));
	result.addOpenValue (1, (onOffSwitches () == nullptr)
		? -1 : (QGAMES::OpenValue (onOffSwitch (_SWITCHAPPEARED) ->  isOn () ? 1 : 0)));
	// The number of cycle has to be saved to avoid the monster to appear twice in th same conditions!

	return (result);
}

// ---
void PacManII::ElementToAppearSceneActionBlock::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	assert (cfg.existOpenValue (0) && cfg.existOpenValue (1));

	if (cfg.openValue (0).intValue () != -1)
		onOffSwitch (_SWITCHBLOCKACTIVE) -> set ((cfg.openValue (0).intValue () == 1) ? true : false);
	if (cfg.openValue (1).intValue () != -1)
		onOffSwitch (_SWITCHAPPEARED) -> set ((cfg.openValue (0).intValue () == 1) ? true : false);

	counter (_COUNTERTOAPPEAR) -> initialize 
		((int) (_properties._secondsMaxToAppear * __BD game () -> framesPerSecond ()), 0, true, false);
	
	if (onOffSwitch (_SWITCHAPPEARED) -> isOn ())
		_afterAppearingActionBlock = createAfterAppearingActionBlock (); // Using the properties...
}

// ---
void PacManII::ElementToAppearSceneActionBlock::initialize ()
{
	PACMAN::SceneActionBlock::initialize ();

	_element = dynamic_cast <PACMAN::PacmanElement*> (game () -> character (_properties._entityId));
	assert (_element != nullptr);

	scene () -> addCharacter (_element);

	PACMAN::Map* aM = dynamic_cast <PACMAN::Map*> (scene () -> activeMap ());
	assert (aM != nullptr);
	_element -> setMap (aM);

	_element -> initialize ();

	_element -> setVisible (false);

	PACMAN::Scene* scn = pScene ();
	assert (scn != nullptr);
	_element -> setPositionFromMazePosition 
		((_properties._position == QGAMES::MazeModel::_noPosition) ? scn -> randomMazePosition () : _properties._position);

	observe (_element);

	reStartAllCounters ();
	reStartAllOnOffSwitches ();

	counter (_COUNTERTOAPPEAR) -> initialize 
		((int) (_properties._secondsMaxToAppear * __BD game () -> framesPerSecond ()), 0, true, false);
}

// ---
void PacManII::ElementToAppearSceneActionBlock::updatePositions ()
{
	PACMAN::SceneActionBlock::updatePositions ();

	// It is no longer active, because any reason...
	if (!onOffSwitch (_SWITCHBLOCKACTIVE) -> isOn ())
		return;

	// If the monster has already appeared...
	// the control has to be hand over to the other action block...
	// so there is nothing to do here...
	if (onOffSwitch (_SWITCHAPPEARED) -> isOn ())
		return;

	PACMAN::Scene* scn = dynamic_cast <PACMAN::Scene*> (scene ());
	assert (scn != nullptr);

	// If it is not appearing...
	if (!onOffSwitch (_SWITCHAPPEARING) -> isOn ())
	{
		// ...controls whether it is time to do so...
		// And if it is, starts from transparent status!
		if (counter (_COUNTERTOAPPEAR) -> isEnd () || 
			scn -> numberBallsEaten () >= _properties._ballsEatenToAppear)
		{
			_element -> setVisible (true);

			_element -> setAlphaLevel (0); // But it is not there...

			onOffSwitch (_SWITCHAPPEARING) -> set (true);
		}
	}
	else
	{
		if (counter (_COUNTERTOFADE) -> isEnd ())
		{
			if (counter (_COUNTERFADE) -> isEnd ())
			{
				_afterAppearingActionBlock = createAfterAppearingActionBlock ();
				assert (_afterAppearingActionBlock != nullptr);
				scene () -> addActionBlock (_afterAppearingActionBlock); // It is added for a direct control...

				onOffSwitch (_SWITCHAPPEARING) -> set (false); // No longer...

				onOffSwitch (_SWITCHAPPEARED) -> set (true);
			}
			else
				_element -> setAlphaLevel (_FADE [counter (_COUNTERFADE) -> value ()]);
		}
	}
}

// ---
void PacManII::ElementToAppearSceneActionBlock::finalize ()
{
	PACMAN::SceneActionBlock::finalize ();

	scene () -> removeActionBlock (_afterAppearingActionBlock);

	_afterAppearingActionBlock = nullptr;

	_element -> setMap (nullptr);

	scene () -> removeCharacter (_element); // It could also be already removed by the _afterAppearingActionBlock block!

	_element = nullptr;
}

// ---			
__IMPLEMENTCOUNTERS__ (PacManII::ElementToAppearSceneActionBlock::Counters)
{
	addCounter (new QGAMES::Counter (_COUNTERTOAPPEAR, 1 /** Better later. */, 0, true, false));
	addCounter (new QGAMES::Counter (_COUNTERTOFADE, 1, 0, true, true));
	addCounter (new QGAMES::Counter (_COUNTERFADE, 
		sizeof (PacManII::ElementToAppearSceneActionBlock::_FADE) / sizeof (int), 0, true, false));
}

__IMPLEMENTONOFFSWITCHES__ (PacManII::ElementToAppearSceneActionBlock::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch (_SWITCHBLOCKACTIVE, false));
	addOnOffSwitch (new QGAMES::OnOffSwitch (_SWITCHAPPEARING, false));
}

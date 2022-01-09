#include "StandardScenes.hpp"
#include "Maps.hpp"
#include "Game.hpp"
#include "SceneActionBlocks.hpp"

// ---
void PacManII::StandardScene::initialize ()
{
	// Loads the pacman...
	addCharacter (_pacman = dynamic_cast <PacManII::PacMan*> (game () -> character (__PACMANII_PACMANBASEENTITYID__)));
	_pacman -> setMap (activeMap ());

	PacManII::Scene::initialize ();

	_pacman -> setCurrentState (__PACMANII_PACMANSTATESTANDLOOKINGRIGHT__);

	_pacman -> setVisible (true);

	PacManII::Map* aM = dynamic_cast <PacManII::Map*> (activeMap ());
	assert (aM != nullptr);
	_pacman -> setPosition (aM -> mazePositionToMapPosition (aM -> pacmanInitialPosition (0 /** It can be more. */)) - 
		QGAMES::Vector (__BD (_pacman -> visualLength () >> 1), __BD (_pacman -> visualHeight () >> 1), __BD 0));

	_numberMonstersEaten = 0;
}

// ---
void PacManII::StandardScene::updatePositions ()
{
	PacManII::Scene::updatePositions ();

	// Regarding the status of the map...
	if (_percentageCleaned == __BD 1)
		notify (QGAMES::Event (__PACMANII_PACMANREACHEDGOAL__, _pacman));

	// To chase or not...(depends on what monsters do)
	_pacman -> toChase (onOffSwitch (_SWITCHMONSTERSBEINGTHREATEN) -> isOn ());

	// When monsters are not longer threaten, the list of monsters eaten is cleaned
	if (!onOffSwitch (_SWITCHMONSTERSBEINGTHREATEN) -> isOn ())
		_numberMonstersEaten = 0;
}

// ---
void PacManII::StandardScene::finalize ()
{
	PacManII::Scene::finalize ();

	_pacman -> setMap (nullptr);

	removeCharacter (_pacman);

	_pacman = nullptr;
}

// ---
void PacManII::StandardScene::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __PACMANII_PACMANEATMONSTER__)
	{
		PacManII::Monster* mtr = static_cast <PacManII::Monster*> (evnt.data ());

		// As the number of monsters eaten is bigger, the number of points grow (double)
		_pacman -> setScore (_pacman -> score () + (mtr -> points () << (_numberMonstersEaten++)));
		if (_numberMonstersEaten == numberMonsters ())
			_pacman -> setScore (_pacman -> score () + 12000); // Additional!!
	}

	PacManII::Scene::processEvent (evnt);
}

// ---
PacManII::BasicScene::BasicScene (int c, const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn, 
		const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
	: StandardScene (c, m, cn, p, ePL)
{  
	// Action blocks to control the monsters...
	// 200 points when it is the first in being eaten. The rest double the points of th previous!
	addActionBlock (new PacManII::MonsterSceneActionBlock 
		(0, PacManII::MonsterSceneActionBlock::Properties (__PACMANII_BLINKYBASEENTITYID__, PacManII::Blinky::_NUMBER, 200)), false);
	addActionBlock (new PacManII::MonsterSceneActionBlock 
		(1, PacManII::MonsterSceneActionBlock::Properties (__PACMANII_PINKYBASEENTITYID__, PacManII::Pinky::_NUMBER, 200)), false);
	addActionBlock (new PacManII::MonsterSceneActionBlock 
		(2, PacManII::MonsterSceneActionBlock::Properties (__PACMANII_INKYBASEENTITYID__, PacManII::Inky::_NUMBER, 200)), false);
	addActionBlock (new PacManII::MonsterSceneActionBlock 
		(3, PacManII::MonsterSceneActionBlock::Properties (__PACMANII_CLYDEBASEENTITYID__, PacManII::Clyde::_NUMBER, 200)), false);
	addActionBlock (new PacManII::FruitSceneActionBlock (4, PacManII::FruitSceneActionBlock::Properties ()), false);
	// The parameters will be later adjusted to the level...
}

// ---
void PacManII::BasicScene::initialize ()
{
	PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
	assert (g != nullptr);

	std::vector <int> bS, bP, bE;
	std::vector <QGAMES::bdata> sD;
	for (auto i : g -> levelDefinition (g -> level ()).fruitConditions ())
	{
		bS.push_back (i.bonusSymbolId ());
		bP.push_back (i.bonusPoints ());
		bE.push_back (i.numberBallsEatenToAppear ());
		sD.push_back (__BD i.secondsBonusToDisappear ());
	}

	// The properties have to be change before initializing fro them to take real effect
	dynamic_cast <PacManII::FruitSceneActionBlock*> (actionBlock (4)) -> setProperties 
			(PacManII::FruitSceneActionBlock::Properties (__PACMANII_FRUITBASEENTITYID__, bS, bP, bE, sD));

	PacManII::StandardScene::initialize ();
}

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
PacManII::BasicScene::BasicScene (int c, const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn, 
		const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
	: StandardScene (c, m, cn, p, ePL)
{  
	// Action blocks to control the monsters...
	addActionBlock (new PacManII::MonsterSceneActionBlock 
		(0, PacManII::MonsterSceneActionBlock::Properties 
			(__PACMANII_INKYBASEENTITYID__, PacManII::Inky::_NUMBER, 100, -0.0, 0.0)), false);
	addActionBlock (new PacManII::MonsterSceneActionBlock 
		(1, PacManII::MonsterSceneActionBlock::Properties 
			(__PACMANII_BLINKYBASEENTITYID__, PacManII::Blinky::_NUMBER, 100, -0.0, 0.0)), false);
	addActionBlock (new PacManII::MonsterSceneActionBlock 
		(2, PacManII::MonsterSceneActionBlock::Properties 
			(__PACMANII_PINKYBASEENTITYID__, PacManII::Pinky::_NUMBER, 100, -0.0, 0.0)), false);
	addActionBlock (new PacManII::MonsterSceneActionBlock 
		(3, PacManII::MonsterSceneActionBlock::Properties 
			(__PACMANII_CLYDEBASEENTITYID__, PacManII::Clyde::_NUMBER, 100, -0.0, 0.0)), false);
}

// ---
void PacManII::BasicScene::initialize ()
{
	PacManII::StandardScene::initialize ();

	// Action blocks to control the fruit...
	// It has to added every time the scene is intialized, becasue aspect and bonus depends on the level...
	PacManII::Game* g = dynamic_cast< PacManII::Game*> (game ());
	assert (g != nullptr);
	assert (!existsActionBlock (4));
	addActionBlock (new PacManII::FruitSceneActionBlock 
		(4, PacManII::FruitSceneActionBlock::Properties (
			__PACMANII_FRUITBASEENTITYID__, 
			g -> levelDefinition (g -> level ()).bonusSymbolId (),
			g -> levelDefinition (g -> level ()).bonusPoints (),
			__BD g -> levelDefinition (g -> level ()).secondsBonusSymbolToAppear (), 
			__BD g -> levelDefinition (g -> level ()).secondsBonusSymbolToDisappear ())));
}

// ---
void PacManII::BasicScene::finalize ()
{
	// It has to be removed as it's been added when initialize...
	removeActionBlock (4);

	PacManII::StandardScene::finalize ();
}

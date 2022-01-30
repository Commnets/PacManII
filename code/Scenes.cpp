#include "Scenes.hpp"
#include "Scenes.hpp"
#include "SceneActionBlocks.hpp"
#include "Monsters.hpp"
#include "Defs.hpp"

// ---
PacManII::ExtendedScene::ExtendedScene (int c, const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn, 
			const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
	: PACMAN::BasicScene (c, m, cn, p, ePL)
{
	addActionBlock (new PacManII::MonsterToAppearAndMoveSceneActionBlock (100,
		PacManII::ElementToAppearSceneActionBlock::Properties ()), false); // It will be initialized later...
}

// ---
void PacManII::ExtendedScene::initialize ()
{
	dynamic_cast <PacManII::MonsterToAppearAndMoveSceneActionBlock*> (actionBlock (100)) -> setProperties 
		(PacManII::ElementToAppearSceneActionBlock::Properties 
			(__PACMANII_WORMYBASEENTITYID__, QGAMES::MazeModel::_noPosition /** It will mean random. */, 10, __BD 7, 
				{ { std::string ("ENTITYID"), std::to_string (__PACMANII_WORMYBASEENTITYID__) }, 
				  { std::string ("MONSTERNUMBER"), std::to_string (PacManII::Wormy::_NUMBER) },
				  { std::string ("POINTS"), std::string ("600") }
				}));

	PACMAN::BasicScene::initialize ();
}

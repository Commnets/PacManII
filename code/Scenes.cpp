#include "Scenes.hpp"
#include "Scenes.hpp"
#include "SceneActionBlocks.hpp"
#include "Monsters.hpp"
#include "Game.hpp"
#include "DataGame.hpp"
#include "Defs.hpp"

// ---
PacManII::ExtendedScene::ExtendedScene (int c, const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn, 
			const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
	: PACMAN::BasicScene (c, m, cn, p, ePL),
	  _monsterActionBlocksAdded ()
{
	// Nothing else to do...
}

// ---
void PacManII::ExtendedScene::initialize ()
{
	PacManII::Game* g = dynamic_cast <PacManII::Game*> (game ());
	assert (g != nullptr);
	const PACMAN::DataGame::LevelDefinition& lDef = g -> levelDefinition (g -> level ());

	if (dynamic_cast <const PacManII::DataGame::LevelDefinition*> (&lDef) != nullptr)
	{
		const PacManII::DataGame::LevelDefinition& nLDef = 
			*static_cast <const PacManII::DataGame::LevelDefinition*> (&lDef);
		if (!nLDef.additionalMonsters ().empty ())
		{
			int ct = 0;
			for (auto i : nLDef.additionalMonsters ())
			{
				QGAMES::SceneActionBlock* mABlk = new PacManII::WormyMonsterSceneActionBlock (100 + (ct << 1),
					PacManII::WormyMonsterSceneActionBlock::Properties 
						(PACMAN::MonsterSceneActionBlock::Properties (false, i.monsterId (), i.monsterNumber (), i.points ()), 
						 i.trailLength ()));
				QGAMES::SceneActionBlock* mABlkCtrl = new PacManII::ElementToAppearSceneActionBlock (101 + (ct << 1),
					PacManII::ElementToAppearSceneActionBlock::Properties 
						(i.monsterId (), QGAMES::MazeModel::_noPosition, 
							i.numberBallsEatenToAppear (), i.maxSecondsToAppear (), 100 + (ct << 1)));

				_monsterActionBlocksAdded.push_back (mABlk);
				_monsterActionBlocksAdded.push_back (mABlkCtrl);
				// Control is added later always, to control the destruction of element moved... 

				addActionBlock (mABlk, false); 
				addActionBlock (mABlkCtrl, false);

				ct++;
			}
		}
	}

	PACMAN::BasicScene::initialize ();
}

// ---
void PacManII::ExtendedScene::finalize ()
{
	for (auto i : _monsterActionBlocksAdded)
		removeActionBlock (i);
	_monsterActionBlocksAdded = { };

	PACMAN::BasicScene::finalize ();
}

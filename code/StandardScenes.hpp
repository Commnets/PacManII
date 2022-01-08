/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: StandardScenes.hpp \n
 *	Game: PacManII \n
 *	Author: Ignacio Cea Forniés (Community Networks) \n
 *	Creation Date: 22/08/2021 \n
 *	Description: To manipulate the standard scenes of the game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_STANDARDSCENES__
#define __PACMANII_STANDARDSCENES__

#include "Defs.hpp"
#include "Worlds.hpp"
#include "Scenes.hpp"
#include "Artists.hpp"
#include "Maze.hpp"
#include <advancedArcade/adinclude.hpp>

namespace PacManII
{
	/** 
	  * The standard scene only considers the single pacman. \n
	  * The rest of elments would have to be defined as external actionblocks in the XML file or
	  * specifically inheriting from this one, that can be overloaded.
	  * That scene control when pacman is in chasing modee taking into account the situation of the monsters. \n
	  */
	class StandardScene : public Scene
	{
		public:
		StandardScene (int c, const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: Scene (c, m, cn, p, ePL),
			  _pacman (nullptr),
			  _numberMonstersEaten (0)
							{ }

		virtual void initialize () override;
		virtual void updatePositions () override;
		virtual void finalize () override;

		virtual void processEvent (const QGAMES::Event& evnt) override;

		private:
		PacMan* _pacman;

		// Implementation
		int _numberMonstersEaten;
	};

	/**
	  * The very basic Scene, is a standard one but defining the monsters and the fruit scene action blocks
	  * in the constrctor of the class, avoiding so their definition in the scene xml file. \n
	  * It can't be extended.
	  */
	class BasicScene final : public StandardScene
	{
		public:
		BasicScene (int c, const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ());

		virtual void initialize () override;
		virtual void finalize () override;
	};
}

#endif
  
// End of the file
/*@}*/
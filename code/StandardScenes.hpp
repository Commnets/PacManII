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
	/** The standard scene only considers the single pacman. 
		The rest of elments would have to define the rest of info as external actionblocks in the XML file. */
	class StandardScene : public Scene
	{
		public:
		StandardScene (int c, const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: Scene (c, m, cn, p, ePL),
			  _pacman (nullptr)	
							{ }

		virtual void initialize () override;
		virtual void updatePositions () override;
		virtual void finalize () override;

		private:
		PacMan* _pacman;
	};

	/** The very basic Scene, is a standard one but defining the monsters and the fruit scenee action blocks
		in the constrctor of the class, avoinding so their definition in the xml file. */
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
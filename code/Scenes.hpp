/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: Scenes.hpp \n
 *	Game: PacManII \n
 *	Author: Ignacio Cea Forniés (Community Networks) \n
 *	Creation Date: 29/01/2021 \n
 *	Description: Define the scenes of this version of the game PacManII
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_SCENES__
#define __PACMANII_SCENES__

#include <PacManLike/pcinclude.hpp>

namespace PacManII
{
	/** The extended scene, adds a specific block for monsters to appear. */
	class ExtendedScene final : public PACMAN::BasicScene
	{
		public:
		ExtendedScene (int c, const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ());

		virtual void initialize () override;
	};
}

#endif
  
// End of the file
/*@}*/
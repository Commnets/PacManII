/** \ingroup Example */
/*@{*/

/**	
 *	@file	
 *	File: Defs.hpp \n
 *	Framework: PacmanII Game \n
 *	Author: Ignacio Cea Forniés (Community Networks) \n
 *	Creation Date: 21/01/2022 \n
 *	Description: Defines and general includes of PacmanII
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_PACMANGAMEDEFINES__
#define __PACMANII_PACMANGAMEDEFINES__

#include <PacManLike/pcdefs.hpp>

// Sounds
#define __PACMANII_ENERGYAROUNDPACMAN__				60000
#define __PACMANII_SOUNDSHIELDCAUGHT__				60001

// The entities
// Wormy
#define __PACMAN_WORMYBASEENTITYID__				60000
#define __PACMANII_WORMYMAZEMOVEMENTBASEID__		600000		
#define __PACMANII_WORMYATHOMEMOVEMENTBASEID__		610000
// Shield
#define __PACMAN_SHIELDBASEENTITYID__				60010
#define __PACMAN_SHIELDSTAYINGMOVEMENTBASEID__		600010
// The types of shields
#define __PACMANII_NUMBERTYPESSHIELD__				3
#define __PACMANII_SHIELDTYPE1__					0
#define __PACMANII_SHIELDTYPE2__					1
#define __PACMANII_SHIELDTYPE3__					2

// Events
#define __PACMANII_SHIELDCAUGHT__					60000

#endif
  
// End of the file
/*@}*/
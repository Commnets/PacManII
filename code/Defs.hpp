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
#define __PACMANII_ENERGYAROUNDPACMAN__							60000
#define __PACMANII_SOUNDSHIELDCAUGHT__							60001

// The entities
// Wormy
#define __PACMANII_WORMYBASEENTITYID__							60000
#define __PACMANII_WORMYMAZEMOVEMENTBASEID__					600000		
#define __PACMANII_WORMYATHOMEMOVEMENTBASEID__					610000
// Shield
#define __PACMANII_SHIELDBASEENTITYID__							60010
#define __PACMANII_SHIELDSTAYINGMOVEMENTBASEID__				600010
// The types of shields
#define __PACMANII_NUMBERTYPESSHIELD__							3
#define __PACMANII_SHIELDTYPE1__								0
#define __PACMANII_SHIELDTYPE2__								1
#define __PACMANII_SHIELDTYPE3__								2

// The scene blocks and it properties
// Element to appear...
#define __PACMANII_ELMNTAPPRSCENEBLOCKENTITYIDATTR__			"ENTITYID"
#define __PACMANII_ELMNTAPPRSCENEBLOCKPOSITIONATTR__			"POSITION"
#define __PACMANII_ELMNTAPPRSCENEBLOCKBALLSEATENTOAPPEARATTR__	"BALLESTOAPPEAR"
#define __PACMANII_ELMNTAPPRSCENEBLOCKSECONDSTOAPPEARATTR__		"SECONDSTOAPPEAR"
// Monster to appear and then move
#define __PACMANII_MONSTERAPPRANDMOVESCENEBLOCKBASEID__			12000
#define __PACMANII_MONSTERAPPRANDMOVESCENEBLOCKNUMBEER__		1000

// Events
// Related with the shields..
#define __PACMANII_SHIELDCAUGHT__								60000

// The World and Scenes used for PacManII
#define __PACMANII_EXTENDEDWORLD__								60000
#define __PACMANII_EXTENDEDSCENE__								60000
#define __PACMANII_EXTENDEDMAP__								60000
#define __PACMANII_EXTENDEDSCENENUMBER__						1000

#endif
  
// End of the file
/*@}*/
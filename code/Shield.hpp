/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: Shield.hpp \n
 *	Game: PacManII \n
 *	Author: Ignacio Cea Forniés (Community Networks) \n
 *	Creation Date: 22/01/2022 \n
 *	Description: To protect pacman!. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_SHIELD__
#define __PACMANII_SHIELD__

#include <PacManLike/pcinclude.hpp>

namespace PacManII
{
	/** The Shield. It has been done to protect pacman!. \n
		@see Thing documentation for more details. */
	class Shield : public PACMAN::Thing
	{
		public:
		Shield (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: PACMAN::Thing (cId, f, d)
							{ }

		virtual QGAMES::Entity* clone () const override
							{ return (new Shield (id (), forms (), data ())); }

		virtual void setType (int t) override;
	};
}

#endif
  
// End of the file
/*@}*/
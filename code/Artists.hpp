/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: Artists.hpp \n
 *	Game: PacManII \n
 *	Author: Ignacio Cea Forniés (Community Networks) \n
 *	Creation Date: 22/08/2021 \n
 *	Description: PacmanII artists main definition file. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_ARTISTS__
#define __PACMANII_ARTISTS__

#include "Defs.hpp"
#include <advancedArcade/adinclude.hpp>

namespace PacManII
{
	/** All artists in pacman moves in the same way. */
	class Artist : public QGAMES::ArtistInATiledMap
	{
		public:
		Artist () = delete;

		Artist (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: QGAMES::ArtistInATiledMap (cId, f, d)
							{ }

		Artist (const Artist&) = delete;

		Artist& operator = (const Artist&) = delete;

		virtual QGAMES::SetOfOpenValues runtimeValues () const override;
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& oV) override;

		virtual bool isAlive () const
							{ return (true); }
	};

	/** What the player controls. */
	class PacMan final : public Artist
	{
		public:
		PacMan (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: Artist (cId, f, d),
			  _score (0),
			  _counter (0)
							{ }

		virtual QGAMES::SetOfOpenValues runtimeValues () const override;
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& oV) override;

		virtual Entity* clone () const override
							{ return (new PacMan (id (), forms (), data ())); }

		int score () const
							{ return (_score); }
		void setScore (int s); 

		void toMove (const QGAMES::Vector& d);
		void toShoot (int f, const QGAMES::Vector& d);

		virtual void initialize () override;
		virtual void updatePositions () override;
		virtual void finalize () override;

		private:
		int _score;

		// Implementation
		int _counter;
	};

	/** What the machine controls. */
	class Monster : public Artist
	{
		public:
		Monster (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: Artist (cId, f, d)
							{ }

		virtual void initialize () override;
	};

	/** Inky, the blue/cyan monster. Bashful. */
	class Inky final : public Monster
	{
		public:
		Inky (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
			const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: Monster (cId, f, d)
							{ }

		virtual Entity* clone () const override
							{ return (new Inky (id (), forms (), data ())); }
	};

	/** Blinky, the red monster. Your shadow. */
	class Blinky final : public Monster
	{
		public:
		Blinky (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
			const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: Monster (cId, f, d)
							{ }

		virtual Entity* clone () const override
							{ return (new Blinky (id (), forms (), data ())); }
	};

	/** Pinky, the pink monster. Pure speed. */
	class Pinky final : public Monster
	{
		public:
		Pinky (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: Monster (cId, f, d)
							{ }

		virtual Entity* clone () const override
							{ return (new Pinky (id (), forms (), data ())); }
	};

	/** Clyde, the orange monster. Just the opposite: pokey*/
	class Clyde final : public Monster
	{
		public:
		Clyde (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: Monster (cId, f, d)
							{ }

		virtual Entity* clone () const override
							{ return (new Clyde (id (), forms (), data ())); }
	};
}

#endif
  
// End of the file
/*@}*/
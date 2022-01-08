/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: Worlds.hpp \n
 *	Game: PacManII \n
 *	Author: Ignacio Cea Forniés (Community Networks) \n
 *	Creation Date: 22/08/2021 \n
 *	Description: To manipulate the worlds of the game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_WORLDS__
#define __PACMANII_WORLDS__

#include "Defs.hpp"
#include "Artists.hpp"
#include "Maze.hpp"
#include <advancedArcade/adinclude.hpp>

namespace PacManII
{
	/** Basic world of the game. 
		Scenes can be only PacManII ones. 
		The game can be extended in terms of worlds as well. */
	class World : public QGAMES::World
	{
		public:
		World (int c, const QGAMES::Scenes& s, const QGAMES::WorldProperties& p = QGAMES::WorldProperties ());

		virtual QGAMES::SetOfOpenValues runtimeValues () const override;
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg) override
							{ _fullConfPerPlayer = cfg; }

		void setClapperBoard (bool o);
		bool clapperBoard () const;

		/** To blink. */
		void startBlinking (QGAMES::bdata bT, int nB);
		bool isBlinking () const;
		void stopBlinking ();

		/** To manage the ball status of the world. */
		int maxNumberBallsToEat () const;
		int numberBallsEaten () const;
		std::string ballsEatenStatus () const;
		void setBallsEatenStatus (const std::string& st);

		/** To play / stop the siren. */
		void playSiren (bool f = false);
		void stopSiren ();

		/** To stop all elments in pacman. */
		void stopAllElements ();
		/** To hide monsters. */
		void hideAllMonsters ();

		virtual void initialize () override;
		virtual void finalize () override;

		/** The events comming from the scene observed are transmitted up. */ 
		virtual void processEvent (const QGAMES::Event& evnt) override
							{ notify (QGAMES::Event (evnt.code (), evnt.data (), evnt.values ())); }

		protected:
		/** 
		  * The structure of the runtimeValues hasn't take into account many players. 
		  *	To this method just return the current information. 
		  *	This method invokes the runtimeValues of the parent.
		  * But it can be overloaded to add additional infomation 
		  * in the same way usually is done with runtimeValues (). 
		  */
		QGAMES::SetOfOpenValues currentPlayerRuntimeValues () const
							{ return (QGAMES::World::runtimeValues ()); }

		protected:
		mutable QGAMES::SetOfOpenValues _fullConfPerPlayer;
	};
}

#endif
  
// End of the file
/*@}*/
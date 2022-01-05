/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: PacMan.hpp \n
 *	Game: PacManII \n
 *	Author: Ignacio Cea Forniés (Community Networks) \n
 *	Creation Date: 22/08/2021 \n
 *	Description: Definition of the main artist = pacman. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_PACMAN__
#define __PACMANII_PACMAN__

#include "Defs.hpp"
#include "Artists.hpp"
#include <advancedArcade/adinclude.hpp>

namespace PacManII
{
	/** What the player controls. */
	class PacMan final : public Artist
	{
		public:
		enum class Status 
			{ _NOTDEFINED = 0, _STOPPED = 1, _MOVING = 2, _CHASING = 3 };

		PacMan (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ());

		virtual Entity* clone () const override
							{ return (new PacMan (id (), forms (), data ())); }

		virtual void toStand () override;
		virtual void toMove (const QGAMES::Vector& d) override;

		// Additional behaviour of pacman...
		void toChase (bool c);
		void toChaseDeferred (bool c);

		virtual bool isEnemy (const PacmanElement* elmnt) const override;

		virtual bool isAlive () const override
							{ return (_alive); }
		void setAlive (bool a)
							{ _alive = a; }
		virtual bool isStanding () const override
							{ return (_status == Status::_STOPPED); }
		virtual bool isMoving () const override
							{ return (_status == Status::_MOVING || _status == Status::_CHASING); }

		int score () const
							{ return (_score); }
		void setScore (int s);

		const std::map <int, bool> fruitsEaten () const
							{ return (_fruitsEaten); }
		void setFruitsEaten (const std::map <int, bool>& fE);
		void addFruitEaten (int fE)
							{ _fruitsEaten [fE] = true; setFruitsEaten (_fruitsEaten); }

		// To know the internal status...
		Status status () const
							{ return (_status); }

		virtual void initialize () override;
		virtual void updatePositions () override;
		virtual void finalize () override;

		virtual void processEvent (const QGAMES::Event& evnt) override;

		virtual void whenCollisionWith (QGAMES::Entity* e) override;

		protected:
		void setStatus (const Status& st);

		/** To know the last status. */
		Status lastStatus () const
							{ return (_lastStatus); }

		/** The target position is the limit of the maze in the current direction of the movement. */
		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const override;

		virtual void whatToDoWhenStopStatusIsRequested (const QGAMES::Vector& d) override;
		virtual void whatToDoWhenMovementStatusIsRequested (const QGAMES::Vector& d) override;
		virtual void whatToDoOnCurrentPosition () override;
		virtual void setStateToStandLookingTo (const QGAMES::Vector& d) override;
		virtual void setStateToMoveTo (const QGAMES::Vector& d) override;

		/** Define a buoy to chase within the inEveryLoop method. */
		class ToChaseBuoy final : public QGAMES::Buoy
		{
			public:
			ToChaseBuoy ()
				: QGAMES::Buoy (__PACMANII_TOCHASEBUOYID__, (QGAMES::bdata) 0)
							{ /** Nothing else to do. */ }

			void setChasing (bool c)
							{ _chasing = c; }

			virtual void* treatFor (QGAMES::Element* e) override;

			private:
			bool _chasing;
		};

		// Implementation
		/** To adapt the speed of the pacman when moving. */
		void adaptSpeed ();
		/** It is a little bit different that the standard on as the movements made by the player 
			in the joystick or keyboard have to be taken into account. */
		virtual QGAMES::MazeModel::PathInMaze& recalculatePathInMaze 
			(const QGAMES::Vector& mD = QGAMES::Vector::_noPoint) override;

		private:
		/** To indicate whether the pacman is alive. */
		bool _alive;
		/** The score of the pacman. It is get and put back into the game. */
		int _score;
		/** The fruits eaten. */
		std::map <int, bool> _fruitsEaten;
		/** The number of points associated to this monster. */
		/** The status of the pacman. */
		Status _status;
		/** The last status of pacman. */
		Status _lastStatus;

		// Implementation
		/** To indicate whether the pacman has eaten or not something. */
		bool _hasEaten;
		/** The last score notified. */
		int _lastMulScoreNotified;
	};
}

#endif
  
// End of the file
/*@}*/
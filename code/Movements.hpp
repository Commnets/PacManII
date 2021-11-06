/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: Movements.hpp \n
 *	Game: PacManII \n
 *	Author: Ignacio Cea Forniés (Community Networks) \n
 *	Creation Date: 30/10/2021 \n
 *	Description: To manipulate the movements of the game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_MOVEMENTS__
#define __PACMANII_MOVEMENTS__

#include "Defs.hpp"

#include <AdvancedArcade/adinclude.hpp>

namespace PacManII
{
	/** Movements in th maze are similar for both pacman and monsters. */
	class MazeMovement : public QGAMES::Movement
	{
		public:
		MazeMovement () = delete;

		MazeMovement (int id, ::std::map <int, double> v)
			: QGAMES::Movement (id, v),
			  _currentMovDirection (QGAMES::Vector::_cero),
			  _nextMovDirection (QGAMES::Vector::_cero),
			  _position (QGAMES::Position::_cero),
			  _firstMovement (true)
							{ }

		MazeMovement (const MazeMovement&) = delete;

		const MazeMovement& operator = (const MazeMovement&) = delete;

		virtual Movement* clone () const
							{ return (new MazeMovement (_id, _variables)); }

		virtual QGAMES::Vector direction () const override
							{ return (_currentMovDirection); }
		virtual QGAMES::Vector acceleration () const override
							{ return (QGAMES::Vector::_cero); }

		/** To set the speed. 
			This parameter comes from the configuration of the game. */
		QGAMES::bdata speed () const
							{ return (_inc); }
		void setSpeed (QGAMES::bdata s)
							{ _inc = s; }

		virtual void initialize () override;
		virtual void initializeFrom (const QGAMES::Movement* m) override;
		virtual void move (const QGAMES::Vector& d, const QGAMES::Vector& a, QGAMES::Entity* e) override;

		protected:
		/** The current direction of the movement. */
		QGAMES::Vector _currentMovDirection;
		/** The next direction of the movement. 
			Whn the limit of the previous was reached. */
		QGAMES::Vector _nextMovDirection; // It could be cero, meaning no movement after reaching limit
		/** The inc in the movement, depending on the speed. */
		QGAMES::bdata _inc;

		// Implementation
		QGAMES::Position _position;
		bool _firstMovement;
	};
}

#endif
  
// End of the file
/*@}*/

/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: Fruit.hpp \n
 *	Game: PacManII \n
 *	Author: Ignacio Cea Forniés (Community Networks) \n
 *	Creation Date: 22/08/2021 \n
 *	Description: Such important element in any pacman game!. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_FRUIT__
#define __PACMANII_FRUIT__

#include "Defs.hpp"
#include "Artists.hpp"
#include <advancedArcade/adinclude.hpp>

namespace PacManII
{
	/** The fruits in the maze. \n
		@see PacmanElement documentation for more details. */
	class Fruit : public PacmanElement
	{
		public:
		// The different status a fruit can be in...
		enum class Status 
			{ _NOTDEFINED = 0, _SHOWN = 1, _EATEN = 2 };

		Fruit (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: PacmanElement (cId, f, d),
			  _type (0), _points (200),
			  _status (Status::_NOTDEFINED)
							{ }

		virtual Entity* clone () const override
							{ return (new Fruit (id (), forms (), data ())); }

		virtual bool isEnemy (const PacmanElement* elmnt) const override
							{ return (false); }
		virtual bool isAlive () const override
							{ return (_status == Status::_SHOWN); }
		virtual bool isStanding () const override
							{ return (true); }
		virtual bool isMoving () const override
							{ return (false); }

		/** To know and change the type of fruit. */
		int type () const
							{ return (_type); }
		void setType (int t);

		/** To set and know the number of points assigned to this fruit.
			Those points should be given to the one eatening it. */
		int points () const
							{ return (_points); }
		void setPoints (int p)
							{ _points = p; }

		/** To know and change the status */
		void setStatus (const Status& st);
		Status status () const
							{ return (_status); }

		virtual void initialize () override;
		virtual void updatePositions () override;
		virtual void drawOn (QGAMES::Screen* scr, const QGAMES::Position& p = QGAMES::Position::_noPoint) override;

		virtual void whenCollisionWith (QGAMES::Entity* e) override;

		private:
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters () override
							{ return (new Counters); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches)
		virtual QGAMES::OnOffSwitches* createOnOffSwitches () override
							{ return (new OnOffSwitches); }

		private:
		int _type;
		int _points;
		Status _status;

		// The counters and the switches
		static const int _COUNTERPOINTSVISIBLE = 0;
		static const int _SWITCHPOINTSVISIBLE = 0;
	};
}

#endif
  
// End of the file
/*@}*/
/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: Monsters.hpp \n
 *	Game: PacManII \n
 *	Author: Ignacio Cea Forniés (Community Networks) \n
 *	Creation Date: 22/08/2021 \n
 *	Description: Definition of the monsters of the game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_MONSTERS__
#define __PACMANII_MONSTERS__

#include "Defs.hpp"
#include "Artists.hpp"
#include <advancedArcade/adinclude.hpp>

namespace PacManII
{
	/** What the machine controls. */
	class Monster : public Artist
	{
		public:
		// The different status a monster cab ne in...
		enum class Status 
			{ _NOTDEFINED = 0, _ATHOME = 1, _EXITINGHOME = 2, _CHASING = 3, _RUNNINGWAY = 4, _TOBEEATEN = 5, _BEINGEATEN = 6 };

		Monster (int cId, int mN, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ());

		virtual bool isEnemy (const PacmanElement* elmnt) const override;

		/** The number of the monster. */
		int monsterNumber () const
							{ return (_monsterNumber); }

		/** The points associated to the monster. */
		int points () const
						{ return (_points); }
		void setPoints (int p)
							{ _points = p; }

		virtual void toStand () override;
		virtual void toMove (const QGAMES::Vector& d) override;
		void toChase (bool f);
		void toBeThreaten (bool f);

		virtual bool isAlive () const override
							{ return (_status != Status::_BEINGEATEN); }
		virtual bool isStanding () const override
							{ return (_status == Status::_NOTDEFINED || _status == Status::_ATHOME); }
		virtual bool isMoving () const override
							{ return (!isStanding ()); }
		bool isDangerous () const
							{ return (_status == Status::_CHASING || _status == Status::_RUNNINGWAY); }

		// To know the status...
		Status status () const
							{ return (_status); }

		virtual void initialize () override;

		virtual void processEvent (const QGAMES::Event& evnt) override;

		virtual void whenCollisionWith (QGAMES::Entity* e) override;

		protected:
		void setStatus (const Status& st);

		virtual void whatToDoWhenStopStatusIsRequested (const QGAMES::Vector& d) override;
		virtual void whatToDoWhenMovementStatusIsRequested (const QGAMES::Vector& d) override;
		virtual void whatToDoOnCurrentPosition () override
							{ }

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

		/** The method to stop deferred. */
		void toChaseDeferred (bool o);

		// Implementation
		virtual void adaptSpeed () = 0;
		/** The monsters can not enter back their home unless they have been eaten!. */
		virtual QGAMES::MazeModel::PathInMaze& recalculatePathInMazeAvoiding (const std::vector <QGAMES::Vector>& d) override;
		/** Where to run. */
		inline QGAMES::MazeModel::PositionInMaze runAwayMazePosition () const;

		protected:
		/** The number of the monster. */
		int _monsterNumber;
		/** The points associated to the monster. */
		int _points;
		/** The status of the monster. */
		Status _status;
	};

	/** The standard monster always pursuit a pacman. */
	class StandardMonster : public Monster
	{
		public:
		StandardMonster (int cId, int mN, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: Monster (cId, mN, f, d)
							{ }

		/** By default the referenced artist is pacman only */
		virtual void setReferenceArtists (const std::vector <Artist*>& r) override;

		/** The speed within the tunnel path has to b taken into account. */
		virtual void updatePositions () override;

		protected:
		/** The tager position in a standard monster for most of the internal status except chasing is calculated in the same way. \
			Just only the target position when chasing will depend on the specific monster. */
		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const override;

		virtual void setStateToStandLookingTo (const QGAMES::Vector& d) override;
		virtual void setStateToMoveTo (const QGAMES::Vector& d) override;

		// Implementation
		virtual void adaptSpeed () override;
	};

	/** Inky, the blue/cyan monster. Bashful. */
	class Inky final : public StandardMonster
	{
		public:
		static const int _NUMBER = 0;

		Inky (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
			const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: StandardMonster (cId, _NUMBER, f, d)
							{ }

		virtual Entity* clone () const override
							{ return (new Inky (id (), forms (), data ())); }

		/** Inky has blinky and pacman as references. */
		virtual void setReferenceArtists (const std::vector <Artist*>& r) override;

		protected:
		/** When chasing Inky's target position is twice the distance between the 2 next positions of pacman and blinky's 
			In other circunstance is the run away position. */
		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const override;
	};

	/** Blinky, the red monster. Your shadow. */
	class Blinky final : public StandardMonster
	{
		public:
		static const int _NUMBER = 1;

		Blinky (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
			const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: StandardMonster (cId, _NUMBER, f, d)
							{ }

		virtual Entity* clone () const override
							{ return (new Blinky (id (), forms (), data ())); }

		protected:
		/** When chasing Blinky's target position is the position where pacman is,
			in other cicunstances is the run away position. */
		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const override;
	};

	/** Pinky, the pink monster. Pure speed. */
	class Pinky final : public StandardMonster
	{
		public:
		static const int _NUMBER = 2;

		Pinky (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: StandardMonster (cId, _NUMBER, f, d)
							{ }

		virtual Entity* clone () const override
							{ return (new Pinky (id (), forms (), data ())); }

		protected:
		/** When chasing Pinky's target position is 4 positions ahead pacman's,
			otherwise if the run away position in the maze. */
		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const override;
	};

	/** Clyde, the orange monster. Just the opposite: pokey*/
	class Clyde final : public StandardMonster
	{
		public:
		static const int _NUMBER = 3;

		Clyde (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: StandardMonster (cId, _NUMBER, f, d)
							{ }

		virtual Entity* clone () const override
							{ return (new Clyde (id (), forms (), data ())); }

		protected:
		/** When chasing, Clyde's target depends on the distance to Pacman:
			If it is 8 positions or greater, then goes for Pacman, otherwhise it is the run away position.
			When no chaing the targt position is the run away. */
		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const override;
	};
}

#endif
  
// End of the file
/*@}*/
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
	/** All artists in pacman moves in the same way. 
		The center of any artist always move from the center to a tile to the center of the next
		following the direction of the movement. \n 
		So, the variable _direction should indicate */
	class Artist : public QGAMES::ArtistInATiledMap
	{
		public:
		Artist () = delete;

		Artist (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: QGAMES::ArtistInATiledMap (cId, f, d),
			  _lastMazePosition (QGAMES::MazeModel::_noPosition),
			  _pathInMaze (), // No defined by default...
			  _referenceArtists ()
							{ }

		Artist (const Artist&) = delete;

		Artist& operator = (const Artist&) = delete;

		virtual QGAMES::SetOfOpenValues runtimeValues () const override;
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& oV) override;

		/** To idntify whether the artist is or not alive. */
		virtual bool isAlive () const
							{ return (true); }

		/** To load other artists takn as a rference for this. */
		virtual void setReferenceArtists (const std::vector <const Artist*>& r)
							{ _referenceArtists = r; }

		// To control the different positions of the artist
		/** To know which is the current position in the maze. 
			This method makes only sense when map has been set. */
		QGAMES::MazeModel::PositionInMaze currentMazePosition () const;
		/** What is the target maze position of the artist. 
			The target will dpend on the artists itself. */
		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const = 0;
		/** To know the next target position of the artist. 
			taken into account a certain number of positions. 
			By default returns the a number of celds ahead of the current position when there is no path defined,
			and the position n in the path when it is defined. */
		virtual QGAMES::MazeModel::PositionInMaze nextMazePosition (int n = 0) const
							{ return (_pathInMaze.empty () 
								? currentMazePosition () + (__BD n * direction ())
								: ((n < (int) _pathInMaze.size ()) ? _pathInMaze [n] : _pathInMaze [(int) _pathInMaze.size () - 1])); }
		/** To know the last target position of the artist. 
			None when the artist has startd to move. */
		const QGAMES::MazeModel::PositionInMaze& lastMazePosition () const
							{ return (_lastMazePosition); }
		/** To know the movement path of the artist if any. */
		const std::vector <QGAMES::MazeModel::PositionInMaze>& pathInMaze () const
							{ return (_pathInMaze); }

		virtual void initialize () override;
		virtual void updatePositions () override;

		protected:
		QGAMES::MazeModel::PositionInMaze _lastMazePosition;
		std::vector <QGAMES::MazeModel::PositionInMaze> _pathInMaze;
		std::vector <const Artist*> _referenceArtists;
	};

	/** What the player controls. */
	class PacMan final : public Artist
	{
		public:
		PacMan (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: Artist (cId, f, d),
			  _score (0),
			  _nextDirectionWhenPossible (QGAMES::Vector::_noPoint), // Meaning not to change even when possible...
			  _counter (0)
							{ }

		virtual QGAMES::SetOfOpenValues runtimeValues () const override;
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& oV) override;

		virtual Entity* clone () const override
							{ return (new PacMan (id (), forms (), data ())); }

		int score () const
							{ return (_score); }
		void setScore (int s); 

		void toMove (const QGAMES::Vector& d)
							{ _nextDirectionWhenPossible = d; }
		const QGAMES::Vector& nextDirectionWhenPossible () const
							{ return (_nextDirectionWhenPossible); }
		void toShoot (int f, const QGAMES::Vector& d);

		/** The target position is the limit of the maze in the current direction of the movement. */
		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const override;

		virtual void initialize () override;
		virtual void updatePositions () override;
		virtual void finalize () override;

		private:
		int _score;
		/** Which is the next direction to take into account when possible. */
		QGAMES::Vector _nextDirectionWhenPossible;

		// Implementation
		int _counter;
	};

	/** What the machine controls. */
	class Monster : public Artist
	{
		public:
		Monster (int cId, int mN, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: Artist (cId, f, d),
			  _monsterNumber (mN),
			  _toPursuit (nullptr),
			  _chasing (false)
							{ assert (_monsterNumber >= 0); }

		/** The number of the monster. */
		int monsterNumber () const
							{ return (_monsterNumber); }

		/** To know where is the place to run away id needed. */
		QGAMES::MazeModel::PositionInMaze runAwayMazePosition () const;

		/** To set the the artist to pursuit. Usually it is a pacman, but... \n
			It is null when none is pursuited. */
		virtual void toPursuit (const Artist* a)
							{ _toPursuit = a; }
		const Artist* pursuit () const
							{ return (_toPursuit); }

		/** Chasing or not. */
		void toChase (bool c)
							{ _chasing = c; }
		bool chasing () const
							{ return (_chasing); }

		virtual void initialize () override;
		virtual void updatePositions () override;
		virtual void finalize () override;

		protected:
		/** The number of the monster. */
		int _monsterNumber;
		/** A reference to the artist being pursuited. */
		const Artist* _toPursuit;
		/** Is the monster chasing someone or not? */
		bool _chasing;
	};

	/** The standard monster always pursuit a pacman. */
	class StandardMonster : public Monster
	{
		public:
		StandardMonster (int cId, int mN, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: Monster (cId, mN, f, d)
							{ }

		/** It has to be pacman. */
		virtual void toPursuit (const Artist* a) final override;
	};

	/** Inky, the blue/cyan monster. Bashful. */
	class Inky final : public StandardMonster
	{
		public:
		Inky (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
			const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: StandardMonster (cId, 0, f, d)
							{ }

		virtual Entity* clone () const override
							{ return (new Inky (id (), forms (), data ())); }

		/** Inky has blinky as reference. 
			(and also pacman, but pacman is the one to pursuit. */
		virtual void setReferenceArtists (const std::vector <const Artist*>& r) override;

		/** When chasing Inky's target position is twice the distance between the 2 next positions of pacman and blinky's 
			In other circunstance is the run away position. */
		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const override;
	};

	/** Blinky, the red monster. Your shadow. */
	class Blinky final : public StandardMonster
	{
		public:
		Blinky (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
			const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: StandardMonster (cId, 1, f, d)
							{ }

		virtual Entity* clone () const override
							{ return (new Blinky (id (), forms (), data ())); }

		// Blinky has no other reference than pacman...

		/** When chasing Blinky's target position is the position where pacman is,
			in other cicunstances is the run away position. */
		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const override;
	};

	/** Pinky, the pink monster. Pure speed. */
	class Pinky final : public StandardMonster
	{
		public:
		Pinky (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: StandardMonster (cId, 2, f, d)
							{ }

		virtual Entity* clone () const override
							{ return (new Pinky (id (), forms (), data ())); }

		/** When chasing Pinky's target position is 4 positions ahead pacman's,
			otherwise if the run away position in the maze. */
		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const override;
	};

	/** Clyde, the orange monster. Just the opposite: pokey*/
	class Clyde final : public StandardMonster
	{
		public:
		Clyde (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: StandardMonster (cId, 3, f, d)
							{ }

		virtual Entity* clone () const override
							{ return (new Clyde (id (), forms (), data ())); }

		/** When chasing, Clyde's target depends on the distance to Pacman:
			If it is 8 positions or greater, then goes for Pacman, otherwhise it is the run away position.
			When no chaing the targt position is the run away. */
		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const override;
	};
}

#endif
  
// End of the file
/*@}*/
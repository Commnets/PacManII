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
	class Map;
	class Artist : public QGAMES::ArtistInATiledMap
	{
		public:
		Artist () = delete;

		Artist (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: QGAMES::ArtistInATiledMap (cId, f, d),
			  _lastMazePosition (QGAMES::MazeModel::_noPosition),
			  _pathInMaze (), // No defined by default...
			  _referenceArtists (),
			  _nextDirectionWhenPossible (QGAMES::Vector::_cero),
			  _referenceArtistLastMazePositions ()
							{ }

		Artist (const Artist&) = delete;

		Artist& operator = (const Artist&) = delete;

		// It is not needed to redefine the methods to save / recover the status of the artist 
		// As the gane always start with them in the same position...

		/** To get the last maze position of the artist. */
		const QGAMES::MazeModel::PositionInMaze lastMazePosition () const
							{ return (_lastMazePosition); }

		/** When it is needed to change the direction of the movement, 
			this method should be invoked. The artist will change when possible. */
		const QGAMES::Vector& directionWhenPossible () const
							{ return (_nextDirectionWhenPossible); }
		void changeDirectionWhenPossibleTo (const QGAMES::Vector& d)
							{ _nextDirectionWhenPossible = d; }

		/** To load other artists taken as a reference for this. */
		const std::vector <const Artist*>& referenceArtists () const
							{ return (_referenceArtists); }
		virtual void setReferenceArtists (const std::vector <const Artist*>& r)
							{ _referenceArtists = r; }
		void keepLastReferenceArtistsPosition ();
		bool hasReferenceArtistsPositionChanged () const;

		virtual bool canMove (const QGAMES::Vector& d, const QGAMES::Vector& a) override;

		// To know which is current state of the artist in terms of movement...
		/** To identify whether the artist is or not alive. */
		virtual bool isAlive () const = 0;
		/** To identify whether it is standing or not. */
		virtual bool isStanding () const = 0;
		/** To identify whether it is moving or not. */
		virtual bool isMoving () const = 0;

		// To control the different positions of the artist
		/** To know the movement path of the artist if any. */
		const std::vector <QGAMES::MazeModel::PositionInMaze>& pathInMaze () const
							{ return (_pathInMaze); }
		/** To set the path to the target fromthe current position. 
			There is gneric way, but it can be redefined by typ of monster. */
		virtual void calculatePathInMazeAvoiding (const std::vector <QGAMES::Vector>& d);
		/** To cancel the path. */
		void anulePath ()
							{ _pathInMaze = { }; }
		/** To reduce the path in x elemnts from the beginning. */
		void extractStepsFromPath (int nS);
		/** To know which is the maze position if the position in the map was... \n
			Obviouslly it makes only sense when the map has been set, otherwise returns no position. */
		QGAMES::MazeModel::PositionInMaze mapPositionToMazePosition (const QGAMES::Position& p) const;
		/** The opposite of the previous one. \n
			To get the maze position from a theoretical maze position. \n 
			Obviously it makes sense only when the map has been defined, otherwise it returns no position. */
		QGAMES::Position mazePositionToMapPosition (const QGAMES::MazeModel::PositionInMaze& p) const;
		/** To know which is the current position in the maze. 
			This method makes only sense when map has been set. */
		QGAMES::MazeModel::PositionInMaze currentMazePosition () const
							{ return (mapPositionToMazePosition (position ())); }
		/** To know whether the current position matches exactly a tile position. \n
			Makes sense only when map has been set. */
		bool positionMatchesTile (const QGAMES::Position& p) const;
		/** What is the target maze position of the artist. 
			The target will depend on the artists itself, and even (in the case of monsters e.g.) on the status of it!. */
		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const = 0;
		/** To know the next target position of the artist. \n
			The parameter indicats the number of step. It is grater than the maximum one, a no position is returned. */
		QGAMES::MazeModel::PositionInMaze nextMazePosition (int n = 0) const;

		virtual void initialize () override;
		virtual void updatePositions () override;

		virtual void processEvent (const QGAMES::Event& evnt) override;

		protected:
		/** To facilitate the internal conversion. */
		inline const Map* pMap () const;
		inline Map* pMap ();

		// To control the aspect and what to do when stopping and moving
		/** To change the state of the artist to stand looking to a direction. */
		virtual void setStateToStandLookingTo (const QGAMES::Vector& d) = 0;
		/** To change the state of the artist to move looking to a direction. */
		virtual void setStateToMoveTo (const QGAMES::Vector& d) = 0;
		/** What to do when the end of the plan path is reached. */
		virtual void whatToDoWhenStopIsRequested () = 0;
		/** What to do when it is the beginning of a new requested movement. */
		virtual void whatToDoWhenMovementIsRequested (const QGAMES::Vector& d) = 0;

		protected:
		QGAMES::MazeModel::PositionInMaze _lastMazePosition;
		QGAMES::Vector _nextDirectionWhenPossible;
		std::vector <QGAMES::MazeModel::PositionInMaze> _pathInMaze;
		std::vector <const Artist*> _referenceArtists;

		// Implementation
		std::map <int, QGAMES::MazeModel::PositionInMaze> _referenceArtistLastMazePositions;
	};

	/** What the player controls. */
	class PacMan final : public Artist
	{
		public:
		PacMan (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: Artist (cId, f, d),
			  _alive (true),
			  _chasing (false),
			  _score (0),
			  _counter (0)
							{ }

		virtual Entity* clone () const override
							{ return (new PacMan (id (), forms (), data ())); }

		virtual bool isAlive () const override
							{ return (_alive); }
		void setAlive (bool a)
							{ _alive = a; }
		virtual bool isStanding () const override;
		virtual bool isMoving () const override;

		// To manahe when it has "power"
		// In this situations, e.g. the speed of the movement could be different...
		bool isChasing () const
							{ return (_chasing); }
		void setChasing (bool c);

		int score () const
							{ return (_score); }
		void setScore (int s); 

		// The movements of the pacman
		void toShoot (int f, const QGAMES::Vector& d);

		/** The target position is the limit of the maze in the current direction of the movement. */
		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const override;

		virtual void initialize () override;
		virtual void updatePositions () override;
		virtual void finalize () override;

		protected:
		virtual void setStateToStandLookingTo (const QGAMES::Vector& d) override;
		virtual void setStateToMoveTo (const QGAMES::Vector& d) override;
		virtual void whatToDoWhenStopIsRequested () override;
		virtual void whatToDoWhenMovementIsRequested (const QGAMES::Vector& d) override;

		/** Internal method to adapt the speed. */
		void adaptSpeed ();

		private:
		/** To indicate whether the pacman is alive. */
		bool _alive;
		/** To indicate whether the pacman in o no chasing monsters. */
		bool _chasing;
		/** The score of the pacman. It is get and put back into the game. */
		int _score;

		// Implementation
		int _counter;
	};

	/** What the machine controls. */
	class Monster : public Artist
	{
		public:
		// The different status a monster cab ne in...
		enum class Status 
			{ _NOTDEFINED = 0, _ATHOME = 1, _EXITINGHOME = 2, _CHASING = 3, _RUNNINGWAY = 4, _TOBEEATEN = 5, _BEINGEATEN = 6 };

		Monster (int cId, int mN, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: Artist (cId, f, d),
			  _monsterNumber (mN),
			  _toPursuit (nullptr),
			  _status (Status::_NOTDEFINED)
							{ assert (_monsterNumber >= 0); }

		/** The number of the monster. */
		int monsterNumber () const
							{ return (_monsterNumber); }

		// To know and change the status...
		void setStatus (const Status& st);
		Status status () const
							{ return (_status); }

		virtual bool isAlive () const override
							{ return (_status != Status::_BEINGEATEN); }
		virtual bool isStanding () const override
							{ return (false); }
		virtual bool isMoving () const override
							{ return (true); }

		/** To know where is the place to run away id needed. */
		QGAMES::MazeModel::PositionInMaze runAwayMazePosition () const;

		/** To set the the artist to pursuit. Usually it is a pacman, but... \n
			It is null when none is pursuited. */
		virtual void toPursuit (const Artist* a)
							{ _toPursuit = a; }
		const Artist* pursuit () const
							{ return (_toPursuit); }

		virtual void initialize () override;
		virtual void updatePositions () override;
		virtual void finalize () override;

		protected:
		virtual void whatToDoWhenStopIsRequested () override;
		virtual void whatToDoWhenMovementIsRequested (const QGAMES::Vector& d) override;

		protected:
		/** The number of the monster. */
		int _monsterNumber;
		/** A reference to the artist being pursuited. */
		const Artist* _toPursuit;
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

		/** It has to be pacman always. */
		virtual void toPursuit (const Artist* a) final override;

		/** The tager position in a standard monster for most of the internal status except chasing is calculated in the same way. \
			Just only the target position when chasing will depend on the specific monster. */
		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const override;
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

		/** Inky has blinky as reference. 
			(and also pacman, but pacman is the one to pursuit. */
		virtual void setReferenceArtists (const std::vector <const Artist*>& r) override;

		/** When chasing Inky's target position is twice the distance between the 2 next positions of pacman and blinky's 
			In other circunstance is the run away position. */
		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const override;

		protected:
		virtual void setStateToStandLookingTo (const QGAMES::Vector& d) override;
		virtual void setStateToMoveTo (const QGAMES::Vector& d) override;
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

		// Blinky has no other reference than pacman...

		/** When chasing Blinky's target position is the position where pacman is,
			in other cicunstances is the run away position. */
		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const override;

		protected:
		virtual void setStateToStandLookingTo (const QGAMES::Vector& d) override;
		virtual void setStateToMoveTo (const QGAMES::Vector& d) override;
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

		/** When chasing Pinky's target position is 4 positions ahead pacman's,
			otherwise if the run away position in the maze. */
		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const override;

		protected:
		virtual void setStateToStandLookingTo (const QGAMES::Vector& d) override;
		virtual void setStateToMoveTo (const QGAMES::Vector& d) override;
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

		/** When chasing, Clyde's target depends on the distance to Pacman:
			If it is 8 positions or greater, then goes for Pacman, otherwhise it is the run away position.
			When no chaing the targt position is the run away. */
		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const override;

		protected:
		virtual void setStateToStandLookingTo (const QGAMES::Vector& d) override;
		virtual void setStateToMoveTo (const QGAMES::Vector& d) override;
	};
}

#endif
  
// End of the file
/*@}*/
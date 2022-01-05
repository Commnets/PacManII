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
	class Map;

	/** Any element in this game to interact with. */
	class PacmanElement : public QGAMES::ArtistInATiledMap
	{
		public:
		PacmanElement () = delete;

		PacmanElement (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: QGAMES::ArtistInATiledMap (cId, f, d)
							{ setVisible (false); }

		PacmanElement (const Artist&) = delete;

		PacmanElement& operator = (const Artist&) = delete;

		/** To know which is the current position in the maze. 
			This method makes only sense when map has been set. */
		QGAMES::MazeModel::PositionInMaze currentMazePosition () const
							{ return (mapPositionToMazePosition (position ())); }
		/** To know which the next grid position will be (from the current position and following the orientation if any). */
		QGAMES::MazeModel::PositionInMaze nextXGridPosition (int x) const;
		/** To know which is th current are ocuppied by the pacman element. */
		int currentMazeArea () const;

		/** To know whether anotheer Pacman Element is or not my enemy. */
		virtual bool isEnemy (const PacmanElement* elmnt) const = 0;

		// To know which is current state of the pacmen element in terms of movement...
		/** To identify whether the artist is or not alive. */
		virtual bool isAlive () const = 0;
		/** To identify whether it is standing or not. */
		virtual bool isStanding () const = 0;
		/** To identify whether it is moving or not. */
		virtual bool isMoving () const = 0;

		protected:
		// Implementation
		// Used many times inside...
		/** Returns the PacMan map used. */
		inline const Map* pMap () const; // Not defined here to avoid recursive #include
		/** Same than above. */
		inline Map* pMap ();
		/** To convert a map position into a maze position. 
			It relies in the information of the map. */
		inline QGAMES::MazeModel::PositionInMaze mapPositionToMazePosition (const QGAMES::Position& p) const;
		/** The opposite. It gives back position in the map from maze position. \n
			The position returned is the center of the tile representing that maze position. */
		inline QGAMES::Position mazePositionToMapPosition (const QGAMES::MazeModel::PositionInMaze& p) const;
		/** Returns true when a map position is aligned exactly with an equivalent maze position. \n
			This method is very useful to determine when is time for the artist to change the path followed. */
		bool doesPositionMatchesTile (const QGAMES::Position& p) const;
	};

	/** All artists in pacman moves and also in the same way. 
		The center of any artist always move from the center of a tile to the center of the next one
		following the direction of the movement. \n 
		So, the variable _direction should indicate that. 
		When the artist is stopped, th variabl direction should be 0, 
		but the variable orientation should indicate to which place the element is looking to. */
	class Artist : public PacmanElement
	{
		public:
		friend class MazeMovement;

		Artist (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ());

		// It is not needed to redefine the methods to save / recover the status of the artist 
		// As the gane always start with them in the same position...

		/** The very basic actions in an artist */
		virtual void toStand () = 0;
		virtual void toMove (const QGAMES::Vector& d) = 0;

		/** To get the last maze position of the artist. */
		const QGAMES::MazeModel::PositionInMaze lastMazePosition () const
							{ return (_lastMazePosition); }

		/** To load other artists taken as a reference for this. */
		const std::vector <Artist*>& referenceArtists () const
							{ return (_referenceArtists); }
		virtual void setReferenceArtists (const std::vector <Artist*>& r)
							{ _referenceArtists = r; }

		/** When it is needed to change the direction of the movement, 
			this method should be invoked. The artist will change when possible. */
		void changeDirectionWhenPossibleTo (const QGAMES::Vector& d);

		/** To know what is the next expected position of the artist in the map. */
		QGAMES::MazeModel::PositionInMaze nextMazePosition (int n = 0) const
							{ return (_pathInMaze.empty () 
								? currentMazePosition () 
								: ((n < (int) _pathInMaze.size ()) ? _pathInMaze [n] : QGAMES::MazeModel::_noPosition)); }

		/** A little bit reduced from the original one. */
		virtual QGAMES::Rectangle collisionZone () const
							{ return (PacmanElement::collisionZone ().scale (__BD 0.75)); } // A little bit reduced (half)

		virtual void initialize () override;
		virtual void updatePositions () override;
		virtual void drawOn (QGAMES::Screen* scr, const QGAMES::Position& p = QGAMES::Position::_noPoint) override;

		virtual void processEvent (const QGAMES::Event& evnt) override;

		protected:
		// Method to really overload per type of artist
		/** What is the target maze position of the artist. 
			The target will depend on the artists itself, and even (in the case of monsters e.g.) on the status of it!. */
		virtual QGAMES::MazeModel::PositionInMaze targetMazePosition () const = 0;

		// To control the aspect and what to do when stopping and moving
		/** What to do when the end of the plan path is reached. */
		virtual void whatToDoWhenStopStatusIsRequested (const QGAMES::Vector&) = 0;
		/** What to do when it is the beginning of a new requested movement. */
		virtual void whatToDoWhenMovementStatusIsRequested (const QGAMES::Vector&) = 0;
		/** What to do when artists is on a specific position in the maze.
			What it is beneath the artist should modify actually the behaviour. \n */
		virtual void whatToDoOnCurrentPosition () = 0;
		/** To change the state of the artist to stand looking to a direction. */
		virtual void setStateToStandLookingTo (const QGAMES::Vector&) = 0;
		/** To change the state of the artist to move looking to a direction. */
		virtual void setStateToMoveTo (const QGAMES::Vector&) = 0;

		/** Define a buoy to stop within the inEveryLoop method. */
		class ToStopBuoy final : public QGAMES::Buoy
		{
			public:
			ToStopBuoy ()
				: QGAMES::Buoy (__PACMANII_TOSTOPBUOYID__, (QGAMES::bdata) 0)
							{ /** Nothing else to do. */ }

			virtual void* treatFor (QGAMES::Element* e) override;
		};

		/** The method to stop deferred. */
		void toStopDeferred ();

		// Implementation
		// Used many times inside...
		/** To recalculate the path to follow (usually 2 steps more) from the current position. \n
			It doesn't matter whether it is aligned with the maze position, but usually it is invoked in that situation. 
			A mandatory direction can be passed. It will be takn into account if possible (by default it is not). */
		virtual QGAMES::MazeModel::PathInMaze& recalculatePathInMaze (const QGAMES::Vector& mD = QGAMES::Vector::_noPoint);
		/** To know whether the current situation in the path implies entering or not a tunnel. \n
			It is useful to know e.g whether to move th eposition of the artist. */
		bool isArtistEnteringATunnel () const;
		/** To know whether the reference artists of the this on have or not change their positions. \n
			In some cases, when this happens, the artist could vary it path. */
		bool hasReferenceArtistsPositionChanged () const; 

		protected:
		QGAMES::MazeModel::PositionInMaze _lastMazePosition;
		std::vector <QGAMES::MazeModel::PositionInMaze> _pathInMaze;
		std::vector <Artist*> _referenceArtists;

		// Implementation
		QGAMES::Vector _nextDirectionWhenPossible;
		std::map <int, QGAMES::MazeModel::PositionInMaze> _referenceArtistLastMazePositions;
	};
}

#include "PacMan.hpp"
#include "Fruit.hpp"
#include "Monsters.hpp"

#endif
  
// End of the file
/*@}*/
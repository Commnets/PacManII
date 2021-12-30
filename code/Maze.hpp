/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: Maze.hpp \n
 *	Game: PacManII \n
 *	Author: Ignacio Cea Forniés (Community Networks) \n
 *	Creation Date: 31/08/2021 \n
 *	Description: PacmanII maze main definition file. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_MAZE__
#define __PACMANII_MAZE__

#include "Defs.hpp"
#include <advancedArcade/adinclude.hpp>
#include <Platform/ptwayinmaze.hpp>

namespace PacManII
{
	/** An internal respresentation of the maze.
		This Maze represents a 2D Maze where the artist can move in only 4 directions: up, down, right and left. 
		So, there are always 4 possible directions to connect a cell with the next: up, down, right o left.
		In every position of the maze 4 OpenValue are kept to indicate whether those 4 directions are o not possible.
		In same cells, when only the movement is a single direction is allowed,
		a 5th element is defined, to indicate the position of the cell connected moving towards the
		opposite direction to th only one allowed. */
	class Map;
	class Maze final : public QGAMES::MazeModel
	{
		public:
		Maze () = delete; // Not possible

		Maze (const Maze&) = default;

		Maze& operator = (const Maze&) = default;

		/** To get the pure grid x next position following a direction.
			If the position will be out of the maze, the last one possible will be returnd. */
		QGAMES::MazeModel::PositionInMaze nextXGridPositionFollowing (const QGAMES::MazeModel::PositionInMaze& p,
			unsigned x, const QGAMES::Vector& d) const;

		/** In this case, because the size of th maze (usually 21 x 24), 
			the standard methods to calculate the path to go from a position to another
			will be very slow. \n
			So, a new one is be defined, but just only to calculate 2 steps. */
		QGAMES::MazeModel::PathInMaze next2StepsToGoTo (const QGAMES::MazeModel::PositionInMaze& p1, 
			const QGAMES::MazeModel::PositionInMaze& p2, const std::vector <QGAMES::Vector>& d) const;
		QGAMES::MazeModel::PathInMaze nextXStepsToGoTo (int x, const QGAMES::MazeModel::PositionInMaze& p1, 
			const QGAMES::MazeModel::PositionInMaze& p2, const std::vector <QGAMES::Vector>& d) const;

		/** Those methods are invoked from the PacManII::Map to create a PacManII::Maze. \n
			The methods guarantte that nothing is wrong in the construction. */
		static Maze generateEmptyMaze (int x, int y);
		static Maze generateMazeFrom (int w, int h, const Map* mp);

		private:
		virtual int maxPossibleConnectionsPerPosition () const override
								{ return (4); } // LEFT, RIGH, UP & DOWN...
		virtual std::vector <QGAMES::Vector> allPossibleDirectionsAt (const QGAMES::MazeModel::PositionInMaze&) const override;
		virtual PositionInMaze nextPositionToFollowing 
			(const QGAMES::MazeModel::PositionInMaze& p, const QGAMES::Vector& d) const override;
		virtual std::vector <QGAMES::Vector> orderDirections (const QGAMES::MazeModel::PositionInMaze& p1, 
			const QGAMES::MazeModel::PositionInMaze& p2, const std::vector <QGAMES::Vector>& dirs) const override;

		/** The SetOfOpenValues has 4/5 values to indicate whether the movement to each of the 4 directions is possible. \n
			Value 0: LFT, Value 1: RIGHT, Value 2: UP, Value 3: DOWN. \n
			The 5th element, when exists, represents the position of the cell 
			connected in the opposite direction to the one allowed. 
			It is declared private to avoid nobody could used out of this class (the static method). */
		Maze (int sX, int sY, const std::vector <QGAMES::SetOfOpenValues>& mW)
			: QGAMES::MazeModel (sX, sY, 1 /* 2d maze always */, mW)
							{ }
	};
}

#endif
  
// End of the file
/*@}*/

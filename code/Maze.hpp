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
	/** An internal respresentation of the maze
		In eevery position of the maze there is information about what cells are connected. 
		In the 4 possible directions of a pacman maze, those are: up, down, lft, right. 
		This maze is 2d maze always. */
	class DirectionsLayer;
	class Maze : public QGAMES::MazeModel
	{
		public:
		Maze () = delete; // Not possible

		/** The SetOfOpenValues has 4 values to indicate whether the movement to each of the 4 directions is possible.
			Value 0: LFT, Value 1: RIGHT, Value 2: UP, Value 3: DOWN. */
		Maze (int sX, int sY, const std::vector <QGAMES::SetOfOpenValues>& mW)
			: QGAMES::MazeModel (sX, sY, 1 /* 2d maze always */, mW)
							{ }

		Maze (const Maze&) = default;

		Maze& operator = (const Maze&) = default;

		static Maze generateEmptyMaze (int x, int y);
		static Maze generateMazeFrom (int w, int h, const DirectionsLayer* dL);

		protected:
		virtual int maxPossibleConnectionsPerPosition () const override
								{ return (4); } // LEFT, RIGH, UP & DOWN...
		virtual std::vector <QGAMES::Vector> allPossibleDirectionsAt (const QGAMES::MazeModel::PositionInMaze&) const override;
		virtual PositionInMaze nextPositionToFollowing 
			(const QGAMES::MazeModel::PositionInMaze& p, const QGAMES::Vector& d) const override;
		virtual std::vector <QGAMES::Vector> orderDirections (const QGAMES::MazeModel::PositionInMaze& p1, 
			const QGAMES::MazeModel::PositionInMaze& p2, const std::vector <QGAMES::Vector>& dirs) const override;
	};
}

#endif
  
// End of the file
/*@}*/

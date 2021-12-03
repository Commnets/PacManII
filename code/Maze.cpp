#include "Maze.hpp"
#include "Worlds.hpp"
#include "Game.hpp"

// ---
std::vector <QGAMES::Vector> PacManII::Maze::allPossibleDirectionsAt (const PositionInMaze& p) const
{
	QGAMES::SetOfOpenValues mI = mazeInfoAt (p);
	assert (mI.existOpenValue (0) && mI.existOpenValue (1) && 
			mI.existOpenValue (2) && mI.existOpenValue (3)); // There must by 4 values...

	std::vector <QGAMES::Vector> result;

	// LEFT, RIGHT, UP, DOWN in th openValues,

	if (mI.openValue (0).boolValue ()) result.push_back (QGAMES::Vector (__BD -1, __BD  0, __BD 0));
	if (mI.openValue (1).boolValue ()) result.push_back (QGAMES::Vector (__BD  1, __BD  0, __BD 0));
	if (mI.openValue (2).boolValue ()) result.push_back (QGAMES::Vector (__BD  0, __BD -1, __BD 0));
	if (mI.openValue (3).boolValue ()) result.push_back (QGAMES::Vector (__BD  0, __BD  1, __BD 0));

	return (result);
}

// ---
QGAMES::MazeModel::PositionInMaze PacManII::Maze::nextPositionToFollowing 
	(const QGAMES::MazeModel::PositionInMaze& p, const QGAMES::Vector& d) const
{
	QGAMES::MazeModel::PositionInMaze nP = p + d;

	if (!isInMaze (nP))
		return (QGAMES::MazeModel::_noPosition);

	std::vector <QGAMES::Vector> aP = allPossibleDirectionsAt (nP);
	return ((std::find (aP.begin (), aP.end (), -d) == aP.end ()) ? QGAMES::MazeModel::_noPosition : nP); 
}

// ---
std::vector <QGAMES::Vector> PacManII::Maze::orderDirections 
	(const QGAMES::MazeModel::PositionInMaze& p1, 
	 const QGAMES::MazeModel::PositionInMaze& p2, const std::vector <QGAMES::Vector>& dirs) const
{
	std::vector <int> o = { 0, 1, 2, 3 }; // By default: RIGHT, DOWN, LEFT, UP
	std::vector <QGAMES::Vector> dO = 
		{ QGAMES::Vector (__BD 1, __BD 0, __BD 0),
		  QGAMES::Vector (__BD 0, __BD 1, __BD 0),
		  QGAMES::Vector (__BD -1, __BD 0, __BD 0),
		  QGAMES::Vector (__BD 0, __BD -1, __BD 0) };	
	
	assert (verifyDirections (dirs));

	if (p2._positionX > p1._positionX)
	{
		if (p2._positionY >= p1._positionY) o = { 0, 1, 2, 3 };
		else if (p2._positionY < p1._positionY) o = { 0, 3, 2, 1 };
	}
	else
	if (p2._positionX < p1._positionX)
	{
		if (p2._positionY >= p1._positionY) o = { 2, 1, 0, 3 };
		else if (p2._positionY < p1._positionY) o = { 2, 3, 0, 1 };
	}
	else
	{
		if (p2._positionY >= p1._positionY) o = { 1, 2, 3, 0 };
		else if (p2._positionY < p1._positionY) o = { 3, 2, 1, 0 };
	}

	std::vector <QGAMES::Vector> result;
	for (auto i : o)
		if (std::find (dirs.begin (), dirs.end (), dO [i]) != dirs.end ())
			result.push_back (dO [i]);

	assert (dirs.size () == result.size ());

	return (result);
}

// ---
PacManII::Maze PacManII::Maze::generateEmptyMaze (int x, int y)
{
	assert (x > 0 && y > 0);

	std::vector <QGAMES::SetOfOpenValues> mV;
	for (int i = 0; i < (x * y); i++)
	{
		QGAMES::SetOfOpenValues tV;
		for (int j = 0; j < 4; tV.addOpenValue (j++, QGAMES::OpenValue (true))); // The four possible directions are true...
		mV.push_back (tV);
	}

	return (PacManII::Maze (x, y, mV));
}

// ---
PacManII::Maze PacManII::Maze::generateMazeFrom (int x, int y, const PacManII::DirectionsLayer* dL)
{
	assert (x > 0 && y > 0 && dL != nullptr);

	// Generate the map
	PacManII::Game* g = dynamic_cast <PacManII::Game*> (QGAMES::Game::game ());
	assert (g != nullptr);
	PacManII::TMXMapBuilder* tB = g -> tmxAddsOnMapBuilder ();
	std::vector <QGAMES::SetOfOpenValues> mV;
	for (auto i : dL -> tiles ())
	{
		QGAMES::SetOfOpenValues tV;
		if (dynamic_cast <QGAMES::NullTile*> (i) != nullptr)
			for (int j = 0; j < 4; tV.addOpenValue (j++, QGAMES::OpenValue (false)));
		else
		{
			// The direction frames have the info of the possibilities of movement per
			std::map <int, std::vector <bool>>::const_iterator j = 
				tB -> directionFrames ().find (i -> numberFrame ());
			assert (j != tB -> directionFrames ().end ());
			
			int k = 0;
			for (auto l : (*j).second)
				tV.addOpenValue (k++, l);
		}

		mV.push_back (tV);
	}

	assert ((int) mV.size () == (x * y));

	return (PacManII::Maze (x, y, mV));
}

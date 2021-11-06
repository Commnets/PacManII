#include "Maze.hpp"
#include "Worlds.hpp"
#include "Game.hpp"

// ---
std::vector <QGAMES::Vector> PacManII::Maze::possibleDirectionsAt (const PositionInMaze& p)
{
	QGAMES::SetOfOpenValues mI = mazeInfoAt (p);
	assert (mI.existOpenValue (0) && mI.existOpenValue (1) && 
			mI.existOpenValue (2) && mI.existOpenValue (3)); // There must by 4 values...

	std::vector <QGAMES::Vector> result;

	if (mI.openValue (0).boolValue ()) result.push_back (QGAMES::Vector (__BD -1, __BD 0, __BD 0));
	if (mI.openValue (1).boolValue ()) result.push_back (QGAMES::Vector (__BD 1, __BD 0, __BD 0));
	if (mI.openValue (2).boolValue ()) result.push_back (QGAMES::Vector (__BD 0, __BD -1, __BD 0));
	if (mI.openValue (3).boolValue ()) result.push_back (QGAMES::Vector (__BD 0, __BD 1, __BD 0));

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
		for (int j = 0; j < 4; tV.addOpenValue (j++, QGAMES::OpenValue (true)));
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

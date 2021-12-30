#include "Maze.hpp"
#include "Game.hpp"
#include "Worlds.hpp"
#include "Scenes.hpp"
#include "Maps.hpp"

// ---
QGAMES::MazeModel::PositionInMaze PacManII::Maze::nextXGridPositionFollowing 
	(const QGAMES::MazeModel::PositionInMaze& p, unsigned x, const QGAMES::Vector& d) const
{
	assert (verifyDirections ({ d }));

	QGAMES::MazeModel::PositionInMaze result = p;
	bool out = false;
	for (unsigned i = 0; i < x && !out; i++)
	{
		if (isInMaze (p + d))
			result = p + d;
		else
			out = true;
	}

	return (result);
}

// ---
std::vector <QGAMES::Vector> PacManII::Maze::allPossibleDirectionsAt (const PositionInMaze& p) const
{
	std::vector <QGAMES::Vector> result;

	// LEFT, RIGHT, UP, DOWN in the openValues,
	QGAMES::SetOfOpenValues mI = mazeInfoAt (p);
	if (mI.openValue (0).boolValue ()) result.push_back (QGAMES::Vector (__BD -1, __BD  0, __BD 0));
	if (mI.openValue (1).boolValue ()) result.push_back (QGAMES::Vector (__BD  1, __BD  0, __BD 0));
	if (mI.openValue (2).boolValue ()) result.push_back (QGAMES::Vector (__BD  0, __BD -1, __BD 0));
	if (mI.openValue (3).boolValue ()) result.push_back (QGAMES::Vector (__BD  0, __BD  1, __BD 0));
	// If the point is a tunnel,...
	if (result.size () == 1 && mI.existOpenValue (4))
		result.push_back (-result [0]);	// then the opposite direction to the only one allowed is also added (continue)

	return (result);
}

// ---
QGAMES::MazeModel::PositionInMaze PacManII::Maze::nextPositionToFollowing 
	(const QGAMES::MazeModel::PositionInMaze& p, const QGAMES::Vector& d) const
{
	QGAMES::MazeModel::PositionInMaze result = QGAMES::MazeModel::_noPosition;

	QGAMES::MazeModel::PositionInMaze nP = p + d; // The natural movement...
	if (!isInMaze (nP)) // but if it is out...
	{
		QGAMES::SetOfOpenValues mI = mazeInfoAt (p);
		if (mI.existOpenValue (4)) // ...it could be a tunnel, if it is defined as it!
			result = QGAMES::MazeModel::PositionInMaze (QGAMES::Vector (mI.openValue (4).strValue ()));
	}
	else
	{
		std::vector <QGAMES::Vector> aP = allPossibleDirectionsAt (nP);
		if (std::find (aP.begin (), aP.end (), -d) != aP.end ()) 
			result = nP; 
	}

	return (result);
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
QGAMES::MazeModel::PathInMaze PacManII::Maze::next2StepsToGoTo (const QGAMES::MazeModel::PositionInMaze& p1, 
	const QGAMES::MazeModel::PositionInMaze& p2, const std::vector <QGAMES::Vector>& d) const
{
	std::vector <QGAMES::Vector> aD = getValidAndOrderedDirectionsPossibleFromTo (p1, p2, d);

	QGAMES::MazeModel::PathInMaze result = { p1 };

	if (aD.empty ())
		return (result);

	if (p1 == p2)
		return (result);

	QGAMES::MazeModel::PositionInMaze pA = QGAMES::MazeModel::_noPosition;
	QGAMES::bdata mD = __MAXBDATA__;
	for (auto i : aD)
	{
		if (std::find (d.begin (), d.end (), i) == d.end ())
		{
			QGAMES::MazeModel::PositionInMaze nP = nextPositionToFollowing (p1, i);
			assert (nP != QGAMES::MazeModel::_noPosition); // It shouldn't but who knows!
			QGAMES::bdata pD;
			if ((pD = nP.asVector ().distanceTo (p2.asVector ())) < mD)
			{
				pA = nP;
				mD = pD;
			}
		}
	}

	if (pA != QGAMES::MazeModel::_noPosition)
		result.push_back (pA);

	return (result);
}

// ---
QGAMES::MazeModel::PathInMaze PacManII::Maze::nextXStepsToGoTo (int x, const QGAMES::MazeModel::PositionInMaze& p1, 
			const QGAMES::MazeModel::PositionInMaze& p2, const std::vector <QGAMES::Vector>& d) const
{
	QGAMES::MazeModel::PathInMaze result = next2StepsToGoTo (p1, p2, d);
	if (result.size () == 2 && result [1] != p2)
	{
		bool tE = false;
		do
		{
			QGAMES::Vector lD = 
				result [result.size () - 1].asVector () - result [result.size () - 2].asVector ();
			QGAMES::MazeModel::PathInMaze pR = next2StepsToGoTo (result [result.size () - 1], p2, { lD });
			if (pR.size () == 2)
			{
				if (std::find (result.begin (), result.end (), pR [1]) != result.end ())
					result.push_back (pR [1]);
				else
					tE = true; // step repeated...
			}
			else
				tE = true; // final point found...
		} while (!tE);
	}

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
PacManII::Maze PacManII::Maze::generateMazeFrom (int x, int y, const PacManII::Map* mp)
{
	assert (x > 0 && y > 0 && mp != nullptr);

	PacManII::Game* g = dynamic_cast <PacManII::Game*> (QGAMES::Game::game ());
	assert (g != nullptr);
	PacManII::TMXMapBuilder* tB = g -> tmxAddsOnMapBuilder ();

	// Generate the basic connections...
	std::vector <QGAMES::SetOfOpenValues> mV;
	for (auto i : mp -> directionsLayer () -> tiles ())
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

	PacManII::Maze result (x, y, mV);

	// Now it is time to add the info of the tunnels...
	// First reads the info...
	int cT = 0;
	std::map <int, std::vector <QGAMES::MazeModel::PositionInMaze>> tunnelPos;
	for (auto i : mp -> locationsLayer () -> tiles ())
	{
		if (std::find (tB -> connectionFrames ().begin (), 
					   tB -> connectionFrames ().end (), i -> numberFrame ()) != tB -> connectionFrames ().end ())
		{
			// The tunnels have to be defined in the limits of the map...
			assert (cT % x == 0 || cT % x == (x - 1) || 
					cT / x == 0 || cT / x == (y - 1));

			tunnelPos [i -> numberFrame () - tB -> connectionFrames ()[0]].
				push_back (QGAMES::MazeModel::PositionInMaze (cT % x, cT / x, 0));
		}

		cT++;
	}

	// ...and finally adds that info to the info related with the path...
	for (auto i : tunnelPos)
	{
		assert (i.second.size () == 2); // Entry and exit position...

		// only one direction allowed to define a tunnel...
		assert ((int) result.allPossibleDirectionsAt (i.second [0]).size () == 1); 
		QGAMES::SetOfOpenValues mI1 = result.mazeInfoAt (i.second [0]);
		mI1.addOpenValue (mI1.lastOpenValueId () + 1, 
			QGAMES::OpenValue ((std::stringstream () << i.second [1].asVector ()).str ()));
		result.setMazeInfoAt (i.second [0], mI1);

		// only one direction allowed to define a tunnel...
		assert ((int) result.allPossibleDirectionsAt (i.second [1]).size () == 1); // only one direction...
		QGAMES::SetOfOpenValues mI2 = result.mazeInfoAt (i.second [1]);
		mI2.addOpenValue (mI2.lastOpenValueId () + 1, 
			QGAMES::OpenValue ((std::stringstream () << i.second [0].asVector ()).str ()));
		result.setMazeInfoAt (i.second [1], mI2);
	}

	return (result);
}

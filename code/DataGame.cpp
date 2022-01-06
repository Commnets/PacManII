#include "DataGame.hpp"

// ---
PacManII::DataGame PacManII::DataGame::standardDataGame ()
{
	DataGame::LevelDefinition::ScatterChaseCycles c1 =
		{ { 7.0f, 20.0f }, { 7.0f, 20.0f }, { 5.0f, 20.0f }, { 5.0f, 999999.0f } };
	DataGame::LevelDefinition::ScatterChaseCycles c2 =
		{ { 7.0f, 20.0f }, { 7.0f, 20.0f }, { 5.0f, 1033.0f }, { 0.0f, 999999.0f } };
	DataGame::LevelDefinition::ScatterChaseCycles c3 =
		{ { 5.0f, 20.0f }, { 5.0f, 20.0f }, { 5.0f, 1037.0f }, { 0.0f, 999999.0f } };

	// Original number of dots were a 70% bigger
	// The number of total dots to eat were originally 244. Now there is 170 maximum.
	// So the numbr of balls to eat before a monsters starts to move is 70% of the original one...
	// Times are not moved because it is not needed!
	DataGame::LevelDefinition::LeaveHomeConditions lH1 = 
		{ { 4.0f, { 0, 0, 21, 42 } }, { 4.0f, { 0, 5, 12, 22 } } };
	DataGame::LevelDefinition::LeaveHomeConditions lH2 = 
		{ { 3.0f, { 0, 0, 0, 35 } }, { 4.0f, { 0, 5, 12, 22 } } };
	DataGame::LevelDefinition::LeaveHomeConditions lH3 = 
		{ { 3.0f, { 0, 0, 0, 0 } }, { 4.0f, { 0, 5, 12, 22 } } };

	// The elroy situation in the original game only affects to blinky
	DataGame::LevelDefinition::ElroyConditions eC1 =
		{ { { true /** No more monsters. */, false }, { 10, 0 }, { 0.8f, 0.0f } }, 
		  { { true, false }, { 5, 0 }, { 0.85f, 0.0f } } };
	DataGame::LevelDefinition::ElroyConditions eC2 =
		{ { { true, false }, { 15 /** No more monsters. */, 0 }, { 0.9f, 0.0f } }, 
		  { { true, false }, { 7, 0 }, { 0.95f, 0.0f } } };
	DataGame::LevelDefinition::ElroyConditions eC3 =
		{ { { true, false }, { 20 /** No more monsters. */, 0 }, { 0.9f, 0.0f } }, 
		  { { true, false }, { 10, 0 }, { 0.95f, 0.0f } } };

	// As the number of normal balls in this version is estimated to be a 70% less than in the original one,
	// here the number of points per ball is 70% more than in the original one!
	// So instead 10 each, 17 should be taken. To avoid such awfull value...15 is choosen.
	// Sam wuth the power ball: 75 is the value choosen.
	// In the case of the fruit, they have to appear at 70 and (some levels) 170 dots eaten 
	// Applying the rule above they would be: 41 and 100, transformed into 40 and 100 respectively.
	#define __PACMANII_BASICBLUESCENE__			__PACMANII_BASICSCENE__
	#define __PACMANII_BASICREDSCENE__			__PACMANII_BASICSCENE__ + 25
	#define __PACMANII_BASICGREENSCENE__		__PACMANII_BASICSCENE__ + 50
	#define __PACMANII_BASICYELLOWSCENE__		__PACMANII_BASICSCENE__ + 75
	#define __PACMANII_BASICBLUEMAP__			__PACMANII_BASICMAP__
	#define __PACMANII_BASICREDMAP__			__PACMANII_BASICMAP__ + 25
	#define __PACMANII_BASICGREENMAP__			__PACMANII_BASICMAP__ + 50
	#define __PACMANII_BASICYELLOWMAP__			__PACMANII_BASICMAP__ + 75
	DataGame::LevelDefinitions lD =
		{ 
		  // Level 1
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICBLUESCENE__, __PACMANII_BASICBLUEMAP__,
			15, 75, 4.0,
			{ { 0, 200, 40, 10.0 } }, 
			c1, lH1,
			0.80, 0.71, 0.90, 0.79, 
			0.75, 0.50, 0.40, 0.40,
			eC1 },
		  // Level 2
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICBLUESCENE__ + 1, __PACMANII_BASICBLUEMAP__ + 1,
			15, 75, 4.0,
			{ { 1, 300, 40, 10.0 }, { 0, 300, 100, 10.0 } }, 
			c2, lH2,
			0.90, 0.79, 0.95, 0.83, 
			0.85, 0.55, 0.40, 0.45,
			eC2 },
		  // Level 3
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICBLUESCENE__ + 2, __PACMANII_BASICBLUEMAP__ + 2,
			15, 75, 4.0,
			{ { 2, 400, 50, 10.0 } },
			c2, lH3, 
			0.90, 0.79, 0.95, 0.83, 
			0.85, 0.55, 0.40, 0.45,
			eC3 },
		  // Group II: Green
		  // Level 4
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICGREENSCENE__, __PACMANII_BASICGREENMAP__,
			15, 75, 3.0,
			{ { 3, 500, 40, 10.0 }, { 1, 500, 100, 10.0 } }, 
			c2, lH3,
			0.90, 0.79, 0.95, 0.83, 
			0.85, 0.55, 0.40, 0.45,
			eC3 },
		  // Level 5
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICGREENSCENE__ + 1, __PACMANII_BASICGREENMAP__ + 1,
			15, 75, 3.0,
			{ { 4, 600, 40, 10.0 }, { 3, 600, 100, 10.0 }}, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  // Level 6
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICGREENSCENE__ + 2, __PACMANII_BASICGREENMAP__ + 2,
			15, 75, 3.0,
			{ { 5, 700, 40, 10.0 } }, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  // Group III: Yellow
		  // Level 7
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICYELLOWSCENE__, __PACMANII_BASICYELLOWMAP__,
			15, 75, 3.0,
			{ { 6, 800, 40, 10.0 }, { 3, 800, 100, 10.0 } }, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  // Level 8
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICYELLOWSCENE__ + 1, __PACMANII_BASICYELLOWMAP__ + 1,
			15, 75, 3.0,
			{ { 6, 900, 40, 10.0 }, { 5, 900, 100, 10.0 } }, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  // Level 9
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICYELLOWSCENE__ + 2, __PACMANII_BASICYELLOWMAP__ + 2,
			15, 75, 3.0,
			{ { 5, 1000, 40, 10.0 }, { 6, 1000, 40, 10.0 } }, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  // Group IV: Red
		  // Level 10
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICREDSCENE__, __PACMANII_BASICREDMAP__,
			15, 75, 2.0,
			{ { 4, 1100, 40, 10.0 }, { 5, 1100, 100, 10.0 } }, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  // Level 11
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICREDSCENE__ + 1, __PACMANII_BASICREDMAP__ + 1,
			15, 75, 2.0,
			{ { 7, 1200, 40, 10.0 }, { 4, 1200, 100, 10.0 } }, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  // Level 12
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICREDSCENE__ + 2, __PACMANII_BASICREDMAP__ + 2,
			15, 75, 2.0,
			{ { 8, 1300, 40, 10.0 }, { 7, 1300, 100, 10.0 } }, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  // One of each...
		  // Level 13
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICGREENSCENE__ + 3, __PACMANII_BASICGREENMAP__ + 3,
			15, 75, 2.0,
			{ { 9, 1400, 40, 10.0 }, { 8, 1400, 100, 10.0 } }, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  // Level 14
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICYELLOWSCENE__ + 3, __PACMANII_BASICYELLOWMAP__ + 3,
			15, 75, 2.0,
			{ { 10, 1500, 40, 10.0 }, { 9, 1500, 100, 10.0 } }, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  // Level 15
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICREDSCENE__ + 3, __PACMANII_BASICREDMAP__ + 3,
			15, 75, 4.0,
			{ { 11, 1600, 40, 10.0 },  { 10, 1600, 40, 10.0 } }, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  // Level 16
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICBLUESCENE__ + 4, __PACMANII_BASICBLUEMAP__ + 4,
			15, 75, 4.0,
			{ { 10, 1700, 40, 10.0 }, { 11, 1700, 100, 10.0 } }, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  // Level 17
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICGREENSCENE__ + 4, __PACMANII_BASICGREENMAP__ + 4,
			15, 75, 4.0,
			{ { 9, 1800, 40, 10.0 }, { 10, 1800, 100, 10.0 } }, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  // Level 18
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICYELLOWSCENE__ + 4, __PACMANII_BASICYELLOWMAP__ + 4,
			15, 75, 4.0,
			{ { 8, 1900, 40, 10.0 }, { 9, 1900, 100, 10.0 } }, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  // Level 19
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICREDSCENE__ + 4, __PACMANII_BASICREDMAP__ + 4,
			15, 75, 3.0,
			{ { 7, 2000, 40, 10.0 }, { 8, 2000, 100, 10.0 } }, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  // Level 20
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICBLUESCENE__ + 5, __PACMANII_BASICBLUEMAP__ + 5,
			15, 75, 2.0,
			{ { 8, 2100, 40, 10.0 }, { 7, 2100, 100, 10.0 } }, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87,
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		};

	for (int i = 20; i < __PACMANII_MAXNUMBERLEVELS__; i++)
	{
		int nS = ((i % 4) == 0 
			? __PACMANII_BASICGREENSCENE__ 
			: ((i % 4) == 1 
				? __PACMANII_BASICYELLOWSCENE__
				: ((i % 4) == 2 
					? __PACMANII_BASICREDSCENE__
					: __PACMANII_BASICBLUESCENE__))) + (i / 4); 

		int nM = ((i % 4) == 0 
			? __PACMANII_BASICGREENMAP__ 
			: ((i % 4) == 1 
				? __PACMANII_BASICYELLOWMAP__
				: ((i % 4) == 2 
					? __PACMANII_BASICREDMAP__
					: __PACMANII_BASICBLUEMAP__))) + (i / 4); 
		// One of each...

		lD.push_back // Repeats the last one...
			(PacManII::DataGame::LevelDefinition 
				(lD [19].worldTypeId (), 
				 lD [19].sceneTypeId () + nS, // Varies...
				 lD [19].mapTypeId () + nM, // Varies...
				 lD [19].pointsBall (), 
				 lD [19].pointsPowerBall (),
				 lD [19].secondsChasing (),
				 lD [19].fruitConditions (),
				 lD [19].scatterChaseCycles (),
				 lD [19].leaveHomeConditions (),
				 lD [19].pacmanSpeed (), 
				 lD [19].pacmanSpeedWhenEatingDots (), 
				 lD [19].pacmanSpeedWhenFrighting (), 
				 lD [19].pacmanSpeedWhenEatingFrightingDots (),
				 lD [19].ghostSpeed (), 
				 lD [19].ghostSpeedWhenBeingFrighten (), 
				 lD [19].ghostSpeedWhenExitingHome (), 
				 lD [19].ghostSpeedWhenCrossingTunnel (),
				 lD [19].elroyConditions ()));
	}

	return (PacManII::DataGame (lD, 15000));
}

// ---
int PacManII::DataGame::firstNumberLevelDefinitionForScene (int sId) const
{
	int result = -1;

	for (int i = 0; i < (int) _levels.size () && result == 1; i++)
		if (_levels [i].sceneTypeId () == sId)
			result = i;
	
	return (result);
}

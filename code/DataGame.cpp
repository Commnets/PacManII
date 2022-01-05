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
	// So instead 50 each, 119 should be taken. To avoid such awfull value...75 is choosen.
	DataGame::LevelDefinitions lD =
	/**
		For testing purpouses only:

		{ { __PACMANII_BASICWORLD__, __PACMAN_TESTINGSCENE__, __PACMAN_TESTINGMAP__,
			75, 200, 5.0,
			0, 200, 10.0, 10.0, 
			c1, lH1,
			0.80, 0.71, 0.90, 0.79, 
			0.75, 0.50, 0.40, 0.40,
			eC1 },
		  { __PACMANII_BASICWORLD__, __PACMAN_TESTINGSCENE__ + 1, __PACMAN_TESTINGMAP__ + 1,
			75, 200, 5.0,
			1, 300, 10.0, 10.0, 
			c2, lH2,
			0.90, 0.79, 0.95, 0.83, 
			0.85, 0.55, 0.40, 0.45,
			eC2 },
	*/
		{ { __PACMANII_BASICWORLD__, __PACMANII_BASICSCENE__, __PACMANII_BASICMAP__,
			75, 200, 4.0,
			0, 200, 10.0, 10.0, 
			c1, lH1,
			0.80, 0.71, 0.90, 0.79, 
			0.75, 0.50, 0.40, 0.40,
			eC1 },
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICSCENE__ + 1, __PACMANII_BASICMAP__ + 1,
			75, 200, 4.0,
			1, 300, 10.0, 10.0, 
			c2, lH2,
			0.90, 0.79, 0.95, 0.83, 
			0.85, 0.55, 0.40, 0.45,
			eC2 },
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICSCENE__ + 2, __PACMANII_BASICMAP__ + 2,
			75, 200, 4.0,
			2, 300, 10.0, 10.0,
			c2, lH3, 
			0.90, 0.79, 0.95, 0.83, 
			0.85, 0.55, 0.40, 0.45,
			eC3 },
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICSCENE__ + 3, __PACMANII_BASICMAP__ + 3,
			75, 200, 3.0,
			3, 300, 10.0, 10.0, 
			c2, lH3,
			0.90, 0.79, 0.95, 0.83, 
			0.85, 0.55, 0.40, 0.45,
			eC3 },
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICSCENE__ + 4, __PACMANII_BASICMAP__ + 4,
			75, 200, 3.0,
			4, 400, 10.0, 10.0, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICSCENE__ + 5, __PACMANII_BASICMAP__ + 5,
			75, 200, 3.0,
			5, 400, 10.0, 10.0, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		{ __PACMANII_BASICWORLD__, __PACMANII_BASICSCENE__ + 6, __PACMANII_BASICMAP__ + 6,
			75, 200, 3.0,
			6, 400, 10.0, 10.0, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICSCENE__ + 7, __PACMANII_BASICMAP__ + 7,
			75, 200, 3.0,
			6, 400, 10.0, 10.0, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICSCENE__ + 8, __PACMANII_BASICMAP__ + 8,
			75, 200, 3.0,
			5, 500, 10.0, 10.0, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICSCENE__ + 9, __PACMANII_BASICMAP__ + 9,
			75, 200, 2.0,
			4, 500, 10.0, 10.0, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICSCENE__ + 10, __PACMANII_BASICMAP__ + 10,
			75, 200, 2.0,
			7, 500, 10.0, 10.0, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICSCENE__ + 11, __PACMANII_BASICMAP__ + 11,
			75, 200, 2.0,
			8, 500, 10.0, 10.0, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICSCENE__ + 12, __PACMANII_BASICMAP__ + 12,
			75, 200, 2.0,
			9, 500, 10.0, 10.0, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICSCENE__ + 13, __PACMANII_BASICMAP__ + 13,
			75, 200, 2.0,
			10, 500, 10.0, 10.0, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICSCENE__ + 14, __PACMANII_BASICMAP__ + 14,
			75, 200, 4.0,
			11, 600, 10.0, 10.0, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICSCENE__ + 15, __PACMANII_BASICMAP__ + 15,
			75, 200, 4.0,
			10, 600, 10.0, 10.0, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICSCENE__ + 16, __PACMANII_BASICMAP__ + 16,
			75, 200, 4.0,
			9, 700, 10.0, 10.0, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICSCENE__ + 17, __PACMANII_BASICMAP__ + 17,
			75, 200, 4.0,
			8, 700, 10.0, 10.0, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICSCENE__ + 18, __PACMANII_BASICMAP__ + 18,
			75, 200, 3.0,
			7, 700, 10.0, 10.0, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		  { __PACMANII_BASICWORLD__, __PACMANII_BASICSCENE__ + 19, __PACMANII_BASICMAP__ + 19,
			75, 200, 2.0,
			8, 700, 10.0, 10.0, 
			c3, lH3,
			1.00, 0.87, 1.00, 0.87,
			0.95, 0.60, 0.40, 0.50,
			eC3 },
		};

	for (int i = 20; i < __PACMANII_MAXNUMBERLEVELS__; i++)
		lD.push_back // Repeats the last one...
			(PacManII::DataGame::LevelDefinition 
				(lD [19].worldTypeId (), 
				 lD [19].sceneTypeId () + (i - 20), // Varies...
				 lD [19].mapTypeId () + (i - 20), // Varies...
				 lD [19].pointsBall (), 
				 lD [19].pointsPowerBall (),
				 lD [19].secondsChasing (),
				 lD [19].bonusSymbolId (),
				 lD [19].bonusPoints (),
				 lD [19].secondsBonusSymbolToAppear (),
				 lD [19].secondsBonusSymbolToDisappear (),
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

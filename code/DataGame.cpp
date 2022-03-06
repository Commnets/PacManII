#include "DataGame.hpp"
#include "Monsters.hpp"
#include "Defs.hpp"

// ---
PACMAN::DataGame::LevelDefinition* PacManII::DataGame::LevelDefinition::cloneFor (int wId, int sId, int mId)
{
	return (new PacManII::DataGame::LevelDefinition 
		(wId, sId, mId,
		 pointsBall (), 
		 pointsPowerBall (),
		 secondsChasing (),
		 fruitConditions (),
		 scatterChaseCycles (),
		 leaveHomeConditions (),
		 pacmanSpeed (), 
		 pacmanSpeedWhenEatingDots (), 
		 pacmanSpeedWhenFrighting (), 
		 pacmanSpeedWhenEatingFrightingDots (),
		 ghostSpeed (), 
		 ghostSpeedWhenBeingFrighten (), 
		 ghostSpeedWhenExitingHome (), 
		 ghostSpeedWhenCrossingTunnel (),
		 elroyConditions (),
		 additionalMonsters ()));
}

// ---
PacManII::DataGame* PacManII::DataGame::trainingDataGame ()
{
	DataGame::LevelDefinition::ScatterChaseCycles sC =
		{ { 7.0f, 20.0f }, { 7.0f, 20.0f }, { 5.0f, 20.0f }, { 5.0f, 999999.0f } };
	DataGame::LevelDefinition::LeaveHomeConditions lH = 
		{ { 4.0f, { 0, 0, 21, 42 } }, { 4.0f, { 0, 5, 12, 22 } } };
	DataGame::LevelDefinition::ElroyConditions eC =
		{ { { true /** No more monsters. */, false }, { 10, 0 }, { 0.8f, 0.0f } }, 
		  { { true, false }, { 5, 0 }, { 0.85f, 0.0f } } };
	PacManII::DataGame::LevelDefinition::AdditionalMonster aM1 
		(__PACMANII_WORMYBASEENTITYID__, PacManII::Wormy::_NUMBER, QGAMES::MazeModel::_noPosition, 10, __BD 5.0, 600, 20); // Wormy 1...
	PacManII::DataGame::LevelDefinition::AdditionalMonster aM2
		(__PACMANII_WORMYBASEENTITYID__ + 1, PacManII::Wormy::_NUMBER, QGAMES::MazeModel::_noPosition, 20, __BD 7.0, 600, 20); // Wormy 2...

	// Just 4 levels to train...
	DataGame::LevelDefinitions result =
		{ 
		  // Level 1
		  new PacManII::DataGame::LevelDefinition (
		  { __PACMANII_EXTENDEDWORLD__, __PACMANII_BLOCKSCENES3MS1__, __PACMANII_MISSPACMAN1BLUEMAP__,
			15, 75, 8.0,
			{ { 0, 200, 40, 10.0, 0.25, 0 } }, 
			sC, lH,
			0.80, 0.71, 0.90, 0.79, 
			0.75, 0.50, 0.40, 0.40,
			eC,
			{ aM1, aM2 } }),
		  // Level 2
		  new PACMAN::DataGame::LevelDefinition (
		  { __PACMANII_EXTENDEDWORLD__, __PACMANII_BLOCKSCENES2MS2__, __PACMANII_MISSPACMAN2REDMAP__,
			15, 75, 8.0,
			{ { 1, 300, 40, 10.0, 0.25, 1 }, { 0, 300, 100, 10.0, 0.25, 2 } }, // two fruits... 
			sC, lH,
			0.90, 0.79, 0.95, 0.83, 
			0.85, 0.55, 0.40, 0.45,
			eC }),
		  // Level 3
		  new PACMAN::DataGame::LevelDefinition (
		  { __PACMANII_EXTENDEDWORLD__, __PACMANII_BLOCKSCENES2MS3__, __PACMANII_MISSPACMAN3GREENMAP__,
			15, 75, 8.0,
			{ { 2, 400, 50, 10.0, 0.25, 0 } },
			sC, lH, 
			0.90, 0.79, 0.95, 0.83, 
			0.85, 0.55, 0.40, 0.45,
			eC }),
		  // Level 4
		  new PACMAN::DataGame::LevelDefinition (
		  { __PACMANII_EXTENDEDWORLD__, __PACMANII_BLOCKSCENES2MS4__, __PACMANII_MISSPACMAN4YELLOWMAP__,
			15, 75, 8.0,
			{ { 2, 400, 50, 10.0, 0.25, 1 } },
			sC, lH, 
			0.90, 0.79, 0.95, 0.83, 
			0.85, 0.55, 0.40, 0.45,
			eC })
		};

	return (new PacManII::DataGame (result, 5000)); // Easier to get a new live, as we are testing!
}

// ---
PacManII::DataGame* PacManII::DataGame::missDataGame (int nL)
{
	// The same "difficuly" than classic game...
	PACMAN::DataGame::LevelDefinitions lD = PACMAN::DataGame::classicCoreLevels ();

	// There can't be less levels than the classical ones...
	assert (nL >= (int) lD.size ());

	// ...but the image of miss pacman (and chaning th different maps as the standard says...
	int lLD = (int) lD.size ();
	PacManII::DataGame::LevelDefinitions nLD;
	for (int i = 0; i < nL && i < __PACMAN_MAXNUMBERLEVELS__; i++)
	{
		// nB = Every three scenes, the type of miss pacman maze has to change.
		// tB = There are 4 types of miss pacman maze.
		// eNB = The number of scene within the block of every three.
		// nB = 0 && eNB = 1,2 
		// means that the very two first mazes are always miss pacman type 1 (tB = 0) and miss pacman type 2 (tB = 1) respectively

		int nB = (i + 1) / 3; // Block number (0, 1, 2, ...)
		int eNB = (i + 1) % 3; // The element in that block (0, 1, 2, 0, 1, 2, ...) 
		int tB = nB % 4; 

		// The number of scene to apply
		int nScn = 
			(nB == 0) 
				? ((eNB == 1) 
					? __PACMANII_BLOCKSCENES2MS1__  
					: __PACMANII_BLOCKSCENES2MS2__)
				: ((tB == 0) 
							? __PACMANII_BLOCKSCENES2MS1__ 
							: ((tB == 1) 
								? __PACMANII_BLOCKSCENES2MS2__ 
								: ((tB == 2)
									? __PACMANII_BLOCKSCENES2MS3__  
									: __PACMANII_BLOCKSCENES2MS4__))); 

		// The number of map to apply...
		int nM = 
			(nB == 0) 
				? ((eNB == 1) // The first two levels dont't repeat...
					? __PACMANII_MISSPACMAN1BLUEMAP__  
					: __PACMANII_MISSPACMAN2GREENMAP__)
				: ((tB == 0) 
							? __PACMANII_MISSPACMAN1BLUEMAP__ // type 1 always is blue...
							: ((tB == 1) 
								? __PACMANII_MISSPACMAN2REDMAP__ // type 2 always in green...
								: ((tB == 2)
									? __PACMANII_MISSPACMAN3GREENMAP__  // type 3 always in yellow...
									: __PACMANII_MISSPACMAN4YELLOWMAP__))); // ...and type 4 always in red...

		// Th first levels are like classic ones in terms of difficulty,...
		PACMAN::DataGame::LevelDefinition* oLD = (i < lLD) ? lD [i] : lD [lLD - 1];
		if (i < lLD)
		{
			nLD.push_back ( 
				new PACMAN::DataGame::LevelDefinition (
					__PACMANII_EXTENDEDWORLD__, 
					nScn, 
					nM,
					(*oLD).pointsBall (), 
					(*oLD).pointsPowerBall (),
					(*oLD).secondsChasing (),
					changeTimeSpeedAndPosition 
						((*oLD).fruitConditions (), 100.0f, 0.25f, 1 /** Meaning sequential if more than one */),
					(*oLD).scatterChaseCycles (),
					(*oLD).leaveHomeConditions (),
					(*oLD).pacmanSpeed (), 
					(*oLD).pacmanSpeedWhenEatingDots (), 
					(*oLD).pacmanSpeedWhenFrighting (), 
					(*oLD).pacmanSpeedWhenEatingFrightingDots (),
					(*oLD).ghostSpeed (), 
					(*oLD).ghostSpeedWhenBeingFrighten (), 
					(*oLD).ghostSpeedWhenExitingHome (), 
					(*oLD).ghostSpeedWhenCrossingTunnel (),
					(*oLD).elroyConditions ()));
		}
		else
			nLD.push_back (oLD -> cloneFor (__PACMANII_EXTENDEDWORLD__, nScn, nM));
	}

	// The classical levels are no longer used...
	for (auto i : lD)
		delete (i);

	return (new PacManII::DataGame (nLD, 10000));
}

// ---
PacManII::DataGame* PacManII::DataGame::mineDataGame (int nL)
{
	assert (nL >= 1);

	// Get the core levels...
	PACMAN::DataGame::LevelDefinitions lD = PACMAN::DataGame::classicCoreLevels ();

	// ...and introduce monsters scenes randomly just in a set of them (every 5)...
	int lLD = (int) lD.size ();
	int nELA = (lLD / 5); // Every 5 more or less...
	if (nELA > __PACMANII_MAXNUMBERXTENDEDLEVELS__) nELA = __PACMANII_MAXNUMBERXTENDEDLEVELS__;
	std::vector <int> wL (nELA, -1); // -1 means not used...
	PacManII::DataGame::LevelDefinition::AdditionalMonster aM1 
		(__PACMANII_WORMYBASEENTITYID__, PacManII::Wormy::_NUMBER, QGAMES::MazeModel::_noPosition, 100, __BD 5.0, 600, 12); // Wormy...
	for (int i = 0; i < nELA; i++) 
	{
		int pWL = rand () % (int) lD.size ();
		while (std::find (wL.begin (), wL.end (), pWL) != wL.end ())
			if (++pWL == (int) lD.size ()) pWL = 0; // If already used,, get the next...
		wL [i] = pWL; // Write it down as used!

		// Replace the selected level definition
		// by a new one with the same level of complexity, but inclusing monsters...
		PACMAN::DataGame::LevelDefinition* oLD = lD [pWL];
		lD [pWL] = new PacManII::DataGame::LevelDefinition
			(
				// Some conditions of the level, but with Wormy...
				__PACMANII_EXTENDEDWORLD__,
				__PACMANII_BLOCKSCENES1__, 
				__PACMAN_BASICBLUEMAP__ + ((rand () % __PACMAN_BASICMAPNUMBERCOLORS__) * 100), 
				(*oLD).pointsBall (), 
				(*oLD).pointsPowerBall (),
				(*oLD).secondsChasing (),
				(*oLD).fruitConditions (),
				(*oLD).scatterChaseCycles (),
				(*oLD).leaveHomeConditions (),
				(*oLD).pacmanSpeed (), 
				(*oLD).pacmanSpeedWhenEatingDots (), 
				(*oLD).pacmanSpeedWhenFrighting (), 
				(*oLD).pacmanSpeedWhenEatingFrightingDots (),
				(*oLD).ghostSpeed (), 
				(*oLD).ghostSpeedWhenBeingFrighten (), 
				(*oLD).ghostSpeedWhenExitingHome (), 
				(*oLD).ghostSpeedWhenCrossingTunnel (),
				(*oLD).elroyConditions (),
				{ aM1 }); // Same definition but including monsters...

		delete (oLD);
	}

	// Now it is time to add the rest of the levels...
	// Getting the attributes of a random one, but with a miss pacman aspect...
	for (int i = lLD; i < nL && i < __PACMAN_MAXNUMBERLEVELS__; i++)
	{
		PACMAN::DataGame::LevelDefinition* lDR = lD [rand () % (int) lD.size ()]; // Which one to repeat?

		// Number of type of miss map to choose...
		int nTMM = rand () % __PACMANII_MISSPACMANTYPEMAPS__;
		// Number of the color of that miss map to choose...
		int nCMM = rand () % __PACMAN_BASICMAPNUMBERCOLORS__;

		std::vector <int> tMM = (nTMM == 0) 
			? std::vector <int> ({ __PACMANII_MISSPACMAN1BLUEMAP__, __PACMANII_MISSPACMAN1REDMAP__, 
				__PACMANII_MISSPACMAN1GREENMAP__, __PACMANII_MISSPACMAN1YELLOWMAP__ })
			: (nTMM == 1) 
				? std::vector <int> ({ __PACMANII_MISSPACMAN2BLUEMAP__, __PACMANII_MISSPACMAN2REDMAP__, 
									   __PACMANII_MISSPACMAN2GREENMAP__, __PACMANII_MISSPACMAN2YELLOWMAP__ })
				: (nTMM == 2) 
					? std::vector <int> ({ __PACMANII_MISSPACMAN3BLUEMAP__, __PACMANII_MISSPACMAN3REDMAP__, 
										   __PACMANII_MISSPACMAN3GREENMAP__, __PACMANII_MISSPACMAN3YELLOWMAP__ })
					: std::vector <int> ({ __PACMANII_MISSPACMAN4BLUEMAP__, __PACMANII_MISSPACMAN4REDMAP__, 
										   __PACMANII_MISSPACMAN4GREENMAP__, __PACMANII_MISSPACMAN4YELLOWMAP__ });

		// Select the scene to be considered
		int nScn = ((dynamic_cast <PacManII::DataGame::LevelDefinition*> (lDR) == nullptr)
			? std::vector <int> 
				({ __PACMANII_BLOCKSCENES2MS1__, 
				   __PACMANII_BLOCKSCENES2MS2__, 
				   __PACMANII_BLOCKSCENES2MS3__, 
				   __PACMANII_BLOCKSCENES2MS4__ })[nTMM]
			: std::vector <int> 
				({ __PACMANII_BLOCKSCENES3MS1__, 
				   __PACMANII_BLOCKSCENES3MS2__, 
				   __PACMANII_BLOCKSCENES3MS3__, 
				   __PACMANII_BLOCKSCENES3MS4__ })[nTMM]);

		// Select the map to be considered
		int nMP = tMM [nCMM];

		// Clone the scene taking into account whether the original one has or nor monsters!
		if (dynamic_cast <PACMAN::DataGame::LevelDefinition*> (lDR) != nullptr)
		{
			lD.push_back 
				(new PACMAN::DataGame::LevelDefinition (
					__PACMANII_EXTENDEDWORLD__, 
					nScn, 
					nMP,
					(*lDR).pointsBall (), 
					(*lDR).pointsPowerBall (),
					(*lDR).secondsChasing (),
					changeTimeSpeedAndPosition 
						((*lDR).fruitConditions (), 100.0f, 0.25f, 1 /** Meaning sequential from 0 if more than 1. */),
					(*lDR).scatterChaseCycles (),
					(*lDR).leaveHomeConditions (),
					(*lDR).pacmanSpeed (), 
					(*lDR).pacmanSpeedWhenEatingDots (), 
					(*lDR).pacmanSpeedWhenFrighting (), 
					(*lDR).pacmanSpeedWhenEatingFrightingDots (),
					(*lDR).ghostSpeed (), 
					(*lDR).ghostSpeedWhenBeingFrighten (), 
					(*lDR).ghostSpeedWhenExitingHome (), 
					(*lDR).ghostSpeedWhenCrossingTunnel (),
					(*lDR).elroyConditions ()));
		}
		else
		{
			lD.push_back 
				(new PacManII::DataGame::LevelDefinition (
					__PACMANII_EXTENDEDWORLD__, 
					nScn, 
					nMP,
					(*lDR).pointsBall (), 
					(*lDR).pointsPowerBall (),
					(*lDR).secondsChasing (),
					changeTimeSpeedAndPosition 
						((*lDR).fruitConditions (), 100.0f, 0.25f, 1 /** Meaning sequential from 0 if more than 1. */),
					(*lDR).scatterChaseCycles (),
					(*lDR).leaveHomeConditions (),
					(*lDR).pacmanSpeed (), 
					(*lDR).pacmanSpeedWhenEatingDots (), 
					(*lDR).pacmanSpeedWhenFrighting (), 
					(*lDR).pacmanSpeedWhenEatingFrightingDots (),
					(*lDR).ghostSpeed (), 
					(*lDR).ghostSpeedWhenBeingFrighten (), 
					(*lDR).ghostSpeedWhenExitingHome (), 
					(*lDR).ghostSpeedWhenCrossingTunnel (),
					(*lDR).elroyConditions (),
					static_cast <PacManII::DataGame::LevelDefinition*> (lDR) ->additionalMonsters ()));
		}
	}

	// and now it is time to mix all of them...
	int nLD = (int) lD.size ();
	std::vector <int> wLR (nLD, -1); // -1 means not used...
	PACMAN::DataGame::LevelDefinitions lDR;
	std::vector <int> (nLD, -1); // -1 meaning not used...
	for (int i = 0; i < nLD; i++)
	{
		int pWL = rand () % nLD;
		while (std::find (wLR.begin (), wLR.end (), pWL) != wLR.end ())
			if (++pWL == nLD) pWL = 0; // If already used,, get the next...
		wLR [i] = pWL; // Write it down as used!

		lDR.push_back (lD [pWL]);
	}

	// The elements of LD can not be deleted because they are usd in LDR...

	return (new PacManII::DataGame (lDR, 10000));
}

// ---
PacManII::DataGame* PacManII::DataGame::hardDataGame (int nL)
{
	assert (nL >= 1);

	// Hardest conditions in clasical pacman...
	DataGame::LevelDefinition::ScatterChaseCycles sC =
		{ { 5.0f, 20.0f }, { 5.0f, 20.0f }, { 5.0f, 1037.0f }, { 0.0f, 999999.0f } };
	DataGame::LevelDefinition::LeaveHomeConditions lH = 
		{ { 3.0f, { 0, 0, 0, 0 } }, { 4.0f, { 0, 5, 12, 22 } } };
	DataGame::LevelDefinition::ElroyConditions eC =
		{ { { true, false }, { 20 /** No more monsters. */, 0 }, { 0.9f, 0.0f } }, 
		  { { true, false }, { 10, 0 }, { 0.95f, 0.0f } } };

	// Then...It is considered the max level of difficulty in the classical version
	PACMAN::DataGame::LevelDefinition* slD = // Just a template...
		  new PACMAN::DataGame::LevelDefinition (
			  { 0, 0, 0, // To define better later...
				15, 75, 6.0,
				{ { 0, 200, 40, 10.0, 0.0, 0 } }, 
				sC, lH,
				1.00, 0.87, 1.00, 0.87, 
				0.95, 0.60, 0.40, 0.50, 
				eC });

	// To add even more difficult additional monsters will be considered
	PacManII::DataGame::LevelDefinition::AdditionalMonster aM1 
		(__PACMANII_WORMYBASEENTITYID__, PacManII::Wormy::_NUMBER, QGAMES::MazeModel::_noPosition, 100, __BD 5.0, 600, 12);
	PacManII::DataGame::LevelDefinition::AdditionalMonster aM2 
		(__PACMANII_WORMYBASEENTITYID__, PacManII::Wormy::_NUMBER, QGAMES::MazeModel::_noPosition, 70, __BD 4.0, 600, 20);
	PacManII::DataGame::LevelDefinition::AdditionalMonster aM3 
		(__PACMANII_WORMYBASEENTITYID__, PacManII::Wormy::_NUMBER, QGAMES::MazeModel::_noPosition, 50, __BD 3.0, 600, 30);

	// In just this templates...
	PacManII::DataGame::LevelDefinition* eLD = // Just a template...
		  new PacManII::DataGame::LevelDefinition (
			  { 0, 0, 0, // To define better later...
				15, 75, 8.0,
				{ { 0, 200, 40, 10.0, 0.0, 0 } }, 
				sC, lH,
				1.00, 0.87, 1.00, 0.87, 
				0.95, 0.60, 0.40, 0.50,
				eC,
			    { aM1 }});

	// Now it is time to buid up the different levels...
	PACMAN::DataGame::LevelDefinitions lD;
	for (int i = 0; i < nL && i < __PACMAN_MAXNUMBERLEVELS__; i++)
	{
		// The first levels doesn't have monsters and are classic
		if (i < (3 * __PACMAN_BASICMAPNUMBERCOLORS__))
			lD.push_back (slD -> cloneFor (__PACMAN_BASICWORLD__, __PACMAN_BASICBLOCKSCENES__, __PACMAN_BASICGREENMAP__));
		else
		// Classic but with a monster...
		if (i == (3 * __PACMAN_BASICMAPNUMBERCOLORS__))
			lD.push_back (new PacManII::DataGame::LevelDefinition
				(
					__PACMANII_EXTENDEDWORLD__,
					__PACMANII_BLOCKSCENES1__,
					__PACMAN_BASICBLUEMAP__,
					(*slD).pointsBall (), 
					(*slD).pointsPowerBall (),
					(*slD).secondsChasing (),
					(*slD).fruitConditions (),
					(*slD).scatterChaseCycles (),
					(*slD).leaveHomeConditions (),
					(*slD).pacmanSpeed (), 
					(*slD).pacmanSpeedWhenEatingDots (), 
					(*slD).pacmanSpeedWhenFrighting (), 
					(*slD).pacmanSpeedWhenEatingFrightingDots (),
					(*slD).ghostSpeed (), 
					(*slD).ghostSpeedWhenBeingFrighten (), 
					(*slD).ghostSpeedWhenExitingHome (), 
					(*slD).ghostSpeedWhenCrossingTunnel (),
					(*slD).elroyConditions (),
					{ aM1 }));
		else
		// Classic but with a monster stronger...
		if (i == ((3 * __PACMAN_BASICMAPNUMBERCOLORS__) + 1))
			lD.push_back (new PacManII::DataGame::LevelDefinition
				(
					__PACMANII_EXTENDEDWORLD__,
					__PACMANII_BLOCKSCENES1__,
					__PACMAN_BASICGREENMAP__,
					(*slD).pointsBall (), 
					(*slD).pointsPowerBall (),
					(*slD).secondsChasing (),
					(*slD).fruitConditions (),
					(*slD).scatterChaseCycles (),
					(*slD).leaveHomeConditions (),
					(*slD).pacmanSpeed (), 
					(*slD).pacmanSpeedWhenEatingDots (), 
					(*slD).pacmanSpeedWhenFrighting (), 
					(*slD).pacmanSpeedWhenEatingFrightingDots (),
					(*slD).ghostSpeed (), 
					(*slD).ghostSpeedWhenBeingFrighten (), 
					(*slD).ghostSpeedWhenExitingHome (), 
					(*slD).ghostSpeedWhenCrossingTunnel (),
					(*slD).elroyConditions (),
					{ aM2 }));
		else
		// Classic but with a monster more stronger...
		if (i == ((3 * __PACMAN_BASICMAPNUMBERCOLORS__) + 2))
			lD.push_back (new PacManII::DataGame::LevelDefinition
				(
					__PACMANII_EXTENDEDWORLD__,
					__PACMANII_BLOCKSCENES1__,
					__PACMAN_BASICYELLOWMAP__,
					(*slD).pointsBall (), 
					(*slD).pointsPowerBall (),
					(*slD).secondsChasing (),
					(*slD).fruitConditions (),
					(*slD).scatterChaseCycles (),
					(*slD).leaveHomeConditions (),
					(*slD).pacmanSpeed (), 
					(*slD).pacmanSpeedWhenEatingDots (), 
					(*slD).pacmanSpeedWhenFrighting (), 
					(*slD).pacmanSpeedWhenEatingFrightingDots (),
					(*slD).ghostSpeed (), 
					(*slD).ghostSpeedWhenBeingFrighten (), 
					(*slD).ghostSpeedWhenExitingHome (), 
					(*slD).ghostSpeedWhenCrossingTunnel (),
					(*slD).elroyConditions (),
					{ aM3 }));
		else
		// Classic but with two monsters...
		if (i == ((3 * __PACMAN_BASICMAPNUMBERCOLORS__) + 3))
			lD.push_back (new PacManII::DataGame::LevelDefinition
				(
					__PACMANII_EXTENDEDWORLD__,
					__PACMANII_BLOCKSCENES1__,
					__PACMAN_BASICREDMAP__,
					(*slD).pointsBall (), 
					(*slD).pointsPowerBall (),
					(*slD).secondsChasing (),
					(*slD).fruitConditions (),
					(*slD).scatterChaseCycles (),
					(*slD).leaveHomeConditions (),
					(*slD).pacmanSpeed (), 
					(*slD).pacmanSpeedWhenEatingDots (), 
					(*slD).pacmanSpeedWhenFrighting (), 
					(*slD).pacmanSpeedWhenEatingFrightingDots (),
					(*slD).ghostSpeed (), 
					(*slD).ghostSpeedWhenBeingFrighten (), 
					(*slD).ghostSpeedWhenExitingHome (), 
					(*slD).ghostSpeedWhenCrossingTunnel (),
					(*slD).elroyConditions (),
					{ aM1, aM2 }));
		// From now onwards the screens are Ms. Pacman...
		else
		{
			int nB = i / 3; // Block number (0, 1, 2, ...)
			int eNB = i % 3; // The element in that block (0, 1, 2, 0, 1, 2, ...) 
			int tB = nB % 4; 

			// The number of scene to apply
			int nScn = ( 
				(nB == 0) 
					? ((eNB == 1) 
						? std::vector <int> ({ __PACMANII_BLOCKSCENES2MS1__, __PACMANII_BLOCKSCENES3MS1__ })   
						: std::vector <int> ({ __PACMANII_BLOCKSCENES2MS2__, __PACMANII_BLOCKSCENES3MS2__ }))
					: ((tB == 0) 
						? std::vector <int> ({ __PACMANII_BLOCKSCENES2MS2__, __PACMANII_BLOCKSCENES3MS2__ }) 
								: ((tB == 1) 
									? std::vector <int> ({ __PACMANII_BLOCKSCENES2MS2__, __PACMANII_BLOCKSCENES3MS2__ }) 
									: ((tB == 2)
										? std::vector <int> ({ __PACMANII_BLOCKSCENES2MS3__, __PACMANII_BLOCKSCENES3MS3__ }) 
										: std::vector <int> ({__PACMANII_BLOCKSCENES2MS4__, __PACMANII_BLOCKSCENES2MS4__ }))))
				)[__1IN2PROBABLE__]; 

			// The number of map to apply...
			int nM = 
				(nB == 0) 
					? ((eNB == 1) // The first two levels dont't repeat...
						? __PACMANII_MISSPACMAN1BLUEMAP__  
						: __PACMANII_MISSPACMAN2GREENMAP__)
					: ((tB == 0) 
								? __PACMANII_MISSPACMAN1BLUEMAP__ // type 1 always is blue...
								: ((tB == 1) 
									? __PACMANII_MISSPACMAN2REDMAP__ // type 2 always in green...
									: ((tB == 2)
										? __PACMANII_MISSPACMAN3GREENMAP__  // type 3 always in yellow...
										: __PACMANII_MISSPACMAN4YELLOWMAP__))); // ...and type 4 always in red...
																				
			// With or without monsters... (no so difficult)
			lD.push_back (
				new PACMAN::DataGame::LevelDefinition (
					__PACMANII_EXTENDEDWORLD__, 
					nScn, 
					nM,
					(*slD).pointsBall (), 
					(*slD).pointsPowerBall (),
					(*slD).secondsChasing (),
					changeTimeSpeedAndPosition 
						((*slD).fruitConditions (), 100.0f, 0.25f, 1 /** Meaning sequential if more than 1. */),
					(*slD).scatterChaseCycles (),
					(*slD).leaveHomeConditions (),
					(*slD).pacmanSpeed (), 
					(*slD).pacmanSpeedWhenEatingDots (), 
					(*slD).pacmanSpeedWhenFrighting (), 
					(*slD).pacmanSpeedWhenEatingFrightingDots (),
					(*slD).ghostSpeed (), 
					(*slD).ghostSpeedWhenBeingFrighten (), 
					(*slD).ghostSpeedWhenExitingHome (), 
					(*slD).ghostSpeedWhenCrossingTunnel (),
					(*slD).elroyConditions ()));
		}
	}

	// No longer needed...
	delete (slD);
	delete (eLD);

	return (new PacManII::DataGame (lD, 10000));
}

// ---
PacManII::DataGame::DataGame (const PacManII::DataGame& d)
	: PACMAN::DataGame 
		([=](const PACMAN::DataGame::LevelDefinitions& ls) -> PACMAN::DataGame::LevelDefinitions 
			{ 
				PACMAN::DataGame::LevelDefinitions result; 
				for (auto i : ls)
				{
					// Uses the right copy constructor...
					if (dynamic_cast <PacManII::DataGame::LevelDefinition*> (i) != nullptr)
						result.push_back (new PacManII::DataGame::LevelDefinition 
							(*(static_cast <PacManII::DataGame::LevelDefinition*> (i))));
					else
						result.push_back (new PACMAN::DataGame::LevelDefinition (*i));
				}

				return (result);
			}(d.levels ()), 
		 d.everyToGetAnExtraLive ())
{
	// Nothing special to build up here...
}

// ---
PACMAN::DataGame::LevelDefinition::FruitConditions PacManII::DataGame::changeTimeSpeedAndPosition
	(const PACMAN::DataGame::LevelDefinition::FruitConditions& fC, double t, double spd, int p)
{
	PACMAN::DataGame::LevelDefinition::FruitConditions result;

	int ct = 0;
	for (auto i : fC)
		result.push_back (
			PACMAN::DataGame::LevelDefinition::FruitCondition (
				i.bonusSymbolId (),
				i.bonusPoints (),
				i.numberBallsEatenToAppear (),
				t,
				spd,
				(p == 0) ? i.positionId () : ct++));

	return (result);
}

/**	
 *	@file	
 *	File: main.cpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Forni�s (Community Networks) \n
 *	Creation Date: 21/08/2021 \n
 *	Description: Main programm for PacManII game
 *	Versions: 1.0 Initial
 */

#include "stdafx.h"
#include "Defs.hpp"
#include "Game.hpp"

using namespace PacManII;

#ifndef _CONSOLE
#include <SDL.h>
#ifdef __cplusplus
#define C_LINKAGE "C"
#else
#define C_LINKAGE
#endif /* __cplusplus */
#if _MSC_VER >= 1900
extern C_LINKAGE FILE __iob_func[3] = { *stdin,*stdout,*stderr };
#endif
extern C_LINKAGE int main(int argc, char *argv[])
#else
int _tmain (int argc, char *argv [])
#endif /* _CONSOLE */
{
#ifdef NDEBUG
	// In the release version, the resources are in a zip file
	// This instruction reads them and store in temporaly files when needed!
	// The resource reader is deleted at the end, and all the temporal file with them!
	QGAMES::InZipResourceReader rR 
		(QGAMES::ConfigurationFile (std::string (__GAME_CONFIGURATIONFILE__)).
			property (std::string (__GAME_DATADIRPROPERTYNAME__)));
#endif

	DataGame::LevelDefinition::ScatterChaseCycles c1 =
		{ { 7, 20 }, { 7, 20 }, { 5, 20 }, { 5, std::numeric_limits <unsigned int>::max () } };
	DataGame::LevelDefinition::ScatterChaseCycles c2 =
		{ { 7, 20 }, { 7, 20 }, { 5, 1033 }, { 0, std::numeric_limits <unsigned int>::max () } };
	DataGame::LevelDefinition::ScatterChaseCycles c3 =
		{ { 5, 20 }, { 5, 20 }, { 5, 1037 }, { 0, std::numeric_limits <unsigned int>::max () } };
	DataGame::LevelDefinitions lD =
		{ { __PACMANII_WORLD__, __PACMANII_BASICSCENE__, __PACMANII_BASICMAP__,
			50, 200, 5.0,
			0, 200, 10.0, 10.0, 
			c1, 
			0.80, 0.71, 0.90, 0.79, 0.75, 0.50, 0.40 },
		  { __PACMANII_WORLD__, __PACMANII_BASICSCENE__ + 1, __PACMANII_BASICMAP__ + 1,
			50, 200, 5.0,
			1, 300, 10.0, 10.0, 
			c2, 
			0.90, 0.79, 0.95, 0.83, 0.85, 0.55, 0.45 },
		  { __PACMANII_WORLD__, __PACMANII_BASICSCENE__ + 2, __PACMANII_BASICMAP__ + 2,
			50, 200, 5.0,
			2, 300, 10.0, 10.0,
			c2,
			0.90, 0.79, 0.95, 0.83, 0.85, 0.55, 0.45 },
		  { __PACMANII_WORLD__, __PACMANII_BASICSCENE__ + 3, __PACMANII_BASICMAP__ + 3,
			50, 200, 5.0,
			3, 300, 10.0, 10.0, 
			c2, 
			0.90, 0.79, 0.95, 0.83, 0.85, 0.55, 0.45},
		  { __PACMANII_WORLD__, __PACMANII_BASICSCENE__ + 4, __PACMANII_BASICMAP__ + 4,
			50, 200, 5.0,
			4, 400, 10.0, 10.0, 
			c3, 
			1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50 },
		  { __PACMANII_WORLD__, __PACMANII_BASICSCENE__ + 5, __PACMANII_BASICMAP__ + 5,
			50, 200, 5.0,
			5, 400, 10.0, 10.0, 
			c3, 
			1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50 },
		  { __PACMANII_WORLD__, __PACMANII_BASICSCENE__ + 6, __PACMANII_BASICMAP__ + 6,
			50, 200, 5.0,
			6, 400, 10.0, 10.0, 
			c3, 
			1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50 },
		  { __PACMANII_WORLD__, __PACMANII_BASICSCENE__ + 7, __PACMANII_BASICMAP__ + 7,
			50, 200, 5.0,
			6, 400, 10.0, 10.0, 
			c3, 
			1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50 },
		  { __PACMANII_WORLD__, __PACMANII_BASICSCENE__ + 8, __PACMANII_BASICMAP__ + 8,
			50, 200, 5.0,
			5, 500, 10.0, 10.0, 
			c3, 1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50 },
		  { __PACMANII_WORLD__, __PACMANII_BASICSCENE__ + 9, __PACMANII_BASICMAP__ + 9,
			50, 200, 5.0,
			4, 500, 10.0, 10.0, 
			c3, 
			1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50 },
		  { __PACMANII_WORLD__, __PACMANII_BASICSCENE__ + 10, __PACMANII_BASICMAP__ + 10,
			50, 200, 5.0,
			7, 500, 10.0, 10.0, 
			c3, 
			1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50 },
		  { __PACMANII_WORLD__, __PACMANII_BASICSCENE__ + 11, __PACMANII_BASICMAP__ + 11,
			50, 200, 5.0,
			8, 500, 10.0, 10.0, 
			c3, 
			1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50 },
		  { __PACMANII_WORLD__, __PACMANII_BASICSCENE__ + 12, __PACMANII_BASICMAP__ + 12,
			50, 200, 5.0,
			9, 500, 10.0, 10.0, 
			c3, 
			1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50 },
		  { __PACMANII_WORLD__, __PACMANII_BASICSCENE__ + 13, __PACMANII_BASICMAP__ + 13,
			50, 200, 5.0,
			10, 500, 10.0, 10.0, 
			c3, 
			1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50 },
		  { __PACMANII_WORLD__, __PACMANII_BASICSCENE__ + 14, __PACMANII_BASICMAP__ + 14,
			50, 200, 5.0,
			11, 600, 10.0, 10.0, 
			c3,
			1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50 },
		  { __PACMANII_WORLD__, __PACMANII_BASICSCENE__ + 15, __PACMANII_BASICMAP__ + 15,
			50, 200, 5.0,
			10, 600, 10.0, 10.0, 
			c3, 
			1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50 },
		  { __PACMANII_WORLD__, __PACMANII_BASICSCENE__ + 16, __PACMANII_BASICMAP__ + 16,
			50, 200, 5.0,
			9, 700, 10.0, 10.0, 
			c3, 
			1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50 },
		  { __PACMANII_WORLD__, __PACMANII_BASICSCENE__ + 17, __PACMANII_BASICMAP__ + 17,
			50, 200, 5.0,
			8, 700, 10.0, 10.0, 
			c3, 
			1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50 },
		  { __PACMANII_WORLD__, __PACMANII_BASICSCENE__ + 18, __PACMANII_BASICMAP__ + 18,
			50, 200, 5.0,
			7, 700, 10.0, 10.0, 
			c3, 
			1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50 },
		  { __PACMANII_WORLD__, __PACMANII_BASICSCENE__ + 19, __PACMANII_BASICMAP__ + 19,
			50, 200, 5.0,
			8, 700, 10.0, 10.0, 
			c3, 
			1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50 }
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
				 lD [19].pacmanSpeed (), 
				 lD [19].pacmanSpeedWhenEatingDots (), 
				 lD [19].pacmanSpeedWhenFrighting (), 
				 lD [19].pacmanSpeedWhenEatingFrightingDots (),
				 lD [19].ghostSpeed (), 
				 lD [19].ghostSpeedWhenBeingFrighten (), 
				 lD [19].ghostSpeedWhenCrossingTunnel ()));
			 

	DataGame dt (lD, 15000);
	Game game (dt);
	game.setLinkDrawToFrameRate (true); 
	game.setPreLoad (false); 
	game.exec ();

	return (0);
}

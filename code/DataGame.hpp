/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: DataGame.hpp \n
 *	Game: PacManII \n
 *	Author: Ignacio Cea Forniés (Community Networks) \n
 *	Creation Date: 22/08/2021 \n
 *	Description: To include the definition data of the PacManII. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_DATAGAME__
#define __PACMANII_DATAGAME__

#include "Defs.hpp"

namespace PacManII
{
	/** General data defining the game.
		All información is describe in the pacman dossier, 
		written down by Jamie Pittman: https://www.gamasutra.com/view/feature/3938/the_pacman_dossier.php?print=1. 
		The class has been built to be used in const expressions. */
	class DataGame final
	{
		public:
		/** In pacman things like the relative speed among monsters and pacman, how much pacman delays when eats a ball,
			or many other things, varies depending on the level. */
		class LevelDefinition final
		{
			public:
			/** Any monster consideres one of the four corners as his/her home.
				During a game, the bad guys moves periodically between an attacking mode (chase) and a more relaxed one (scatter). 
				The time between that two moments depends on the level. In a specific level 4 cycles minimum happens. */
			class ScatterChaseCycle final
			{
				public:
				constexpr ScatterChaseCycle ()
					: _secondsScatter1 (7), _secondsChase1 (20), 
					  _secondsScatter2 (7), _secondsChase2 (20),
					  _secondsScatter3 (7), _secondsChase3 (20),
					  _secondsScatter4 (7), _secondsChase4 (-1) // Infinite in the last one.
							{ }

				constexpr ScatterChaseCycle 
						(int sS1, int sC1, int sS2, int sC2, int sS3, int sC3, int sS4, int sC4 = -1 /** meaning infinite */)
					: _secondsScatter1 (sS1), _secondsChase1 (sC1),
					  _secondsScatter2 (sS2), _secondsChase2 (sC2),
					  _secondsScatter3 (sS3), _secondsChase3 (sC3),
					  _secondsScatter4 (sS4), _secondsChase4 (sC4)
							{ }

				constexpr ScatterChaseCycle (const ScatterChaseCycle&) = default;

				constexpr ScatterChaseCycle& operator = (const ScatterChaseCycle&) = default;

				constexpr int secondsScatter (int i) const
							{ return ((i == 1) ? _secondsScatter1 
												: ((i == 2) ? _secondsScatter2 
															: ((i == 3) ? _secondsScatter3 : _secondsScatter4))); }
				constexpr int secondsChase (int i) const
							{ return ((i == 1) ? _secondsChase1 
												: ((i == 2) ? _secondsChase2 
															: ((i == 3) ? _secondsChase3 : _secondsChase4))); }

				private:
				int _secondsScatter1, _secondsChase1;
				int _secondsScatter2, _secondsChase2;
				int _secondsScatter3, _secondsChase3;
				int _secondsScatter4, _secondsChase4;
			};

			constexpr LevelDefinition ()
				: _worldTypeId (__PACMANII_WORLD__),
				  _sceneTypeId (__PACMANII_BASICSCENE__),
				  _mapTypeId (__PACMANII_BASICMAP__),
				  _bonusSymbolId (0),
				  _bonusPoints (100),
				  _scatterChaseCycle (/* the default one. */),
				  _pacmanSpeed (1.0f), 
				  _pacmanSpeedWhenEatingDots (1.0f), 
				  _pacmanSpeedWhenFrighting (1.0f), 
				  _pacmanSpeedWhenEatingFrightingDots (1.0f),
				  _ghostSpeeed (1.0f), 
				  _ghostSpeedWhenBeingFrighten (1.0f), 
				  _ghostSpeedWhenCrossingTunnel (1.0f)
							{ }

			constexpr LevelDefinition (int wT, int sT, int mT, int bS, int bP, const ScatterChaseCycle& sC, 
					double pS, double pED, double pWF, double pWEFD, double gS, double gWF, double gWT)
				: _worldTypeId (wT),
				  _sceneTypeId (sT),
				  _mapTypeId (mT),
				  _bonusSymbolId (bS),
				  _bonusPoints (bP),
				  _scatterChaseCycle (sC),
				  _pacmanSpeed (pS), 
				  _pacmanSpeedWhenEatingDots (pED), 
				  _pacmanSpeedWhenFrighting (pWF), 
				  _pacmanSpeedWhenEatingFrightingDots (pWEFD),
				  _ghostSpeeed (gS), 
				  _ghostSpeedWhenBeingFrighten (gWF), 
				  _ghostSpeedWhenCrossingTunnel (gWT)
							{ }

			constexpr LevelDefinition (const LevelDefinition&) = default;

			constexpr LevelDefinition& operator = (const LevelDefinition&) = default;

			constexpr int worldTypeId () const
							{ return (_worldTypeId); }
			constexpr int sceneTypeId () const 
							{ return (_sceneTypeId); }
			constexpr int mapTypeId () const
							{ return (_mapTypeId); }
			constexpr int bonusSymbolId () const 
							{ return (_bonusSymbolId); }
			constexpr int bonusPoints () const 
							{ return (_bonusPoints); }
			constexpr const ScatterChaseCycle& scatterChaseCycle () const 
							{ return (_scatterChaseCycle); }
			constexpr double pacmanSpeed () const 
							{ return (_pacmanSpeed); } 
			constexpr double pacmanSpeedWhenEatingDots () const 
							{ return (_pacmanSpeedWhenEatingDots); }
			constexpr double pacmanSpeedWhenFrighting () const 
							{ return (_pacmanSpeedWhenFrighting); }
			constexpr double pacmanSpeedWhenEatingFrightingDots () const 
							{ return (_pacmanSpeedWhenEatingFrightingDots); } 
			constexpr double ghostSpeeed () const 
							{ return (_ghostSpeeed); }
			constexpr double ghostSpeedWhenBeingFrighten () const 
							{ return (_ghostSpeedWhenBeingFrighten); }
			constexpr double ghostSpeedWhenCrossingTunnel () const 
							{ return (_ghostSpeedWhenCrossingTunnel); }

			private:
			int _worldTypeId; // Usually it will be the same for all scenes in the game...
			int _sceneTypeId;
			int _mapTypeId;
			int _bonusSymbolId;
			int _bonusPoints;
			ScatterChaseCycle _scatterChaseCycle;
			double _pacmanSpeed; /** Percentage eover the maximum possible. */
			double _pacmanSpeedWhenEatingDots;
			double _pacmanSpeedWhenFrighting;
			double _pacmanSpeedWhenEatingFrightingDots;
			double _ghostSpeeed;
			double _ghostSpeedWhenBeingFrighten;
			double _ghostSpeedWhenCrossingTunnel;
		};

		inline constexpr DataGame ();

		constexpr DataGame (const DataGame& d) = default;

		constexpr DataGame& operator = (const DataGame& d) = default;

		constexpr const LevelDefinition& levelDefinition (int lD) const
							{ return (_levels [lD]); }

		constexpr const int numberLevels () const
							{ return (__PACMANII_MAXNUMBERLEVELS__); }

		LevelDefinition _levels [__PACMANII_MAXNUMBERLEVELS__];
	};

	// ---
	inline constexpr DataGame::DataGame ()
		: _levels ()
	{
		DataGame::LevelDefinition::ScatterChaseCycle sC1 (7, 20, 7, 20, 5, 20, 5); // Only in level 1...
		DataGame::LevelDefinition::ScatterChaseCycle sC2 (7, 20, 7, 20, 5, 20, 5); // From level 2 to 4...
		DataGame::LevelDefinition::ScatterChaseCycle sC3 (7, 20, 7, 20, 5, 20, 5); // From level 5 onwards...

		_levels [0]  = DataGame::LevelDefinition (__PACMANII_WORLD__, __PACMANII_BASICSCENE__, __PACMANII_BASICMAP__,
					0, 200, sC1, 0.80, 0.71, 0.90, 0.79, 0.75, 0.50, 0.40);
		_levels [2]  = DataGame::LevelDefinition (__PACMANII_WORLD__, __PACMANII_BASICSCENE__, __PACMANII_BASICMAP__,
					0, 200, sC2, 0.90, 0.79, 0.95, 0.83, 0.85, 0.55, 0.45);
		_levels [3]  = DataGame::LevelDefinition (__PACMANII_WORLD__, __PACMANII_BASICSCENE__, __PACMANII_BASICMAP__,
					0, 200, sC2, 0.90, 0.79, 0.95, 0.83, 0.85, 0.55, 0.45);
		_levels [4]  = DataGame::LevelDefinition (__PACMANII_WORLD__, __PACMANII_BASICSCENE__, __PACMANII_BASICMAP__,
					0, 200, sC2, 0.90, 0.79, 0.95, 0.83, 0.85, 0.55, 0.45);
		_levels [5]  = DataGame::LevelDefinition (__PACMANII_WORLD__, __PACMANII_BASICSCENE__, __PACMANII_BASICMAP__,
					0, 200, sC3, 1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50);
		_levels [6]  = DataGame::LevelDefinition (__PACMANII_WORLD__, __PACMANII_BASICSCENE__, __PACMANII_BASICMAP__,
					0, 200, sC3, 1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50);
		_levels [7]  = DataGame::LevelDefinition (__PACMANII_WORLD__, __PACMANII_BASICSCENE__, __PACMANII_BASICMAP__,
					0, 200, sC3, 1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50);
		_levels [8]  = DataGame::LevelDefinition (__PACMANII_WORLD__, __PACMANII_BASICSCENE__, __PACMANII_BASICMAP__,
					0, 200, sC3, 1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50);
		_levels [9]  = DataGame::LevelDefinition (__PACMANII_WORLD__, __PACMANII_BASICSCENE__, __PACMANII_BASICMAP__,
					0, 200, sC3, 1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50);
		_levels [10] = DataGame::LevelDefinition (__PACMANII_WORLD__, __PACMANII_BASICSCENE__, __PACMANII_BASICMAP__,
					0, 200, sC3, 1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50);
		_levels [11] = DataGame::LevelDefinition (__PACMANII_WORLD__, __PACMANII_BASICSCENE__, __PACMANII_BASICMAP__,
					0, 200, sC3, 1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50);
		_levels [12] = DataGame::LevelDefinition (__PACMANII_WORLD__, __PACMANII_BASICSCENE__, __PACMANII_BASICMAP__,
					0, 200, sC3, 1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50);
		_levels [13] = DataGame::LevelDefinition (__PACMANII_WORLD__, __PACMANII_BASICSCENE__, __PACMANII_BASICMAP__,
					0, 200, sC3, 1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50);
		_levels [14] = DataGame::LevelDefinition (__PACMANII_WORLD__, __PACMANII_BASICSCENE__, __PACMANII_BASICMAP__,
					0, 200, sC3, 1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50);
		_levels [15] = DataGame::LevelDefinition (__PACMANII_WORLD__, __PACMANII_BASICSCENE__, __PACMANII_BASICMAP__,
					0, 200, sC3, 1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50);
		_levels [16] = DataGame::LevelDefinition (__PACMANII_WORLD__, __PACMANII_BASICSCENE__, __PACMANII_BASICMAP__,
					0, 200, sC3, 1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50);
		_levels [17] = DataGame::LevelDefinition (__PACMANII_WORLD__, __PACMANII_BASICSCENE__, __PACMANII_BASICMAP__,
					0, 200, sC3, 1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50);
		_levels [18] = DataGame::LevelDefinition (__PACMANII_WORLD__, __PACMANII_BASICSCENE__, __PACMANII_BASICMAP__,
					0, 200, sC3, 1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50);
		_levels [19] = DataGame::LevelDefinition (__PACMANII_WORLD__, __PACMANII_BASICSCENE__, __PACMANII_BASICMAP__,
					0, 200, sC3, 1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50);
		_levels [20] = DataGame::LevelDefinition (__PACMANII_WORLD__, __PACMANII_BASICSCENE__, __PACMANII_BASICMAP__,
					0, 200, sC3, 1.00, 0.87, 1.00, 0.87, 0.95, 0.60, 0.50);

		// From level 21 onwards, all are the same...
		_levels [21] = DataGame::LevelDefinition (__PACMANII_WORLD__, __PACMANII_BASICSCENE__, __PACMANII_BASICMAP__,
					0, 200, sC3, 0.90, 0.79, 0.90, 0.79, 0.95, 0.95, 0.50);
		for (int i = 22; i < __PACMANII_MAXNUMBERLEVELS__; _levels [i++] = _levels [21]);
	}
}

#endif
  
// End of the file
/*@}*/

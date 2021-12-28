/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: DataGame.hpp \n
 *	Game: PacManII \n
 *	Author: Ignacio Cea Forniés (Community Networks) \n
 *	Creation Date: 22/08/2021 \n
 *	Description: To keep the main paramters to run a Pacman game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_DATAGAME__
#define __PACMANII_DATAGAME__

#include "Defs.hpp"

namespace PacManII
{
	/** 
		General data defining the game.
		All información is describe in the pacman dossier, 
		written down by Jamie Pittman: https://www.gamasutra.com/view/feature/3938/the_pacman_dossier.php?print=1.
	  */
	class DataGame final
	{
		public:
		class LevelDefinition final
		{
			public:
			class ScatterChaseCycle final
			{
				public:
				ScatterChaseCycle ()
					: _secondsScatter (7), _secondsChase (20)
							{ }

				ScatterChaseCycle (unsigned int sS, unsigned int sC)
					: _secondsScatter(sS), _secondsChase (sC)
							{ }

				ScatterChaseCycle (const ScatterChaseCycle&) = default;

				ScatterChaseCycle& operator = (const ScatterChaseCycle&) = default;

				int secondsScatter () const
							{ return (_secondsScatter); }
				int secondsChase () const
							{ return (_secondsChase); }

				private:
				/** Seconds running away from pacman to home,
					and seconds chasing pacman. */
				const unsigned int _secondsScatter, _secondsChase;
			};

			typedef std::vector <ScatterChaseCycle> ScatterChaseCycles;

			LevelDefinition ()
				: _worldTypeId (__PACMANII_WORLD__), _sceneTypeId (__PACMANII_BASICSCENE__), _mapTypeId (__PACMANII_BASICMAP__),
				  _pointsBall (50), _pointsPowerBall (200), _secondsChasing (1.0f),
				  _bonusSymbolId (0), _bonusPoints (100), _secondsBonusToAppear (10.0f), _secondsBonusToDisappear (10.0f),
				  _scatterChaseCycles ({ ScatterChaseCycle () }),
				  _pacmanSpeed (1.0f), _pacmanSpeedWhenEatingDots (1.0f), 
						_pacmanSpeedWhenFrighting (1.0f), _pacmanSpeedWhenEatingFrightingDots (1.0f),
				  _ghostSpeed (1.0f), _ghostSpeedWhenBeingFrighten (1.0f), 
						_ghostSpeedWhenCrossingTunnel (1.0f)
							{ }

			LevelDefinition (int wT, int sT, int mT, 
					int pB, int pPB, double mSC, 
					int bS, int bP, double sBA, double sBD, 
					const ScatterChaseCycles& sC, 
					double pS, double pED, double pWF, double pWEFD, double gS, double gWF, double gWT)
				: _worldTypeId (wT), _sceneTypeId (sT), _mapTypeId (mT),
				  _pointsBall (pB), _pointsPowerBall (pPB), _secondsChasing (mSC),
				  _bonusSymbolId (bS), _bonusPoints (bP), _secondsBonusToAppear (sBA), _secondsBonusToDisappear (sBD),
				  _scatterChaseCycles (sC),
				  _pacmanSpeed (pS), _pacmanSpeedWhenEatingDots (pED), 
						_pacmanSpeedWhenFrighting (pWF), _pacmanSpeedWhenEatingFrightingDots (pWEFD),
				  _ghostSpeed (gS), _ghostSpeedWhenBeingFrighten (gWF), 
						_ghostSpeedWhenCrossingTunnel (gWT)
							{ assert (_scatterChaseCycles.size () != 0); }

			LevelDefinition (const LevelDefinition&) = default;

			LevelDefinition& operator = (const LevelDefinition&) = default;

			int worldTypeId () const
							{ return (_worldTypeId); }
			int sceneTypeId () const 
							{ return (_sceneTypeId); }
			int mapTypeId () const
							{ return (_mapTypeId); }
			int pointsBall () const
							{ return (_pointsBall); }
			int pointsPowerBall () const
							{ return (_pointsPowerBall); }
			double secondsChasing () const
							{ return (_secondsChasing); }
			int bonusSymbolId () const 
							{ return (_bonusSymbolId); }
			int bonusPoints () const 
							{ return (_bonusPoints); }
			double secondsBonusSymbolToAppear () const
							{ return (_secondsBonusToAppear); }
			double secondsBonusSymbolToDisappear () const
							{ return (_secondsBonusToDisappear); }
			const ScatterChaseCycles& scatterChaseCycles () const
							{ return (_scatterChaseCycles); }
			const ScatterChaseCycle& scatterChaseCycle (unsigned int n) const 
							{ return (n > _scatterChaseCycles.size () 
								? _scatterChaseCycles [_scatterChaseCycles.size () - 1] : _scatterChaseCycles [n]); }
			double pacmanSpeed () const 
							{ return (_pacmanSpeed); } 
			double pacmanSpeedWhenEatingDots () const 
							{ return (_pacmanSpeedWhenEatingDots); }
			double pacmanSpeedWhenFrighting () const 
							{ return (_pacmanSpeedWhenFrighting); }
			double pacmanSpeedWhenEatingFrightingDots () const 
							{ return (_pacmanSpeedWhenEatingFrightingDots); } 
			double ghostSpeed () const 
							{ return (_ghostSpeed); }
			double ghostSpeedWhenBeingFrighten () const 
							{ return (_ghostSpeedWhenBeingFrighten); }
			double ghostSpeedWhenCrossingTunnel () const 
							{ return (_ghostSpeedWhenCrossingTunnel); }

			private:
			/** The word, the scene and the map of the level. */
			const int _worldTypeId; 
			const int _sceneTypeId;
			const int _mapTypeId;
			/** The number of points for eating a ball */
			const int _pointsBall;
			/** Idem but for a power ball. */
			const int _pointsPowerBall;
			/** The seconds chasing. */
			const double _secondsChasing;
			/** Id of the symbol that will appears randomly at pacman's home after several of seconds. */
			const int _bonusSymbolId;
			/** How many point will eating it give you? */
			const int _bonusPoints;
			/** The number of seconds before the bonus symbol to appear. */
			const double _secondsBonusToAppear;
			/** The number of senconds the bonus symbol is maintained in the screen. */
			const double _secondsBonusToDisappear;
			/** The cycles beetween scatting and chaseing */
			const ScatterChaseCycles _scatterChaseCycles;
			// Proportion eover the maximum possible...
			/** Usual Pacman speed. */
			const double _pacmanSpeed; 
			/** Pacman's speed when eating dots. */
			const double _pacmanSpeedWhenEatingDots;
			/** Pacman's speed when chaesing monsters after eeating a poweer dot. */
			const double _pacmanSpeedWhenFrighting;
			/** Pacman's sped when bot eating and cheasing happens at the same time. */
			const double _pacmanSpeedWhenEatingFrightingDots;
			/** Usual Monster speed. */
			const double _ghostSpeed;
			/** Monster's speed when are frighten because Pacman has eaten a power dot. */
			const double _ghostSpeedWhenBeingFrighten;
			/** Monster's speed when they are facind the tunnel. */
			const double _ghostSpeedWhenCrossingTunnel;
		};

		typedef std::vector <LevelDefinition> LevelDefinitions;

		DataGame (const LevelDefinitions& l = { LevelDefinition () }, int sL = 15000)
			: _levels (l),
			  _everyToGetAnExtraLive (sL)
							{ }

		DataGame (const DataGame& d) = default;

		DataGame& operator = (const DataGame& d) = default;

		/** Parameter starts in 1. 
			The last on always repeat. */
		const LevelDefinition& levelDefinition (unsigned int lD) const
							{ return ((lD <= _levels.size ()) ? _levels [lD - 1] : _levels [_levels.size () - 1]); }

		const int everyToGetAnExtraLive () const
							{ return (_everyToGetAnExtraLive); }

		const unsigned int numberLevelsDefined () const
							{ return ((unsigned int) _levels.size ()); }

		private:
		const LevelDefinitions _levels;
		const int _everyToGetAnExtraLive;
	};
}

#endif
  
// End of the file
/*@}*/

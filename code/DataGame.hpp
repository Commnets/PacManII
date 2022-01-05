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
			/** In one level there can be several of these: One per round. 
				Every elroy condition has one different value of its attributes per monster. 
				If it is requested an attribute for a monster number not defined, the last one available will be returned. 
				So, it is not needed to create th elroy condition for all monsters in the game, 
				but only for the ones an elroy condition has to be taken into account. 
				A no useful elroy condition will be the one defining the number of remaining balls before being activated to 0. */
			class ElroyCondition final
			{
				public:
				ElroyCondition () 
					: _chasingWhenScatter ({ true, false }), 
					  _numberBallsLeft ({ 10, 0 /** The ky. */ }), 
					  _ghostSpeed ({ 0.8f, 0.0f })
							{ }

				ElroyCondition (const std::vector <bool>& cS, const std::vector <int> nL, const std::vector <double>& mS)
					: _chasingWhenScatter (cS), _numberBallsLeft (nL), _ghostSpeed (mS)
							{ }

				ElroyCondition (const ElroyCondition&) = default;

				ElroyCondition& operator = (const ElroyCondition&) = default;

				int chasingWhenScatter (int nM) const
							{ return ((nM >= (int) (_chasingWhenScatter.size () - 1)) 
										? _chasingWhenScatter [(int) _chasingWhenScatter.size () - 1] 
										: _chasingWhenScatter [nM]); }
				int numberBallLeft (int nM) const
							{ return ((nM >= (int) (_numberBallsLeft.size () - 1)) 
										? _numberBallsLeft [(int) _numberBallsLeft.size () - 1] 
										: _numberBallsLeft [nM]); }
				double ghostSpeed (int nM) const
							{ return ((nM >= (int) (_ghostSpeed.size () - 1)) 
										? _ghostSpeed [(int) _ghostSpeed.size () - 1] 
										: _ghostSpeed [nM]); }

				private:
				const std::vector <bool> _chasingWhenScatter;
				const std::vector <int> _numberBallsLeft;
				const std::vector <double> _ghostSpeed;
			};

			typedef std::vector <ElroyCondition> ElroyConditions;

			/** In a level, there can be several of these: One per round. 
				Every leave home condition can have different values for its attributs per number of monster.
				If it is requested an attribute for a monster number not defined, the last one available will be returned. 
				So, it is not needed to create the leave home condition for all monsters in the game, 
				but only for the ones an leave home condition has to be taken into account. 
				A no useful condition will be the one defining the number of balls eaten before moving to 0. */
			class LeaveHomeCondition final
			{
				public:
				LeaveHomeCondition () 
					: _maxSecondsToLeave (__BD 3),
					  _dotsToLeavePerMonster ({ 0, 0, 30, 60 })
							{ }

				LeaveHomeCondition (double sL, const std::vector <int>& dL)
					: _maxSecondsToLeave (sL),
					  _dotsToLeavePerMonster (dL)
							{ }	

				LeaveHomeCondition (const LeaveHomeCondition&) = default;

				LeaveHomeCondition& operator = (const LeaveHomeCondition&) = default;

				double maxSecondsToLeave () const
							{ return (_maxSecondsToLeave); }
				int dotsToLeaveFor (int nM) const
							{ return ((nM >= (int) (_dotsToLeavePerMonster.size () - 1)) 
										? _dotsToLeavePerMonster [(int) _dotsToLeavePerMonster.size () - 1] 
										: _dotsToLeavePerMonster [nM]); }

				private:
				const double _maxSecondsToLeave;
				const std::vector <int> _dotsToLeavePerMonster;
			};

			typedef std::vector <LeaveHomeCondition> LeaveHomeConditions;

			/** The scatter - chase cycle is the time spend between switching from scatter to chase or the opposite. 
				It can be defined as many as needed. 
				If a not defined cycle is requested, the last one will be returned. */
			class ScatterChaseCycle final
			{
				public:
				ScatterChaseCycle ()
					: _secondsScatter (__BD 7), _secondsChase (__BD 20)
							{ }

				ScatterChaseCycle (QGAMES::bdata sS, QGAMES::bdata sC)
					: _secondsScatter(sS), _secondsChase (sC)
							{ }

				ScatterChaseCycle (const ScatterChaseCycle&) = default;

				ScatterChaseCycle& operator = (const ScatterChaseCycle&) = default;

				double secondsScatter () const
							{ return (_secondsScatter); }
				double secondsChase () const
							{ return (_secondsChase); }

				private:
				/** Seconds running away from pacman to home,
					and seconds chasing pacman. */
				const double _secondsScatter, _secondsChase;
			};

			typedef std::vector <ScatterChaseCycle> ScatterChaseCycles;

			LevelDefinition ()
				: _worldTypeId (__PACMANII_BASICWORLD__), _sceneTypeId (__PACMANII_BASICSCENE__), _mapTypeId (__PACMANII_BASICMAP__),
				  _pointsBall (50), _pointsPowerBall (200), _secondsChasing (1.0f),
				  _bonusSymbolId (0), _bonusPoints (100), _secondsBonusToAppear (10.0f), _secondsBonusToDisappear (10.0f),
				  _scatterChaseCycles ({ ScatterChaseCycle () }), _leaveHomeConditions (),
				  _pacmanSpeed (1.0f), _pacmanSpeedWhenEatingDots (1.0f), 
						_pacmanSpeedWhenFrighting (1.0f), _pacmanSpeedWhenEatingFrightingDots (1.0f),
				  _ghostSpeed (1.0f), _ghostSpeedWhenBeingFrighten (1.0f), 
						_ghostSpeedWhenExitingHome (1.0f), _ghostSpeedWhenCrossingTunnel (1.0f),
				  _elroyConditions ({ })
							{ }

			LevelDefinition (int wT, int sT, int mT, 
					int pB, int pPB, double mSC, 
					int bS, int bP, double sBA, double sBD, 
					const ScatterChaseCycles& sC, const LeaveHomeConditions& lHC,
					double pS, double pED, double pWF, double pWEFD, 
					double gS, double gWF, double gWE, double gWT,
					const ElroyConditions& eC)
				: _worldTypeId (wT), _sceneTypeId (sT), _mapTypeId (mT),
				  _pointsBall (pB), _pointsPowerBall (pPB), _secondsChasing (mSC),
				  _bonusSymbolId (bS), _bonusPoints (bP), _secondsBonusToAppear (sBA), _secondsBonusToDisappear (sBD),
				  _scatterChaseCycles (sC), _leaveHomeConditions (lHC),
				  _pacmanSpeed (pS), _pacmanSpeedWhenEatingDots (pED), 
						_pacmanSpeedWhenFrighting (pWF), _pacmanSpeedWhenEatingFrightingDots (pWEFD),
				  _ghostSpeed (gS), _ghostSpeedWhenBeingFrighten (gWF), 
						_ghostSpeedWhenExitingHome (gWE), _ghostSpeedWhenCrossingTunnel (gWT),
				  _elroyConditions (eC)
							{ assert (!_scatterChaseCycles.empty () && 
									  !_elroyConditions.empty ()); }

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
			const ScatterChaseCycle& scatterChaseCycle (int n) const 
							{ return (n >= ((int) _scatterChaseCycles.size () - 1) 
								? _scatterChaseCycles [(int) _scatterChaseCycles.size () - 1] : _scatterChaseCycles [n]); }
			const LeaveHomeConditions& leaveHomeConditions () const
							{ return (_leaveHomeConditions); }
			const LeaveHomeCondition& leaveHomeCondition (int nR) const
							{ return (nR >= (int) (_leaveHomeConditions.size () - 1) 
								? _leaveHomeConditions [(int) _leaveHomeConditions.size () - 1] : _leaveHomeConditions [nR]); }
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
			double ghostSpeedWhenExitingHome () const
							{ return (_ghostSpeedWhenExitingHome); }
			double ghostSpeedWhenCrossingTunnel () const 
							{ return (_ghostSpeedWhenCrossingTunnel); }
			const ElroyConditions& elroyConditions () const
							{ return (_elroyConditions); }
			const ElroyCondition& elroyCondition (int nR) const
							{ return (nR >= (int) (_elroyConditions.size () - 1) 
								? _elroyConditions [(int) _elroyConditions.size () - 1] : _elroyConditions [nR]); }

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
			/** The number of seconds the bonus symbol is maintained in the screen. */
			const double _secondsBonusToDisappear;
			/** The cycles beetween scatting and chaseing */
			const ScatterChaseCycles _scatterChaseCycles;
			/** The conditions to manag when th monstrs leave home. */
			const LeaveHomeConditions _leaveHomeConditions;

			// Related wth the speed:
			// Proportion over the maximum possible...
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
			/** Monster speed whn exiting/entering home */
			const double _ghostSpeedWhenExitingHome;
			/** Monster's speed when they are facind the tunnel. */
			const double _ghostSpeedWhenCrossingTunnel;
			/** Potential elroy conditions. */
			const ElroyConditions _elroyConditions;
		};

		typedef std::vector <LevelDefinition> LevelDefinitions;

		static DataGame standardDataGame ();

		DataGame (const LevelDefinitions& l = { LevelDefinition () }, int sL = 15000)
			: _levels (l),
			  _everyToGetAnExtraLive (sL)
							{ }

		DataGame (const DataGame& d) = default;

		DataGame& operator = (const DataGame& d) = default;

		int firstNumberLevelDefinitionForScene (int sId) const;

		/** Parameter starts in 0. 
			The last on always repeat. */
		const LevelDefinition& levelDefinition (int lD) const
							{ return ((lD >= (int) _levels.size ()) 
								? _levels [(int) _levels.size () - 1] : _levels [lD - 1]); }

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

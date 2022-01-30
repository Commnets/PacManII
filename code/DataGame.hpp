/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: DataGame.hpp \n
 *	Game: PacManII \n
 *	Author: Ignacio Cea Forni�s (Community Networks) \n
 *	Creation Date: 30/01/2022 \n
 *	Description: To manipulate th data of the PacManII game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_DATAGAME__
#define __PACMANII_DATAGAME__

#include <PacManLike/pcinclude.hpp>

namespace PacManII
{
	class DataGame final : public PACMAN::DataGame
	{
		public:
		class LevelDefinition final : public PACMAN::DataGame::LevelDefinition
		{
			public:
			class AdditionalMonster final
			{
				public:
				AdditionalMonster ()
					: _monsterId (0),
					  _monsterNumber (0),
					  _position (QGAMES::MazeModel::_noPosition),
					  _numberBallsEatenToAppear (100),
					  _maxSecondsToAppear (__BD 5.0)
							{ }

				AdditionalMonster (int mId, int mN, const QGAMES::MazeModel::PositionInMaze& p, int nB, QGAMES::bdata sM)
					: _monsterId (mId), 
					  _monsterNumber (mN),
					  _position (p),
					  _numberBallsEatenToAppear (nB),
					  _maxSecondsToAppear (sM)
							{ }

				AdditionalMonster (const AdditionalMonster&) = default;

				AdditionalMonster& operator = (const AdditionalMonster&) = default;

				int monsterId () const
							{ return (_monsterId); }
				int monsterNumber () const
							{ return (_monsterNumber); }
				const QGAMES::MazeModel::PositionInMaze& position () const
							{ return (_position); }
				int numberBallsEatenToAppear () const
							{ return (_numberBallsEatenToAppear); }
				QGAMES::bdata maxSecondsToAppear () const
							{ return (_maxSecondsToAppear); }
				
				private:
				int _monsterId;
				int _monsterNumber;
				QGAMES::MazeModel::PositionInMaze _position;
				int _numberBallsEatenToAppear;
				QGAMES::bdata _maxSecondsToAppear;

			};

			typedef std::vector <AdditionalMonster> AdditionalMonsters;

			LevelDefinition ()
				: PACMAN::DataGame::LevelDefinition (),
				  _additionalMonsters ()
							{ }

			LevelDefinition (int wT, int sT, int mT, 
					int pB, int pPB, double mSC, 
				    const FruitConditions& fC, 
					const ScatterChaseCycles& sC, 
					const LeaveHomeConditions& lHC,
					double pS, double pED, double pWF, double pWEFD, 
					double gS, double gWF, double gWE, double gWT,
					const ElroyConditions& eC)
				: PACMAN::DataGame::LevelDefinition (wT, sT, mT, pB, pPB, mSC, fC, sC, lHC, pS, pED, pWF, pWEFD, gS, gWF, gWE, gWT, eC),
				  _additionalMonsters ()
							{ }

			LevelDefinition (const LevelDefinition&) = default;

			LevelDefinition& operator = (const LevelDefinition&) = default;

			const AdditionalMonsters& additionalMonsters () const
							{ return (_additionalMonsters); }
			const AdditionalMonster& additionalMonster (int nM) const
							{ return ((nM >= (int) _additionalMonsters.size ()) 
								? _additionalMonsters [(int) _additionalMonsters.size () - 1] : _additionalMonsters [nM]); }

			private:
			AdditionalMonsters _additionalMonsters;
		};

		static DataGame* mineDataGame (int nL = __PACMAN_MAXNUMBERLEVELS__);
		static DataGame* hardDataGame (int nL = __PACMAN_MAXNUMBERLEVELS__);

		DataGame (const LevelDefinitions& l = { new LevelDefinition () }, int sL = 15000)
			: PACMAN::DataGame (l, sL)
							{ }

		DataGame (const DataGame& d);

		DataGame& operator = (const DataGame& d) = delete;
	};
}

#endif
  
// End of the file
/*@}*/
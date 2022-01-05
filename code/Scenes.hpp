/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: Scenes.hpp \n
 *	Game: PacManII \n
 *	Author: Ignacio Cea Forniés (Community Networks) \n
 *	Creation Date: 22/08/2021 \n
 *	Description: To manipulate the scenes of the game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_SCENES__
#define __PACMANII_SCENES__

#include "Defs.hpp"
#include "Artists.hpp"
#include "Maze.hpp"
#include <advancedArcade/adinclude.hpp>

namespace PacManII
{
	class MonsterSceneActionBlock;

	/** Basic Scene of the game. \n
		Maps can be only PacManII ones. \n
		The Scene has a siren in the background. \n
		The rate of the siren goes up as map has less balls to eat. \n
		When the a power ball is eaten the scene turns into a chaising state. What to do depends on the definitions later.
		The game can be extended in terms of scenes. */
	class Scene : public QGAMES::Scene
	{
		public:
		Scene (int c, const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ());

		/** The pacman scene can not delegate thes methods in the standard ones.
			The status of th switxhs and countrs will be then saved, and this is not what we want to happen.
			Everytime a screen is lunched back the only important information to keep is the status of the ball aeaten. */
		virtual QGAMES::SetOfOpenValues runtimeValues () const override;
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg) override;

		/** Know whether the scene is or not already working. 
			This is mainly for counters and switches consideration. */
		void setClapperBoard (bool o)
							{ _clapperBoard = o; }
		bool clapperBoard () const
							{ return (_clapperBoard); }

		/** To know whether the scene is or not in chasing mode. */
		bool chasingMode () const
							{ return (onOffSwitch (_SWITCHMONSTERSCHASING) -> isOn ()); }

		/** To blink. */
		void startBlinking (QGAMES::bdata bT, int nB);
		bool isBlinking () const;
		void stopBlinking ();

		/** To control and manage the ball status of the scene. */
		int maxNumberBallsToEat () const;
		int numberBallsEaten () const;
		int numberBallsEatenRound () const
							{ return (_numberBallsEaten); }
		std::string ballsEatenStatus () const;
		void setBallsEatenStatus (const std::string& st);
		int numberRound () const
							{ return (_numberRound); }

		/** The siren will be played when the actual rate changes or when it was force. */
		void playSiren (bool f = false);
		void stopSiren ();

		virtual void initialize () override;
		virtual void updatePositions () override;
		virtual void finalize () override;

		virtual void processEvent (const QGAMES::Event& evnt) override;

		protected:
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters () override
							{ return (new Counters); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches)
		virtual QGAMES::OnOffSwitches* createOnOffSwitches () override
							{ return (new OnOffSwitches); }

		// Implementation
		/** To switch on/off, the chasing status of the monsters */
		void setMonstersChasing (bool c);
		/** To switch on/off, the threaten status of the monsters. */
		void setMonstersBeingThreaten (bool o);
		/** The monsters to blink. */
		void setMonstersBlinking (bool b, int bp);
		/** To launch another monster. 
			Returns true, when at least one monster was launched. */
		bool launchNextMonster ();

		protected:
		bool _clapperBoard;

		/** The counters and the switches. */
		static const int _COUNTERMONSTERSCHASING = 0;
		static const int _COUNTERMONSTERSRUNNINGAWAY = 1;
		static const int _COUNTERMONSTERCHASINGCYCLES = 2;
		static const int _COUNTERMONSTERSBEINGTHREATEN = 3;
		static const int _COUNTERMONSTERSTIMETOLEAVEHOME = 4;
		static const int _COUNTERMONSTERTOLEAVE = 5;
		static const int _SWITCHMONSTERSCHASING = 0;
		static const int _SWITCHMONSTERSBEINGTHREATEN = 1;
		static const int _SWITCHALLMONSTERSMOVING = 2;
		static const int _SWITCHMONSTERSBLINKING = 3;

		// Implementation
		/** The percentage of the maze cleaned. */
		QGAMES::bdata _percentageCleaned;
		/* Number balls to eate, eaten and eaten in this round. */
		mutable int _totalNumberBallsToEat, _totalNumberBallsEaten, _numberBallsEaten;
		/** To indicta whether it is the first time to execute the updatePositions method. \n
			First time that method is executed, it is set who refers to whom. */
		bool _firstTimeUpdateMethod;
		/** To indicate th number of round. */
		bool _numberRound;
		/** A list of the action blocks relatd with monsters.
			It is usefull later. */
		std::vector <MonsterSceneActionBlock*> _monsterActionBlocks;
		/** The list of monster numbers already started. */
		std::vector <int> _numberMonstersMoving;

		/** Used to play the siren. */
		enum class SirenRate { _NORMAL = 0, _FAST = 1, _VERYFAST = 2 } _sirenRate;
	};
}

#include "StandardScenes.hpp"

#endif
  
// End of the file
/*@}*/
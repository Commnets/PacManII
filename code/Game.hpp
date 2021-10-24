/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: Game.hpp \n
 *	Game: PacManII \n
 *	Author: Ignacio Cea Forniés (Community Networks) \n
 *	Creation Date: 22/08/2021 \n
 *	Description: To manipulate the very basics of the PacManII game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_GAME__
#define __PACMANII_GAME__

#include "Defs.hpp"
#include "DataGame.hpp"
#include "InputHandler.hpp"
#include "Builders.hpp"
#include "Artists.hpp"
#include "ScoreObjects.hpp"

#include <advancedArcade/adinclude.hpp>

namespace PacManII
{
	/** The game class. */
	class Game final : public QGAMES::AdvancedArcadeGame
	{
		public:
		class Conf final : public QGAMES::AdvancedArcadeGame::Conf
		{
			public:
			Conf (int nP, int mL, int dL);

			virtual Configuration* clone () const override
							{ Conf* result = new Conf (*this); 
							  result -> cloneImplementation (this); 
							  return (result); }

			/** To know and set the default and scene used for the players in the game. */
			int defaultLevel () const
							{ return (_defaultLevel); }
			void setDefaultLevel (int dL)
							{ _defaultLevel = dL; }

			// To Read & Set the internal attributes
			// nP from 1 to the max number of users
			const std::string& playerName (int nP) const 
							{ assert (nP > 0 && nP <= (int) _playerName.size ());
							  return (_playerName [nP - 1]); }
			void setPlayerName (int nP, const std::string& n)
							{ assert (nP > 0 && nP <= (int) _playerName.size ());
							  _playerName [nP - 1] = n; }

			int score (int nP) const 
							{ assert (nP > 0 && nP <= (int) _score.size ());
							  return (_score [nP - 1]); }
			void setScore (int nP, int s)
							{ assert (nP > 0 && nP <= (int) _score.size ());
							  _score [nP - 1] = (s >= 0) ? s : 0; }

			int playerSeconds (int nP) const
							{ assert (nP > 0 && nP <= (int) _playerSeconds.size ());
							  return (_playerSeconds [nP - 1]); }
			void setPlayerSeconds (int nP, int s) // The value can't ever be less than 0
							{ assert (nP > 0 && nP <= (int) _playerSeconds.size ());
							  _playerSeconds [nP - 1] = (s >= 0) ? s : 0; }

			int level (int nP) const // The current level the player is in...
							{ assert (nP > 0 && nP <= (int) _level.size ());
							  return (_level [nP - 1]); }
			void setLevel (int nP, int l)
							{ assert (nP > 0 && nP <= (int) _level.size ());
							  _level [nP - 1] = (l >= 0) ? l : 0; }
			bool levelCompleted (int nP, int nL) const // ...but different story is what level has been completed!
							{ assert (nP > 0 && nP <= (int) _levelCompleted.size ());
							  std::map <int, bool>::const_iterator i = _levelCompleted [nP - 1].find (nL);
							  return ((i != _levelCompleted [nP - 1].end ()) ? (*i).second : false); }
			void setLevelCompleted (int nP, int nL, bool c = true)
							{ assert (nP > 0 && nP <= (int) _levelCompleted.size ());
							  _levelCompleted [nP - 1][nL] = c; }
			std::string levelBallsEaten (int nP, int nL) const // ...necessary to know which balls have already been aeaten in this level
							{ assert (nP > 0 && nP <= (int) _levelBallsEaten.size ());
							  std::map <int, std::string>::const_iterator i = _levelBallsEaten [nP - 1].find (nL);
							  return ((i != _levelBallsEaten [nP - 1].end ()) ? (*i).second : std::string (__NULL_STRING__)); }
			void setLevelBallsEaten (int nP, int nL, const std::string& st)
							{ assert (nP > 0 && nP <= (int) _levelBallsEaten.size ());
							  _levelBallsEaten [nP - 1][nL] = st; }

			/** To change the hall of fame. */
			void actualizeHallOfFame ();

			protected:
			virtual void adjustToPlayers (int nP) override;

			virtual void cfgToStream (std::ostringstream& oS) override;
			virtual void cfgFromStream (std::istringstream& iS) override;

			protected:
			int _maxLives;
			int _defaultLevel;
			std::vector <std::string> _playerName;
			std::vector <int> _score;
			std::vector <int> _playerSeconds;
			std::vector <int> _level;
			std::vector <std::map <int, bool>> _levelCompleted;
			std::vector <std::map <int, std::string>> _levelBallsEaten;
		};

		Game ()
			: QGAMES::AdvancedArcadeGame (),
			  _theDataGame (),
			  _lives (nullptr),
			  _timeCounter (nullptr),
			  _scoreCounter (nullptr),
			  _playerName (nullptr),
			  _showFPS (false), // Not to show by default...
			  _detectCollisions (false), // Not detected by default...
			  _tmxAddsOnMapBuilder (nullptr)
							{ }

		~Game ()
							{ removeScoreObjects (); }

		constexpr const DataGame& dataGame () const
							{ return (_theDataGame); }

		/** To activate or desactivate the detection of collisions. */
		void setDetectCollisions (bool dC)
							{ _detectCollisions = dC; }
	
		/** The name of the player. */
		const std::string& playerName (int nP = -1) const
							{ return (((Game::Conf*) configuration ()) -> 
								playerName ((nP == -1) ? currentPlayer () : nP)); }
		void setPlayerName (const std::string& pN, int nP = -1)
							{ ((Game::Conf*) configuration ()) -> 
								setPlayerName ((nP == -1) ? currentPlayer () : nP, pN); }

		/** The manage the score. */
		int score (int nP = -1) const
							{ return (((Game::Conf*) configuration ()) -> 
								score ((nP == -1) ? currentPlayer () : nP)); }
		void setScore (int s, int nP = -1)
							{ ((Game::Conf*) configuration ()) -> 
								setScore ((nP == -1) ? currentPlayer () : nP, s); }
		void addScore (int a, int nP = -1)
							{ setScore (score (nP) + a, nP); }

		/** The number of seconds the player is taking. */
		int playerSeconds (int nP = -1) const
							{ return (((Game::Conf*) configuration ()) -> 
								playerSeconds ((nP == -1) ? currentPlayer () : nP)); }
		void setPlayerSeconds (int s, int nP = -1)
							{ ((Game::Conf*) configuration ()) -> 
								setPlayerSeconds ((nP == -1) ? currentPlayer () : nP, s); }
		void addToPlayerSeconds (int s, int nP = -1) // Add a number of seconds to the current value (it can be negative)
							{ setPlayerSeconds (playerSeconds (nP) + s, nP); }

		/** The level. */
		int level (int nP = -1) const
							{ return (((Game::Conf*) configuration ()) -> 
								level ((nP == -1) ? currentPlayer () : nP)); }
		void setLevel (int l, int nP = -1); 
		// Fix the world, scene and map to play in...
		// Do not use either setCurrentWorld or setCurrentScene (inherited from Conf)
		void setNextLevel (int nP = -1) // Based on the previous one...
							{ setLevel (level (nP) + 1, nP); }
		const DataGame::LevelDefinition& levelDefinition (int nL = -1) const
							{ return (_theDataGame.levelDefinition ((nL == -1) ? level () : nL)); }

		/** Managing the levels totally completed. */
		bool levelCompleted (int nL, int nP = -1) const
							{ return (((Conf*) configuration ()) -> 
								levelCompleted ((nP == -1) ? currentPlayer () : nP, nL)); }
		void setLevelCompleted (int nL, bool c = true, int nP = -1)
							{ ((Conf*) configuration ()) -> 
								setLevelCompleted ((nP == -1) ? currentPlayer () : nP, nL, c); }
		bool stopsAfterCurrentLevel () const;
		bool isCurrentLevelLast () const;

		/** Managing the ball status per level.
				In this string the cell (number) with a ball already eaten will appear! */
		std::string levelBallsEaten (int nL, int nP = -1) const
							{ return (((Conf*) configuration ()) -> 
								levelBallsEaten ((nP == -1) ? currentPlayer () : nP, nL)); }
		void setLevelBallsEaten (int nL, const std::string& st, int nP = -1)
							{ ((Conf*) configuration ()) -> 
								setLevelBallsEaten ((nP == -1) ? currentPlayer () : nP, nL, st); }

		/** Actualize the hall of fame, with the last results. */
		void actualizeHallOfFame ()
							{ ((Game::Conf*) configuration ()) -> actualizeHallOfFame (); }

		/** To switch the visualization of the fPS. */
		void setShowFPS (bool s);

		/** To get a reference to the main characteres of the game. */
		PacMan* pacman (int nE = 0) 
							{ assert (nE >= 0 && nE < __PACMANII_NUMBERENTITIES__); 
							  return (dynamic_cast <PacMan*> (character (__PACMANII_PACMANBASEENTITYID__ + nE))); }
		Inky* inky (int nE = 0) 
							{ assert (nE >= 0 && nE < __PACMANII_NUMBERENTITIES__); 
							  return (dynamic_cast <Inky*> (character (__PACMANII_INKYBASEENTITYID__ + nE))); }
		Blinky* blinky (int nE = 0) 
							{ assert (nE >= 0 && nE < __PACMANII_NUMBERENTITIES__); 
							  return (dynamic_cast <Blinky*> (character (__PACMANII_BLINKYBASEENTITYID__ + nE))); }
		Pinky* pinky (int nE = 0) 
							{ assert (nE >= 0 && nE < __PACMANII_NUMBERENTITIES__); 
							  return (dynamic_cast <Pinky*> (character (__PACMANII_PINKYBASEENTITYID__ + nE))); }
		Clyde* clyde (int nE = 0) 
							{ assert (nE >= 0 && nE < __PACMANII_NUMBERENTITIES__); 
							  return (dynamic_cast <Clyde*> (character (__PACMANII_CLYDEBASEENTITYID__ + nE))); }

		virtual void addScoreObjects () override;
		virtual void removeScoreObjects () override;

		virtual void pauseGame () override;
		virtual void continueGame () override;

		virtual bool drawGameBeforeState () const override
							{ return (true); }

		virtual void processEvent (const QGAMES::Event& evnt) override;

		virtual void detectCollisions () override;

		/** To initilize the entities used by the game and the worlds. 
			It has to be invoked from the InitializePacManIIGameState o directly. 
			But it has to be invoked anyway before starting the game. 
			It can be overloaded. */
		virtual void initializeEntitiesAndWorlds ();

		const TMXMapBuilder* tmxAddsOnMapBuilder () const
							{ return (_tmxAddsOnMapBuilder); }
		TMXMapBuilder* tmxAddsOnMapBuilder () 
							{ return (_tmxAddsOnMapBuilder); }

		private:
		virtual QGAMES::TextBuilder* createTextBuilder () override
							{ return (new TextBuilder ()); }
		virtual QGAMES::GUISystemBuilder* createGUISystemBuilder ()
							{ return (new GUISystemBuilder (game () -> parameter (__GAME_PROPERTYGUISYSTEMFILE__), 
								game () -> formBuilder ())); }
		virtual QGAMES::GameStateBuilder* createGameStateBuilder ();
		virtual QGAMES::EntityBuilder* createEntityBuilder () override
							{ return (new EntityBuilder (parameter (__GAME_PROPERTYENTITIESFILE__), 
									formBuilder (), movementBuilder ())); }
		virtual QGAMES::MovementBuilder* createMovementBuilder () override
							{ return (new MovementBuilder (parameter (__GAME_PROPERTYMOVEMENTSFILE__))); }
		virtual QGAMES::MapBuilder* createMapBuilder () override;
		virtual QGAMES::WorldBuilder* createWorldBuilder () override
							{ return (new WorldBuilder (parameter (__GAME_PROPERTYWORLDSFILE__), 
								mapBuilder ())); }
		virtual QGAMES::CharacterControlStepsMonitorBuilder* createCharacterMonitorBuilder () override
							{ return (new QGAMES::CharacterControlStepsMonitorBuilder 
								(parameter (__GAME_PROPERTUCHARACTERMONITORFILE__))); }

		virtual Configuration* createConfiguration () override;

		virtual QGAMES::InputHandler* createInputHandler () override
							{ return (implementation () -> createInputHandler (new PacManII::InputHandlerBehaviour ())); }

		virtual std::string defaultParameter (const std::string& p) const override;

		virtual void initialize () override;
		virtual void finalize () override;

		private:
		const DataGame _theDataGame;

		// The scoreObjects...
		Lives* _lives;
		TimeCounter* _timeCounter;
		ScoreCounter* _scoreCounter;
		FixText* _playerName;

		// Implmentation
		/** To show or not the FPS in the right bottom corner. */
		bool _showFPS;
		/** To control whether it makes or nor sense to detect collisions. */
		bool _detectCollisions;
		/** A reference to the adds on to create the maps. */
		TMXMapBuilder* _tmxAddsOnMapBuilder;
	};
}

#endif
  
// End of the file
/*@}*/
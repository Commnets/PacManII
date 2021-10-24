/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: Worlds.hpp \n
 *	Game: PacManII \n
 *	Author: Ignacio Cea Forniés (Community Networks) \n
 *	Creation Date: 22/08/2021 \n
 *	Description: To manipulate the worlds, scene and maps of the game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_WORLDS__
#define __PACMANII_WORLDS__

#include "Defs.hpp"
#include "Artists.hpp"
#include <advancedArcade/adinclude.hpp>

namespace PacManII
{
	/** Basic world of the game. 
		Scenes can be only PacManII ones. */
	class World final : public QGAMES::World
	{
		public:
		World (int c, const QGAMES::Scenes& s, const QGAMES::WorldProperties& p = QGAMES::WorldProperties ());

		virtual QGAMES::SetOfOpenValues runtimeValues () const override;
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg) override
							{ _fullConfPerPlayer = cfg; }

		/** To blink. */
		void startBlinking (QGAMES::bdata bT, int nB);
		bool isBlinking () const;
		void stopBlinking ();

		/** To manage the ball status of the world. */
		int maxNumberBallsToEat () const;
		int numberBallsEaten () const;
		std::string ballsEatenStatus () const;
		void setBallsEatenStatus (const std::string& st);

		/** To play the sire. */
		void playSiren (bool f = false);
		void stopSiren ();

		virtual void initialize () override;
		virtual void finalize () override;

		protected:
		/** 
		  * The structure of the runtimeValues hasn't take into account many players. 
		  *	To this method just return the current information. 
		  *	This method invokes the runtimeValues of the parent.
		  * But it can be overloaded to add additional infomation 
		  * in the same way usually is done with runtimeValues (). 
		  */
		QGAMES::SetOfOpenValues currentPlayerRuntimeValues () const
							{ return (QGAMES::World::runtimeValues ()); }

		protected:
		mutable QGAMES::SetOfOpenValues _fullConfPerPlayer;
	};

	/** Basic Scene of the game. 
		Maps can be only PacManII ones. 
		The Scene has a siren in the background.
		The rate of the siren goes up as the pacman has less ball to eat. */
	class Scene final : public QGAMES::Scene
	{
		public:
		Scene (int c, const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ());

		/** To blink. */
		void startBlinking (QGAMES::bdata bT, int nB);
		bool isBlinking () const;
		void stopBlinking ();

		/** To manage the ball status of the scene. */
		int maxNumberBallsToEat () const;
		int numberBallsEaten () const;
		std::string ballsEatenStatus () const;
		void setBallsEatenStatus (const std::string& st);

		/** The siren will be played when the actual rate changes or when it was force. */
		void playSiren (bool f = false);
		void stopSiren ();

		virtual void initialize () override;
		virtual void finalize () override;

		virtual void processEvent (const QGAMES::Event& evnt) override;

		private:
		PacMan* _pacman;
		Inky* _inky;
		Blinky* _blinky;
		Pinky* _pinky;
		Clyde* _clyde;

		// Implementation
		/** The percetnage of the maze cleaned. */
		QGAMES::bdata _percentageCleaned;
		enum class SirenRate { _NORMAL = 0, _FAST = 1, _VERYFAST = 2 } _sirenRate;
	};

	/** Basic map of the game. 
		Layers can be only PacManII ones. */
	class Map : public QGAMES::TiledMap
	{
		public:
		Map (int c, const QGAMES::Layers& l, int w, int h, int d, int tW, int tH, int tD,
			const QGAMES::MapProperties& p = QGAMES::MapProperties ());

		/** To know important positions of the map. */
		virtual QGAMES::Position pacmanInitialPosition () const = 0;
		virtual QGAMES::Position monsterInitialPosition (int nP) const = 0;

		/** To blink. */
		void startBlinking (QGAMES::bdata bT, int nB);
		bool isBlinking () const;
		void stopBlinking ();

		/** To manage the ball status of the map. */
		int maxNumberBallsToEat () const;
		int numberBallsEaten () const;
		std::string ballsEatenStatus () const;
		void setBallsEatenStatus (const std::string& st);

		/** The events comming from the layers observed are transmitted up. */ 
		virtual void processEvent (const QGAMES::Event& evnt) override
							{ notify (QGAMES::Event (evnt.code (), this, evnt.values ())); }
	};

	/** The most common well known pacman maze. */
	class StandardMap final : public Map
	{
		public:
		StandardMap (int c, const QGAMES::Layers& l, int w, int h, int d, int tW, int tH, int tD,
			const QGAMES::MapProperties& p = QGAMES::MapProperties ())
			: Map (c, l, w, h, d, tW, tH, tD, p)
							{ }

		virtual QGAMES::Position pacmanInitialPosition () const override;
		virtual QGAMES::Position monsterInitialPosition (int nP) const override;
	};

	/** Representing a layer of the game.
		Tiles can be only PacManII ones. */
	class TileLayer final : public QGAMES::TileLayer
	{
		public:
		class BlinkingMaze final : public QGAMES::Layer::VisualEffect
		{
			public:
			/** if nB = -1 means never ends. */
			BlinkingMaze (QGAMES::Layer* ly, QGAMES::bdata bT, int nB)
				: QGAMES::Layer::VisualEffect (ly, QGAMES::LayerProperties ()),
				  _counterBlinkingTime (0, (int) (bT * __BD QGAMES::Game::game () -> framesPerSecond ()), 0, true, true),
				  _counterNumberBlinks (1, (nB == -1) ? __MAXINT__ : nB, 0, true, true),
				  _switchBlinkOn (0, false),
				  _active (false)
							{ assert (bT > __BD 0 && 
									  (nB > 0 || nB == -1)); }

			BlinkingMaze (const BlinkingMaze&) = delete;

			BlinkingMaze& operator = (const BlinkingMaze&) = delete;

			bool isActive () const
							{ return (_active); }

			virtual void initialize () override; 
			virtual void updatePositions () override; 
			virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& pos = QGAMES::Position::_cero) override
							{ }
			virtual void finalize () override;

			private:
			QGAMES::Counter _counterBlinkingTime;
			QGAMES::Counter _counterNumberBlinks;
			QGAMES::OnOffSwitch _switchBlinkOn;

			// Implementation 
			bool _active;
		};

		TileLayer (int c, const std::string& n, const QGAMES::Tiles& t, QGAMES::Map* m = NULL, 
				const QGAMES::LayerProperties& p = QGAMES::LayerProperties ());

		/** Set the blink on/off. */
		void startBlinking (QGAMES::bdata bT, int nB);
		bool isBlinking () const;
		void stopBlinking ();

		/** To manage the ball status of the layer. */
		int maxNumberBallsToEat () const;
		int numberBallsEaten () const;
		std::string ballsEatenStatus () const;
		void setBallsEatenStatus (const std::string& st);

		/** The events comming from the tiles observed are transmitted up. */ 
		virtual void processEvent (const QGAMES::Event& evnt) override
							{ notify (QGAMES::Event (evnt.code (), this, evnt.values ())); }
	};

	/** Representing the limit tile. */
	class TileLimit final : public QGAMES::Tile
	{
		public:
		TileLimit (int id, QGAMES::Form* f, int nf, const QGAMES::TileProperties& p = QGAMES::TileProperties ())
			: QGAMES::Tile (id, f, nf, __PACMANII_LIMITTILE__, p)
							{ }

		/** To indicate whether the tile has to bright or not. */
		void setBright (bool b);
	};

	/** Representing all types of possible path. */
	class TilePath : public QGAMES::Tile
	{
		public:
		typedef enum { _EMPTY = 0, _BALL = 1, _POWERBALL = 2, _RIGHTLIMIT = 3, _LEFTLIMIT = 4 } Type;

		TilePath (int id, QGAMES::Form* f, int nf, Type t, const QGAMES::TileProperties& p = QGAMES::TileProperties ())
			: QGAMES::Tile (id, f, nf, t, p),
			  _type (t),
			  _originalType (t)
							{ }

		/** To know and change th type. */
		Type type () const
							{ return (_type); }
		bool alreadyEaten () const
							{ return (canBeEaten () && _type == _EMPTY); } 
		bool canBeEaten () const
							{ return (_originalType == _BALL || _originalType == _POWERBALL); }
		bool eaten ();

		protected:
		Type _type;

		// Implementation
		Type _originalType;
	};
}

#endif
  
// End of the file
/*@}*/
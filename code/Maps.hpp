/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: Maps.hpp \n
 *	Game: PacManII \n
 *	Author: Ignacio Cea Forniés (Community Networks) \n
 *	Creation Date: 22/08/2021 \n
 *	Description: To manipulate the maps of the game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_MAPS__
#define __PACMANII_MAPS__

#include "Defs.hpp"
#include "Artists.hpp"
#include "Maze.hpp"
#include <advancedArcade/adinclude.hpp>

namespace PacManII
{
	class ArtistsLocationsLayer;
	class MazeLocationsLayer;
	class MazeZonesLayer;
	class DirectionsLayer;
	class MazeLayer;
	class BackgroundLayer;

	/** Basic map of the game. 
		Layers can be only PacManII ones. 
		The game can be extended in terms of maps. */
	class Map final : public QGAMES::TiledMap
	{
		public:
		Map (int c, const QGAMES::Layers& l, int w, int h, int d, int tW, int tH, int tD,
			 const QGAMES::MapProperties& p = QGAMES::MapProperties ());

		virtual QGAMES::SetOfOpenValues runtimeValues () const override;
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg) override;

		const ArtistsLocationsLayer* artistsLocationsLayer () const
							{ return (_artistsLocationsLayer); }
		ArtistsLocationsLayer* artistsLocationsLayer () 
							{ return (_artistsLocationsLayer); }
		const MazeLocationsLayer* mazeLocationsLayer () const
							{ return (_mazeLocationsLayer); }
		MazeLocationsLayer* mazeLocationsLayer () 
							{ return (_mazeLocationsLayer); }
		const MazeZonesLayer* mazeZonesLayer () const
							{ return (_mazeZonesLayer); }
		MazeZonesLayer* mazeZonesLayer () 
							{ return (_mazeZonesLayer); }
		const DirectionsLayer* directionsLayer () const
							{ return (_directionsLayer); }
		DirectionsLayer* directionsLayer () 
							{ return (_directionsLayer); }
		const MazeLayer* mazeLayer () const
							{ return (_mazeLayer); }
		MazeLayer* mazeLayer () 
							{ return (_mazeLayer); }
		const BackgroundLayer* backgroundLayer () const
							{ return (_backgroundLayer); }
		BackgroundLayer* backgroundLayer ()
							{ return (_backgroundLayer); }

		/** To know important positions of the map. 
			If nothing is found a position out of the maze should be returned. */
		const QGAMES::MazeModel::PositionInMaze& pacmanInitialPosition (int nP) const;
		const QGAMES::MazeModel::PositionInMaze& monsterInitialPosition (int nM) const;
		const QGAMES::MazeModel::PositionInMaze& monsterRunAwayPosition (int nM) const;
		const QGAMES::MazeModel::PositionInMaze& monsterExitingHomePosition () const;
		const QGAMES::MazeModel::PositionInMaze& monsterReturningPositionAfterDieing () const;
		const QGAMES::MazeModel::PositionInMaze& oneHomePosition () const;
		const QGAMES::MazeModel::PositionInMaze& fruitPosition () const;

		/** To convert any position in the maze in a position in the map. 
			What is returned is always the central position. */
		QGAMES::Position mazePositionToMapPosition (const QGAMES::MazeModel::PositionInMaze& p) const
							{ return ((p == QGAMES::MazeModel::_noPosition)
								? QGAMES::Position::_noPoint
								: QGAMES::Position ((__BD p._positionX + __BD 0.5) * tileWidth (), 
													(__BD p._positionY + __BD 0.5) * tileHeight (), __BD 0 /** 2d always. */)); }
		/** To get the position in the maze of a position in the screen. 
			Several positions can be matched to the same maze position. */
		QGAMES::MazeModel::PositionInMaze mapPositionToMazePosition (const QGAMES::Position& p) const
							{ return ((p == QGAMES::Position::_noPoint) 
								? QGAMES::MazeModel::_noPosition
								: QGAMES::MazeModel::PositionInMaze ((int) (p.posX () / __BD tileWidth ()), 
																	 (int) (p.posY () / __BD tileHeight ()), 0)); }

		/** To get the maze.
			The maze is set at construction time, and can't be modifid at all. */
		const Maze& maze () const
							{ return (_maze); }

		/** To blink. */
		void startBlinking (QGAMES::bdata bT, int nB);
		bool isBlinking () const;
		void stopBlinking ();

		/** To manage the ball status of the map. */
		int maxNumberBallsToEat () const;
		int numberBallsEaten () const;
		std::string ballsEatenStatus () const;
		void setBallsEatenStatus (const std::string& st);

		virtual void initialize () override;

		/** The events comming from the layers observed are transmitted up. */ 
		virtual void processEvent (const QGAMES::Event& evnt) override
							{ notify (QGAMES::Event (evnt.code (), evnt.data (), evnt.values ())); }

		protected:
		/** A representation of the maze, 
			to indicate the different directions possible per position in the maze (if any). */
		Maze _maze;

		// Implementation
		/** To accelerate the access to the different layers. */
		ArtistsLocationsLayer* _artistsLocationsLayer;
		MazeLocationsLayer* _mazeLocationsLayer;
		MazeZonesLayer* _mazeZonesLayer;
		DirectionsLayer* _directionsLayer;
		MazeLayer* _mazeLayer;
		BackgroundLayer* _backgroundLayer;

		/** To acclerate how positions are calculated (per number of artist). */
		// Artists positions
		mutable std::map <int, QGAMES::MazeModel::PositionInMaze> _pacmanInitialPositions;
		mutable std::map <int, QGAMES::MazeModel::PositionInMaze> _monsterInitialPositions;
		mutable std::map <int, QGAMES::MazeModel::PositionInMaze> _monsterRunAwayPositions;
		mutable QGAMES::MazeModel::PositionInMaze _monsterExitingHomePosition;
		mutable QGAMES::MazeModel::PositionInMaze _monsterReturningPositionAfterDieing;
		mutable QGAMES::MazeModel::PositionInMaze _fruitPosition;
		// Maze zones
		mutable std::map <int, std::vector <QGAMES::MazeModel::PositionInMaze>> _mazeZones;
	};

	/** Representing the background layer. */
	class BackgroundLayer final : public QGAMES::TileLayer
	{
		public:
		BackgroundLayer (int c, const std::string& n, const QGAMES::Tiles& t, QGAMES::Map* m = NULL, 
				const QGAMES::LayerProperties& p = QGAMES::LayerProperties ())
			: QGAMES::TileLayer (c, n, t, m, QGAMES::TileLayer::_ORTHOGONAL, p, false)
							{ }
	};

	/** Representing the "ArtistsLocations" layer. */
	class ArtistsLocationsLayer final : public QGAMES::TileLayer
	{
		public:
		ArtistsLocationsLayer (int c, const std::string& n, const QGAMES::Tiles& t, QGAMES::Map* m = NULL, 
				const QGAMES::LayerProperties& p = QGAMES::LayerProperties ())
			: QGAMES::TileLayer (c, n, t, m, QGAMES::TileLayer::_ORTHOGONAL, p, false),
			  _pacmanInitialPositions (), 
			  _monsterInitialPositions (), 
			  _monsterRunAwayPositions (),
			  _fruitPosition (QGAMES::Position::_noPoint)
							{ setVisible (false); /* Always */ }

		/** To know important positions kept in this layer. 
			The position (screen) of the tile is returned. */
		const std::map <int, QGAMES::Position>& pacmanInitialPositions () const; 
		const QGAMES::Position& pacmanInitialPosition (int nP) const;
		const std::map <int, QGAMES::Position>& monsterInitialPositions () const;
		const QGAMES::Position& monsterInitialPosition (int nM) const;
		const std::map <int, QGAMES::Position>& monsterRunAwayPositions () const;
		const QGAMES::Position& monsterRunAwayPosition (int nM) const;
		const QGAMES::Position& fruitPosition () const;

		protected:
		/** To accelerate how positions are accesed (per number of artist). */
		mutable std::map <int, QGAMES::Position> _pacmanInitialPositions;
		mutable std::map <int, QGAMES::Position> _monsterInitialPositions;
		mutable std::map <int, QGAMES::Position> _monsterRunAwayPositions;
		mutable QGAMES::Position _fruitPosition;
	};

	/** Representing the "MazeLocations" layer. */
	class MazeLocationsLayer final : public QGAMES::TileLayer
	{
		public:
		MazeLocationsLayer (int c, const std::string& n, const QGAMES::Tiles& t, QGAMES::Map* m = NULL, 
				const QGAMES::LayerProperties& p = QGAMES::LayerProperties ())
			: QGAMES::TileLayer (c, n, t, m, QGAMES::TileLayer::_ORTHOGONAL, p, false),
			  _monsterExitingHomePosition (QGAMES::Position::_noPoint),
			  _monsterReturningPositionAfterDieing (QGAMES::Position::_noPoint)
							{ setVisible (false); /* Always */ }

		/** To know important positions kept in this layer. 
			The position of the tile is returned. */
		const QGAMES::Position& monsterExitingHomePosition () const;
		const QGAMES::Position& monsterReturningPositionAfterDieing () const;

		protected:
		/** To accelerate how positions are calculated (per number of artist). */
		mutable QGAMES::Position _monsterExitingHomePosition;
		mutable QGAMES::Position _monsterReturningPositionAfterDieing;
	};

	/** Representing the "MazeZones" layer. */
	class MazeZonesLayer final : public QGAMES::TileLayer
	{
		public:
		MazeZonesLayer (int c, const std::string& n, const QGAMES::Tiles& t, QGAMES::Map* m = NULL, 
				const QGAMES::LayerProperties& p = QGAMES::LayerProperties ())
			: QGAMES::TileLayer (c, n, t, m, QGAMES::TileLayer::_ORTHOGONAL, p, false)
							{ setVisible (false); /* Always */ }
	};

	/** Representing the "Maze" (where directions are presented) layer. */
	class DirectionsLayer final : public QGAMES::TileLayer
	{
		public:
		DirectionsLayer (int c, const std::string& n, const QGAMES::Tiles& t, QGAMES::Map* m = NULL, 
				const QGAMES::LayerProperties& p = QGAMES::LayerProperties ())
			: QGAMES::TileLayer (c, n, t, m, QGAMES::TileLayer::_ORTHOGONAL, p, false)
							{ setVisible (false); /* Always. */ }
	};

	/** Representing the pure maze. the only one visible appart of background!. 
		The effect of blinking is executed here. \n
		Th information needed to execute it is defined in the builder (TMX). */
	class MazeLayer final : public QGAMES::TileLayer
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

		MazeLayer (int c, const std::string& n, const QGAMES::Tiles& t, QGAMES::Map* m = NULL, 
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
							{ notify (QGAMES::Event (evnt.code (), evnt.data (), evnt.values ())); }
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

	/** Representing all types elements in the path. */
	class TilePath final : public QGAMES::Tile
	{
		public:
		typedef enum { _EMPTY = 0, _BALL = 1, _POWERBALL = 2, _RIGHTLIMIT = 3, _LEFTLIMIT = 4 } Type;

		TilePath (int id, QGAMES::Form* f, int nf, Type t, const QGAMES::TileProperties& p = QGAMES::TileProperties ())
			: QGAMES::Tile (id, f, nf, t, p),
			  _type (t),
			  _originalType (t),
			  _originalFrame (nf)
							{ }

		/** To know and change the type. */
		Type type () const
							{ return (_type); }
		bool alreadyEaten () const
							{ return (canBeEaten () && _type == _EMPTY); } 
		bool canBeEaten () const
							{ return (_originalType == _BALL || _originalType == _POWERBALL); }
		bool hasPower () const
							{ return (_originalType == _POWERBALL); }
		bool eaten (bool s, bool n = true);

		protected:
		Type _type;

		// Implementation
		Type _originalType;
		int _originalFrame;
	};
}

#endif
  
// End of the file
/*@}*/
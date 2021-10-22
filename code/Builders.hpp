/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: Builders.hpp \n
 *	Game: PacManII \n
 *	Author: Ignacio Cea Forniés (Community Networks) \n
 *	Creation Date: 22/08/2021 \n
 *	Description: The constructors that the PacManII game needs. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_BUILDERS__
#define __PACMANII_BUILDERS__

#include "Defs.hpp"
#include <advancedArcade/adinclude.hpp>

namespace PacManII
{
	/** To create the letters of this game. */
	class TextBuilder final : public QGAMES::AdvancedTextBuilder
	{
		public:
		TextBuilder ();

		virtual QGAMES::ScoreObjectText* createText (int fnt, const std::string& txt) override;
	};

	/** To build the GUI systems and widgets of this game. */
	class GUISystemBuilder : public QGAMES::StandardGUISystemBuilder
	{
		public:
		GUISystemBuilder (const std::string& fDef, QGAMES::FormBuilder* fB)
			:  QGAMES::StandardGUISystemBuilder (fDef, fB)
							{ }

		protected:
		virtual QGAMES::GUISystem* createSystem (const QGAMES::GUISystemBuilder::GUISystemDefinition& def, 
			QGAMES::CompositeWidget* mW) override;
		virtual QGAMES::Widget* createWidget (const QGAMES::GUISystemBuilder::WidgetDefinition& def) override;
	};

	/** To create the game state to initialize the game. */
	class ForInitializePacManIIGameStateAddOn final : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForInitializePacManIIGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
						{ }

		virtual bool canCreateGameState (int t) const override
						{ return (t == __PACMANII_GAMESTATEINITIALIZEGAMETYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def) override;
	};

	/** To create a game state add on that shows a set of monsters moving in the right side of the screen moving randomly. */
	class ForShowingMonsterMovingGameStateAddOn final : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForShowingMonsterMovingGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
						{ }

		virtual bool canCreateGameState (int t) const override
						{ return (t == __PACMANII_GAMESTATEMOVINGMONSTERSTYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def) override;
	};

	/** To create a game state add on to show the name of the player. */
	class ForShowingPlayerNameGameStateAddOn final : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForShowingPlayerNameGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
						{ }

		virtual bool canCreateGameState (int t) const override
						{ return (t == __PACMANII_GAMESTATEPLAYERNAMETYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def) override;
	};

	/** To create a game state add on to show the name of the current lvel. */
	class ForShowingLevelGameStateAddOn final : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForShowingLevelGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
						{ }

		/** @see parent. */
		virtual bool canCreateGameState (int t) const override
						{ return (t == __PACMANII_GAMESTATELEVELTYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def) override;
	};

	/** To create the control playing game state of this type of games. */
	class ForPlayingControlGameStateAddOn final : public QGAMES::GameStateBuilder::ForControlAddsOn
	{
		public:
		ForPlayingControlGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::ForControlAddsOn (b)
							{ }

		virtual bool canCreateGameState (int t) const override
						{ return (t == __PACMANII_GAMESTATECONTROLPLAYINGTYPE__); }

		protected:
		virtual QGAMES::GameState* createControlGameState 
			(const QGAMES::GameStateBuilder::ControlGameStateDefinition* def,
			 const QGAMES::GameStates& sts, const QGAMES::GameStateControl::FlowMap& fl) override;
	};

	/** To create the game state to play. */
	class ForPlayingGameStateAddOn final : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForPlayingGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
							{ }

		virtual bool canCreateGameState (int t) const override
						{ return (t == __PACMANII_GAMESTATEPLAYINGTYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def) override;
	};

	/** To create the game state for the map to blink. */
	class ForBlinkMazePlayingGameStateAddOn final : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForBlinkMazePlayingGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
							{ }

		virtual bool canCreateGameState (int t) const override
						{ return (t == __PACMANII_GAMESTATEBLINKMAZEMAPTYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def) override;
	};

	/** To create the game state with the options when playing. */
	class ForOptionsWhenPlayingGameStateAddOn final : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForOptionsWhenPlayingGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
							{ }

		virtual bool canCreateGameState (int t) const override
						{ return (t == __PACMANII_GAESTATEOPTIONSWHENPLAYTYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def) override;
	};

	/** To create the game state talking about the hall of fame in this type of games. 
		The parameters are the same than the ones used for the basic hall of fame game state defined in AdvacedArcade
		but what changes is the ways the information is understood. */
	class ForShowingHallOfFameGameStateAddOn final : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForShowingHallOfFameGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
							{ }

		virtual bool canCreateGameState (int t) const override
						{ return (t == __PACMANII_GAMESTATEHALLOFFAMETYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def) override;
	};

	/** To create the special object to control the flow of the game. */
	class ForGameStateControlAddon final : public QGAMES::ForAdvancedGameStateControlAddon
	{
		public:
		ForGameStateControlAddon (QGAMES::GameStateBuilder* b)
			: QGAMES::ForAdvancedGameStateControlAddon (b)
							{ }

		virtual bool canCreateGameState (int t) const override
						{ return (QGAMES::ForAdvancedGameStateControlAddon::canCreateGameState (t) ||
								  t == __PACMANII_GAMESTATECTRLTYPE__); }

		protected:
		virtual QGAMES::GameState* createControlGameState 
			(const QGAMES::GameStateBuilder::ControlGameStateDefinition* def,
			 const QGAMES::GameStates& sts, const QGAMES::GameStateControl::FlowMap& fl) override;
	};

	/** To create the movements of the game. */
	class MovementBuilder final : public QGAMES::AdvancedMovementBuilder
	{
		public:
		MovementBuilder () = delete;

		MovementBuilder (const std::string& fDef)
			: QGAMES::AdvancedMovementBuilder (fDef) 
								{ }

		MovementBuilder (const MovementBuilder&) = delete;

		MovementBuilder& operator = (const MovementBuilder&) = delete;

		protected:
		virtual QGAMES::Movement* createMovement 
			(const QGAMES::MovementBuilder::MovementDefinition& def) override;
	};

	/** To create specific entities of the game. */
	class EntityBuilder final : public QGAMES::AdvancedEntityBuilder
	{
		public:
		EntityBuilder () = delete;

		EntityBuilder (const std::string& eDef, QGAMES::FormBuilder* fB, QGAMES::MovementBuilder* mB)
			: QGAMES::AdvancedEntityBuilder (eDef, fB, mB)
								{ }

		EntityBuilder (const EntityBuilder&) = delete;

		EntityBuilder& operator = (const EntityBuilder&) = delete;

		protected:
		virtual QGAMES::Entity* createEntity (const QGAMES::EntityBuilder::EntityDefinition& def) override;
	};

	/** The extension to create maps. */
	class TMXMapBuilder : public QGAMES::TMXMapBuilderAddsOn
	{
		public:
		TMXMapBuilder (QGAMES::Sprite2DBuilder* sB)
			: QGAMES::TMXMapBuilderAddsOn (sB)
							{ }

		protected:
		virtual QGAMES::Tile* createTile (int id, QGAMES::Form* form, int nf, 
			const QGAMES::TileProperties& p = QGAMES::TileProperties ()) override;
		virtual QGAMES::TileLayer* createTileLayer (int id, const std::string& n, 
			const QGAMES::Tiles& t, QGAMES::Map* m, QGAMES::TileLayer::VisualOrientation o, 
			const QGAMES::LayerProperties& p = QGAMES::LayerProperties ()) override;
		virtual QGAMES::ObjectLayer* createObjectLayer (int id, const std::string& n, const QGAMES::Objects&, 
			QGAMES::Map* m, const QGAMES::LayerProperties& oP = QGAMES::LayerProperties ()) override;
		virtual QGAMES::ImageLayer* createImageLayer (int id, const std::string& n, QGAMES::Image* i, 
			QGAMES::Map* m, const QGAMES::LayerProperties& oP = QGAMES::LayerProperties ()) override;
		virtual QGAMES::Map* createMapObject (int id, const QGAMES::Layers& l, int w, int h, int d, int tW, int tH, int tD,
			const QGAMES::MapProperties& p = QGAMES::MapProperties ()) override;
	};

	/** The extension to create worlds. */
	class WorldBuilder final : public QGAMES::WorldBuilder
	{
		public: 
		WorldBuilder () = delete;
			
		WorldBuilder (const std::string& eDef, QGAMES::MapBuilder* mB)
			: QGAMES::WorldBuilder (eDef, mB)
							{ }

		WorldBuilder (const WorldBuilder&) = delete;

		WorldBuilder& operator = (const WorldBuilder&) = delete;

		private:
		virtual QGAMES::World* createWorldObject (int no, const QGAMES::Scenes& s, 
			const QGAMES::WorldProperties& p) override final;
		virtual QGAMES::Scene* createSceneObject (int ns, const QGAMES::Maps& m, 
			const QGAMES::Scene::Connections& cn, 
			const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL) override;
		virtual QGAMES::SceneActionBlock* createSceneActionBlockObject (int nAB, 
			const QGAMES::SceneActionBlockProperties& prps) override;
	};
}

#endif
  
// End of the file
/*@}*/
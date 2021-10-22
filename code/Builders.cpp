#include "Builders.hpp"
#include "Letters.hpp"
#include "GUISystem.hpp"
#include "Artists.hpp"
#include "GameStates.hpp"
#include "Worlds.hpp"

// ---
PacManII::TextBuilder::TextBuilder ()
	: QGAMES::AdvancedTextBuilder ()
{
	QGAMES::AdvancedTextBuilder::FontDataPerSize fNT;

	// NewTour 24...
	QGAMES::AdvancedTextBuilder::FontDataList fNT24;
	fNT24.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__PACMANII_NEWTOUR24BLUELETTERS__, __PACMANII_NEWTOURLETTERS__, 24, __QGAMES_BLUECOLOR__));
	fNT24.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__PACMANII_NEWTOUR24GREENLETTERS__, __PACMANII_NEWTOURLETTERS__, 24, __QGAMES_GREENCOLOR__));
	fNT24.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__PACMANII_NEWTOUR24REDLETTERS__, __PACMANII_NEWTOURLETTERS__, 24, __QGAMES_REDCOLOR__));
	fNT24.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__PACMANII_NEWTOUR24YELLOWLETTERS__, __PACMANII_NEWTOURLETTERS__, 24, __QGAMES_YELLOWCOLOR__));
	fNT [24] = fNT24;

	// NewTour 36...
	QGAMES::AdvancedTextBuilder::FontDataList fNT36;
	fNT36.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__PACMANII_NEWTOUR36BLUELETTERS__, __PACMANII_NEWTOURLETTERS__, 36, __QGAMES_BLUECOLOR__));
	fNT36.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__PACMANII_NEWTOUR36GREENLETTERS__, __PACMANII_NEWTOURLETTERS__, 36, __QGAMES_GREENCOLOR__));
	fNT36.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__PACMANII_NEWTOUR36REDLETTERS__, __PACMANII_NEWTOURLETTERS__, 36, __QGAMES_REDCOLOR__));
	fNT36.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__PACMANII_NEWTOUR36YELLOWLETTERS__, __PACMANII_NEWTOURLETTERS__, 36, __QGAMES_YELLOWCOLOR__));
	fNT [36] = fNT36;

	// NewTour 48...
	QGAMES::AdvancedTextBuilder::FontDataList fNT48;
	fNT48.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__PACMANII_NEWTOUR48BLUELETTERS__, __PACMANII_NEWTOURLETTERS__, 48, __QGAMES_BLUECOLOR__));
	fNT48.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__PACMANII_NEWTOUR48GREENLETTERS__, __PACMANII_NEWTOURLETTERS__, 48, __QGAMES_GREENCOLOR__));
	fNT48.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__PACMANII_NEWTOUR48REDLETTERS__, __PACMANII_NEWTOURLETTERS__, 48, __QGAMES_REDCOLOR__));
	fNT48.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__PACMANII_NEWTOUR48YELLOWLETTERS__, __PACMANII_NEWTOURLETTERS__, 48, __QGAMES_YELLOWCOLOR__));
	fNT [48] = fNT48;

	_fontData [__PACMANII_NEWTOURLETTERS__] = fNT;
}

// ---
QGAMES::ScoreObjectText* PacManII::TextBuilder::createText (int fnt, const std::string& txt)
{
	QGAMES::ScoreObjectText* result = nullptr;
	
	switch (fnt)
	{
		case __PACMANII_NEWTOUR24BLUELETTERS__:
			result = new PacManII::NewTour24BlueFont (txt);
			break;

		case __PACMANII_NEWTOUR24GREENLETTERS__:
			result = new PacManII::NewTour24GreenFont (txt);
			break;

		case __PACMANII_NEWTOUR24REDLETTERS__:
			result = new PacManII::NewTour24RedFont (txt);
			break;

		case __PACMANII_NEWTOUR24YELLOWLETTERS__:
			result = new PacManII::NewTour24YellowFont (txt);
			break;

		case __PACMANII_NEWTOUR36BLUELETTERS__:
			result = new PacManII::NewTour36BlueFont (txt);
			break;

		case __PACMANII_NEWTOUR36GREENLETTERS__:
			result = new PacManII::NewTour36GreenFont (txt);
			break;

		case __PACMANII_NEWTOUR36REDLETTERS__:
			result = new PacManII::NewTour36RedFont (txt);
			break;

		case __PACMANII_NEWTOUR36YELLOWLETTERS__:
			result = new PacManII::NewTour36YellowFont (txt);
			break;

		case __PACMANII_NEWTOUR48BLUELETTERS__:
			result = new PacManII::NewTour48BlueFont (txt);
			break;

		case __PACMANII_NEWTOUR48GREENLETTERS__:
			result = new PacManII::NewTour48GreenFont (txt);
			break;

		case __PACMANII_NEWTOUR48REDLETTERS__:
			result = new PacManII::NewTour48RedFont (txt);
			break;

		case __PACMANII_NEWTOUR48YELLOWLETTERS__:
			result = new PacManII::NewTour48YellowFont (txt);
			break;

		default:
			result = QGAMES::AdvancedTextBuilder::createText (fnt, txt);
	}

	return (result);
}

// ---
QGAMES::GUISystem* PacManII::GUISystemBuilder::createSystem 
	(const QGAMES::GUISystemBuilder::GUISystemDefinition& def, QGAMES::CompositeWidget* mW)
{
	if (def._id == __PACMANII_OPTIONWHENPLAYINGGUISYSTEM__)
		return (new PacManII::GUISystem (mW));
	else
		return (QGAMES::StandardGUISystemBuilder::createSystem (def, mW));
}

// ---
QGAMES::Widget* PacManII::GUISystemBuilder::createWidget (const QGAMES::GUISystemBuilder::WidgetDefinition& def)
{
	QGAMES::Widget* result = nullptr;

	if (def._id == __QGAMES_STDGUISYSTEMMAINWIDGET__ ||
		def._id == __QGAMES_STDGUISYSTEMMAINCONFWIDGET__ ||
		def._id == __PACMANII_OPTIONSGUISYSTEMMAINWIDGET__) 
		result = new PacManII::ControlWidget (def._id);
	else
		result = QGAMES::StandardGUISystemBuilder::createWidget (def);

	return (result);
}

// ---
QGAMES::GameState* PacManII::ForInitializePacManIIGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new PacManII::InitializePacManIIGameState (def -> _id, 
		(def -> _nested != nullptr) ? builder () -> gameState (def -> _nested -> _id) : nullptr));
}

// ---
QGAMES::GameState* PacManII::ForShowingMonsterMovingGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new PacManII::MonsterMovingGameState (def -> _id, 
		PacManII::MonsterMovingGameState::Properties (def -> _attributes),
		(def -> _nested != nullptr) ? builder () -> gameState (def -> _nested -> _id) : nullptr));
}

// ---
QGAMES::GameState* PacManII::ForShowingPlayerNameGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new PacManII::ShowingCurrentPlayerNameGameState (
		def -> _id,
		def -> _attributes.find (__QGAMES_GAMESTATESHOWSTDTXTATTRTEXT__) -> second,
		std::atoi (def -> _attributes.find (__QGAMES_GAMESTATESHOWSTDTXTATTRFONT__) -> second.c_str ()),
		QGAMES::ShowingTextGameState::Properties (def -> _attributes), // Other attributes...
		(def -> _nested != nullptr) ? builder () -> gameState (def -> _nested -> _id) : nullptr));
}

// ---
QGAMES::GameState* PacManII::ForShowingLevelGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new PacManII::ShowingCurrentLevelGameState (
		def -> _id,
		def -> _attributes.find (__QGAMES_GAMESTATESHOWSTDTXTATTRTEXT__) -> second,
		std::atoi (def -> _attributes.find (__QGAMES_GAMESTATESHOWSTDTXTATTRFONT__) -> second.c_str ()),
		QGAMES::ShowingTextGameState::Properties (def -> _attributes), // Other attributes...
		(def -> _nested != nullptr) ? builder () -> gameState (def -> _nested -> _id) : nullptr));
}

// ---
QGAMES::GameState* PacManII::ForPlayingControlGameStateAddOn::createControlGameState 
	(const QGAMES::GameStateBuilder::ControlGameStateDefinition* def,
	 const QGAMES::GameStates& sts, const QGAMES::GameStateControl::FlowMap& fl)
{
	return (new PacManII::PlayingControlGameState (def -> _id, sts, fl));
}

// ---
QGAMES::GameState* PacManII::ForPlayingGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new PacManII::PlayingGameState (def -> _id,
		(def -> _nested != nullptr) ? builder () -> gameState (def -> _nested -> _id) : nullptr));
}

// ---
QGAMES::GameState* PacManII::ForBlinkMazePlayingGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new PacManII::BlinkMazePlayingGameState (
		def -> _id, PacManII::BlinkMazePlayingGameState::Properties (def -> _attributes),
		(def -> _nested != nullptr) ? builder () -> gameState (def -> _nested -> _id) : nullptr));
}

// ---
QGAMES::GameState* PacManII::ForOptionsWhenPlayingGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new PacManII::OptionsWhenPlayingGameState (def -> _id,
		(def -> _nested != nullptr) ? builder () -> gameState (def -> _nested -> _id) : nullptr));
}

// ---
QGAMES::GameState* PacManII::ForShowingHallOfFameGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new PacManII::ShowingHallOfFameGameState (
		def -> _id, QGAMES::ShowingHallOfFameGameState::Properties (def -> _attributes),
		(def -> _nested != nullptr) ? builder () -> gameState (def -> _nested -> _id) : nullptr));
}

// ---
QGAMES::GameState* PacManII::ForGameStateControlAddon::createControlGameState
			(const QGAMES::GameStateBuilder::ControlGameStateDefinition* def,
			 const QGAMES::GameStates& sts, const QGAMES::GameStateControl::FlowMap& fl)
{
	return ((def -> _type == __PACMANII_GAMESTATECTRLTYPE__)
		? new PacManII::GameStateControl (def -> _id, 
			builder () -> gameState (std::atoi (def -> _attributes.find (__QGAMES_GAMESTATEADVANCEDCTRLPAUSE__) -> second.c_str ())), sts, fl)
		: QGAMES::ForAdvancedGameStateControlAddon::createControlGameState (def, sts, fl));
}

// ---
QGAMES::Movement* PacManII::MovementBuilder::createMovement 
	(const QGAMES::MovementBuilder::MovementDefinition& def)
{
	QGAMES::Movement* result = nullptr;

	// TODO

	// Pacman Movement's
	if (def._id >= __PACMANII_PACMANSTAYMOVEMENTBASEID__ &&
		def._id < (__PACMANII_PACMANSTAYMOVEMENTBASEID__ + __PACMANII_NUMBERENTITIES__))
		result = new QGAMES::NoMovement (def._id, def._variables);
	else 
	if (def._id >= __PACMANII_PACMANEATINGMOVEMENTBASEID__ &&
		def._id < (__PACMANII_PACMANEATINGMOVEMENTBASEID__ + __PACMANII_NUMBERENTITIES__))
		result = new QGAMES::NoMovement (def._id, def._variables);
	else
	// Monsters' chasing movements
	if ((def._id >= __PACMANII_INKYCHASINGMOVEMENTBASEID__ &&
		 def._id < (__PACMANII_INKYCHASINGMOVEMENTBASEID__ + __PACMANII_NUMBERENTITIES__)) ||
		(def._id >= __PACMANII_BLINKYCHASINGMOVEMENTBASEID__ &&
		 def._id < (__PACMANII_BLINKYCHASINGMOVEMENTBASEID__ + __PACMANII_NUMBERENTITIES__)) ||		
		(def._id >= __PACMANII_PINKYCHASINGMOVEMENTBASEID__ &&
		 def._id < (__PACMANII_PINKYCHASINGMOVEMENTBASEID__ + __PACMANII_NUMBERENTITIES__)) ||		
		(def._id >= __PACMANII_CLYDECHASINGMOVEMENTBASEID__ &&
		 def._id < (__PACMANII_CLYDECHASINGMOVEMENTBASEID__ + __PACMANII_NUMBERENTITIES__)))		
		result = new QGAMES::NoMovement (def._id, def._variables);
	else
	// Monsters' running away movements
	if ((def._id >= __PACMANII_INKYRUNNINGAWAYMOVEMENTBASEID__ &&
		 def._id < (__PACMANII_INKYRUNNINGAWAYMOVEMENTBASEID__ + __PACMANII_NUMBERENTITIES__)) ||
		(def._id >= __PACMANII_BLINKYRUNNINGAWAYMOVEMENTBASEID__ &&
		 def._id < (__PACMANII_BLINKYRUNNINGAWAYMOVEMENTBASEID__ + __PACMANII_NUMBERENTITIES__)) ||		
		(def._id >= __PACMANII_PINKYRUNNINGAWAYMOVEMENTBASEID__ &&
		 def._id < (__PACMANII_PINKYRUNNINGAWAYMOVEMENTBASEID__ + __PACMANII_NUMBERENTITIES__)) ||		
		(def._id >= __PACMANII_CLYDERUNNINGAWAYMOVEMENTBASEID__ &&
		 def._id < (__PACMANII_CLYDERUNNINGAWAYMOVEMENTBASEID__ + __PACMANII_NUMBERENTITIES__)))		
		result = new QGAMES::NoMovement (def._id, def._variables);
	else
	// Monsters' going-back-movements movements to home after being eaten
	if ((def._id >= __PACMANII_INKYAFTEREATENMOVEMENTBASEID__ &&
		 def._id < (__PACMANII_INKYAFTEREATENMOVEMENTBASEID__ + __PACMANII_NUMBERENTITIES__)) ||
		(def._id >= __PACMANII_BLINKYAFTEREATENMOVEMENTBASEID__ &&
		 def._id < (__PACMANII_BLINKYAFTEREATENMOVEMENTBASEID__ + __PACMANII_NUMBERENTITIES__)) ||		
		(def._id >= __PACMANII_PINKYAFTEREATENMOVEMENTBASEID__ &&
		 def._id < (__PACMANII_PINKYAFTEREATENMOVEMENTBASEID__ + __PACMANII_NUMBERENTITIES__)) ||		
		(def._id >= __PACMANII_CLYDEAFTEREATENMOVEMENTBASEID__ &&
		 def._id < (__PACMANII_CLYDEAFTEREATENMOVEMENTBASEID__ + __PACMANII_NUMBERENTITIES__)))		
		result = new QGAMES::NoMovement (def._id, def._variables);
	else
	// Monsters' at-home movements
	if ((def._id >= __PACMANII_INKYATHOMEMOVEMENTBASEID__ &&
		 def._id < (__PACMANII_INKYATHOMEMOVEMENTBASEID__ + __PACMANII_NUMBERENTITIES__)) ||
		(def._id >= __PACMANII_BLINKYATHOMEMOVEMENTBASEID__ &&
		 def._id < (__PACMANII_BLINKYATHOMEMOVEMENTBASEID__ + __PACMANII_NUMBERENTITIES__)) ||		
		(def._id >= __PACMANII_PINKYATHOMEMOVEMENTBASEID__ &&
		 def._id < (__PACMANII_PINKYATHOMEMOVEMENTBASEID__ + __PACMANII_NUMBERENTITIES__)) ||		
		(def._id >= __PACMANII_CLYDEATHOMEMOVEMENTBASEID__ &&
		 def._id < (__PACMANII_CLYDEATHOMEMOVEMENTBASEID__ + __PACMANII_NUMBERENTITIES__)))		
		result = new QGAMES::NoMovement (def._id, def._variables);
	else
		result = QGAMES::AdvancedMovementBuilder::createMovement (def);

	return (result);
}

// ---
QGAMES::Entity* PacManII::EntityBuilder::createEntity (const QGAMES::EntityBuilder::EntityDefinition& def)
{
	QGAMES::Entity* result = nullptr;

	// There can be a couple of similar running entities...
	if (def._id >= __PACMANII_PACMANBASEENTITYID__ && 
		def._id < (__PACMANII_PACMANBASEENTITYID__ + __PACMANII_NUMBERENTITIES__)) 
			result = new PacManII::PacMan (def._id);
	else 
	if (def._id >= __PACMANII_INKYBASEENTITYID__ && 
		def._id < (__PACMANII_INKYBASEENTITYID__ + __PACMANII_NUMBERENTITIES__))
			result = new PacManII::Inky (def._id);
	else 
	if (def._id >= __PACMANII_BLINKYBASEENTITYID__ && 
		def._id < (__PACMANII_BLINKYBASEENTITYID__ + __PACMANII_NUMBERENTITIES__))
			result = new PacManII::Blinky (def._id);
	else 
	if (def._id >= __PACMANII_PINKYBASEENTITYID__ && 
		def._id < (__PACMANII_PINKYBASEENTITYID__ + __PACMANII_NUMBERENTITIES__))
			result = new PacManII::Pinky (def._id);
	else
	if (def._id >= __PACMANII_CLYDEBASEENTITYID__ && 
		def._id < (__PACMANII_CLYDEBASEENTITYID__ + __PACMANII_NUMBERENTITIES__)) 
			result = new PacManII::Clyde (def._id);
	else 
		result = QGAMES::AdvancedEntityBuilder::createEntity (def);

	return (result);
}

// ---
QGAMES::Tile* PacManII::TMXMapBuilder::createTile (int id, QGAMES::Form* form, int nf, const QGAMES::TileProperties& p)
{
	return (new PacManII::TileLimit (id, form, nf, p));
}

// ---
QGAMES::TileLayer* PacManII::TMXMapBuilder::createTileLayer (int id, const std::string& n, 
	const QGAMES::Tiles& t, QGAMES::Map* m, QGAMES::TileLayer::VisualOrientation o, const QGAMES::LayerProperties& p)
{
	return (new PacManII::TileLayer (id, n, t, m, p));
}

// ---
QGAMES::ObjectLayer* PacManII::TMXMapBuilder::createObjectLayer (int id, const std::string& n, const QGAMES::Objects&, 
	QGAMES::Map* m, const QGAMES::LayerProperties& oP)
{
	return (nullptr); // Not used in pacman II
}

// ---
QGAMES::ImageLayer* PacManII::TMXMapBuilder::createImageLayer (int id, const std::string& n, QGAMES::Image* i, 
	QGAMES::Map* m, const QGAMES::LayerProperties& oP)
{
	return (nullptr); // Not used in pacman II
}

// ---
QGAMES::Map* PacManII::TMXMapBuilder::createMapObject (int id, const QGAMES::Layers& l, 
	int w, int h, int d, int tW, int tH, int tD, const QGAMES::MapProperties& p)
{
	return (new PacManII::StandardMap (id, l, w, h, d, tW, tH, tD, p));
}

// ---
QGAMES::World* PacManII::WorldBuilder::createWorldObject (int no, const QGAMES::Scenes& s, const QGAMES::WorldProperties& p)
{
	return (new PacManII::World (no, s, p));
}

// ---
QGAMES::Scene* PacManII::WorldBuilder::createSceneObject (int ns, const QGAMES::Maps& m, 
	const QGAMES::Scene::Connections& cn, const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
{
	return (new PacManII::Scene (ns, m, cn, p, ePL));
}

// ---
QGAMES::SceneActionBlock* PacManII::WorldBuilder::createSceneActionBlockObject (int nAB, 
	const QGAMES::SceneActionBlockProperties& prps)
{
	return (nullptr);

	// TODO
}

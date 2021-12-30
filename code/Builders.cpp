#include "Builders.hpp"
#include "Letters.hpp"
#include "GUISystem.hpp"
#include "Artists.hpp"
#include "Movements.hpp"
#include "GameStates.hpp"
#include "Worlds.hpp"
#include "Scenes.hpp"
#include "Maps.hpp"
#include "SceneActionBlocks.hpp"

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
		result = new PacManII::MazeMovement (def._id, def._variables);
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
		result = new PacManII::MazeMovement (def._id, def._variables);
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
		result = new PacManII::MazeMovement (def._id, def._variables);
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
		result = new PacManII::MazeMovement (def._id, def._variables);
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
	// Fruit's staying movement
	if (def._id >= __PACMANII_FRUITSTAYINGMOVEMENTBASEID__ &&
		 def._id < (__PACMANII_FRUITSTAYINGMOVEMENTBASEID__ + __PACMANII_NUMBERENTITIES__))
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
	if (def._id >= __PACMANII_FRUITBASEENTITYID__ && 
		def._id < (__PACMANII_FRUITBASEENTITYID__ + __PACMANII_NUMBERENTITIES__)) 
			result = new PacManII::Fruit (def._id);
	else
		result = QGAMES::AdvancedEntityBuilder::createEntity (def);

	return (result);
}

// ---
PacManII::TMXMapBuilder::TMXMapBuilder (QGAMES::Sprite2DBuilder* sB)
	: QGAMES::TMXMapBuilderAddsOn (sB),
	  _TILESLIMITDARK				({ 0,1,2,3,4,5,6,7,8,9,10,12,13,14,15,16,17,20,21,22,23,24,25,26 }),
	  _TILESLIMITBRIGHT				({ 50,51,52,53,54,55,56,57,58,59,60,62,63,64,65,66,67,70,71,72,73,74,75,76 }),
	  _TILESPOWERBALL				({ 27,28,57,58 }),
	  _TILESNORMALBALL				({ 29,59 }),
	  _TILESPATH					({ 11,61,18,19,68,69 }),
	  _TILESPATHLIMITRIGHT			({ 30,31,32,33,34,35,36,37,38,39 }),
	  _TILESPATHLIMITLEFT			({ 40,41,42,43,44,45,46,47,48,49}),
	  _TILESHOMEPACMAN				({ 20,21,22,23,24,25,26,27,28,29 }),
	  _TILESHOMEMONSTERS			({ 10,11,12,13,14,15,16,17,18,19 }),
	  _TILESRUNAWAYMONSTERS			({ 0,1,2,3,4,5,6,7,8,9 }),
	  _TILEEXITINGHOMEMONSTERS		(50),
	  _TILEDEFININGTUNNELPATH		(51),
	  _TILEDEFININGFRUITPOSITION	(52),
	  _TILESDIRECTIONMAZE			({	{  0, { false,	true,	false,	true	} }, // LEFT, RIGHT, UP, DOWN
										{  1, { true,	true,	false,	true	} },
										{  2, { true,	false,	false,	true	} },
										{  3, { false,	false,	true,	false	} },
										{  4, { false,	true,	false,	false	} },
										{  5, { true,	true,	false,	false	} },
										{  6, { true,	false,	false,	false	} },
										{  7, { false,	true,	true,	true	} },
										{  8, { true,	true,	true,	true	} },
										{  9, { true,	false,	true,	true	} },
										{ 10, { false,	false,	true,	true	} },
										{ 11, { false,	true,	true,	false	} },
										{ 12, { true,	true,	true,	false	} },
										{ 13, { true,	false,	true,	false	} },
										{ 14, { false,	false,	true,	false	} },
										{ 15, { false,	false,	false,	false	} }}),
	  _TILESCONNECTION				({ 30,31,32,33,34,35,36,37,38,39 })
{
	assert (_TILESLIMITBRIGHT.size () == _TILESLIMITDARK.size ());
	// The have to have the same size to change between them...
}

// ---
int PacManII::TMXMapBuilder::brightFrameFor (int nF) const
{
	int result = nF;

	if (isInType (nF, _TILESLIMITDARK)) // If not, it can't change...
		result = _TILESLIMITBRIGHT [positionInType (nF, _TILESLIMITDARK)];

	return (result);
}

// ---
int PacManII::TMXMapBuilder::darkFrameFor (int nF) const
{
	int result = nF;

	if (isInType (nF, _TILESLIMITBRIGHT)) // If not, it can't chang...
		result = _TILESLIMITDARK [positionInType (nF, _TILESLIMITBRIGHT)];

	return (result);
}

// ---
QGAMES::Tile* PacManII::TMXMapBuilder::createTile (int id, QGAMES::Form* form, int nf, const QGAMES::TileProperties& p)
{
	QGAMES::Tile* result = nullptr;

	if (form -> name () == std::string (__PACMANII_FORMTILESMAZENAME__))
	{
		if (isInType (nf, _TILESLIMITDARK) || isInType (nf, _TILESLIMITBRIGHT)) result = new PacManII::TileLimit (id, form, nf, p);
		else if (isInType (nf, _TILESPOWERBALL)) result = new PacManII::TilePath (id, form, nf, PacManII::TilePath::Type::_POWERBALL,	p);
		else if (isInType (nf, _TILESNORMALBALL)) result = new PacManII::TilePath (id, form, nf,  PacManII::TilePath::Type::_BALL, p);
		else if (isInType (nf, _TILESPATH)) result = new PacManII::TilePath (id, form, nf,  PacManII::TilePath::Type::_EMPTY, p);
		else if (isInType (nf, _TILESPATHLIMITRIGHT)) result = new PacManII::TilePath (id, form, nf,  PacManII::TilePath::Type::_RIGHTLIMIT, p);
		else if (isInType (nf, _TILESPATHLIMITLEFT)) result = new PacManII::TilePath (id, form, nf,  PacManII::TilePath::Type::_LEFTLIMIT, p);
		else result = new QGAMES::NullTile (id); /// Just in case, but a well defined maze whould reach this point ever...
	}
	else
		result = new QGAMES::Tile (id, form, nf, 0, p); // When it is not a corner, all of them will be equal...

	return (result);
}

// ---
QGAMES::TileLayer* PacManII::TMXMapBuilder::createTileLayer (int id, const std::string& n, 
	const QGAMES::Tiles& t, QGAMES::Map* m, QGAMES::TileLayer::VisualOrientation o, const QGAMES::LayerProperties& p)
{
	QGAMES::TileLayer* result = nullptr;

	if (n == std::string (__PACMANII_BACKGROUNDLAYERNAME__))
		result = new PacManII::BackgroundLayer (id, n, t, m, p);
	else if (n == std::string (__PACMANII_LOCATIONSLAYERNAME__))
		result = new PacManII::LocationsLayer (id, n, t, m, p);
	else if (n == std::string (__PACMANII_DIRECTIONSLAYERNAME__))
		result = new PacManII::DirectionsLayer (id, n, t, m, p);
	else if (n == std::string (__PACMANII_MAZELAYERNAME__))
		result = new PacManII::MazeLayer (id, n, t, m, p);

	// A layer not well defined becomes null...

	return (result);
}

// ---
QGAMES::ObjectLayer* PacManII::TMXMapBuilder::createObjectLayer (int id, const std::string& n, const QGAMES::Objects&, 
	QGAMES::Map* m, const QGAMES::LayerProperties& oP)
{
	return (nullptr); // Not used in Pacman II
}

// ---
QGAMES::ImageLayer* PacManII::TMXMapBuilder::createImageLayer (int id, const std::string& n, QGAMES::Image* i, 
	QGAMES::Map* m, const QGAMES::LayerProperties& oP)
{
	return (nullptr); // Not used in Pacman II
}

// ---
QGAMES::Map* PacManII::TMXMapBuilder::createMapObject (int id, const QGAMES::Layers& l, 
	int w, int h, int d, int tW, int tH, int tD, const QGAMES::MapProperties& p)
{
	// By default all are the same...
	return (new PacManII::Map (id, l, w, h, d, tW, tH, tD, p));
}

// ---
int PacManII::TMXMapBuilder::positionInType (int nF, const std::vector <int>& t) const
{
	assert (isInType (nF, t));

	bool found = false;
	int result = 0;
	for (std::vector <int>::const_iterator i = t.begin (); i != t.end () && !found; i++)
		if ((*i) != nF) result++; 
		else found = true;

	assert (found); // Not possible other result...

	return (result);
}

// ---
QGAMES::World* PacManII::WorldBuilder::createWorldObject (int no, const QGAMES::Scenes& s, const QGAMES::WorldProperties& p)
{
	// By deefault, all are the same...
	return (new PacManII::World (no, s, p));
}

// ---
QGAMES::Scene* PacManII::WorldBuilder::createSceneObject (int ns, const QGAMES::Maps& m, 
	const QGAMES::Scene::Connections& cn, const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
{
	QGAMES::Scene* result = nullptr;

	if (ns >= __PACMANII_BASICSCENE__ &&
		ns < (__PACMANII_BASICSCENE__ + __PACMANII_MAXNUMBERLEVELS__))
		// The monsters and the fruit are defined in the constructor...
		result = new PacManII::BasicScene (ns, m, cn, p, ePL);
	else
		// The elments to play with have to b defined in the XML file as action blocks...
		result = new PacManII::StandardScene (ns, m, cn, p, ePL);

	return (result);
}

// ---
QGAMES::SceneActionBlock* PacManII::WorldBuilder::createSceneActionBlockObject (int nAB, 
	const QGAMES::SceneActionBlockProperties& prps)
{
	QGAMES::SceneActionBlock* result = nullptr;

	if (nAB >= __PACMANII_MONSTERSCENEBLOCKBASEID__ && 
		nAB < (__PACMANII_MONSTERSCENEBLOCKBASEID__ + __PACMANII_MONSTERSCENEBLOCKNUMBER__))
		result = new PacManII::MonsterSceneActionBlock (nAB, PacManII::MonsterSceneActionBlock::Properties (prps));
	else
	if (nAB >= __PACMANII_FRUITSCENEBLOCKBASEID__ && 
		nAB < (__PACMANII_FRUITSCENEBLOCKBASEID__ + __PACMANII_FRUITSCENEBLOCKNUMBER__))
		result = new PacManII::FruitSceneActionBlock (nAB, PacManII::FruitSceneActionBlock::Properties (prps));

	return (result);
}

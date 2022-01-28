#include "Builders.hpp"
#include "Pacman.hpp"
#include "Monsters.hpp"
#include "Shield.hpp"
#include "Defs.hpp"

// ---
QGAMES::Movement* PacManII::MovementBuilder::createMovement 
	(const QGAMES::MovementBuilder::MovementDefinition& def)
{
	QGAMES::Movement* result = nullptr;

	// (Wormy) Monsters' chasing movements
	if (def._id >= __PACMANII_WORMYMAZEMOVEMENTBASEID__ &&
		 def._id < (__PACMANII_WORMYMAZEMOVEMENTBASEID__ + __PACMAN_NUMBERENTITIES__)) 
		result = new PACMAN::MazeMovement (def._id, def._variables);
	else
	// (Wormy) Monsters' at-home movements
	if (def._id >= __PACMANII_WORMYATHOMEMOVEMENTBASEID__ &&
		 def._id < (__PACMANII_WORMYATHOMEMOVEMENTBASEID__ + __PACMAN_NUMBERENTITIES__))
		result = new QGAMES::NoMovement (def._id, def._variables);
	else
	// Shield's staying movement
	if (def._id >= __PACMAN_SHIELDSTAYINGMOVEMENTBASEID__ &&
		 def._id < (__PACMAN_SHIELDSTAYINGMOVEMENTBASEID__ + __PACMAN_NUMBERENTITIES__))
		result = new QGAMES::NoMovement (def._id, def._variables);
	// For th rest of the movements the system relies in the standard framework...
	else
		result = PACMAN::MovementBuilder::createMovement (def);

	return (result);
}

// ---
QGAMES::Entity* PacManII::EntityBuilder::createEntity (const QGAMES::EntityBuilder::EntityDefinition& def)
{
	QGAMES::Entity* result = nullptr;

	// The pacman used in this instance is a little but different
	// It can be shield a little bit!
	if (def._id >= __PACMAN_PACMANBASEENTITYID__ && 
		def._id < (__PACMAN_PACMANBASEENTITYID__ + __PACMAN_NUMBERENTITIES__)) 
			result = new PacManII::PacMan (def._id);
	else 
	// To create the new pacman monster for this version
	if (def._id >= __PACMAN_WORMYBASEENTITYID__ && 
		def._id < (__PACMAN_WORMYBASEENTITYID__ + __PACMAN_NUMBERENTITIES__)) 
			result = new PacManII::Wormy (def._id);
	else
	// To create th shields
	if (def._id >= __PACMAN_SHIELDBASEENTITYID__ &&
		def._id < (__PACMAN_SHIELDBASEENTITYID__ + __PACMAN_NUMBERENTITIES__))
			result = new PacManII::Shield (def._id);
	// For any other entity the default implementation can be usd...
	else 
		result = PACMAN::EntityBuilder::createEntity (def);

	return (result);
}

// ---
QGAMES::Scene* PacManII::WorldBuilder::createSceneObject (int ns, const QGAMES::Maps& m, 
	const QGAMES::Scene::Connections& cn, const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
{
	QGAMES::Scene* result = nullptr;

	// TODO
	result = PACMAN::WorldBuilder::createSceneObject (ns, m, cn, p, ePL);

	return (result);
}

// ---
QGAMES::SceneActionBlock* PacManII::WorldBuilder::createSceneActionBlockObject (int nAB, 
	const QGAMES::SceneActionBlockProperties& prps)
{
	QGAMES::SceneActionBlock* result = nullptr;

	// TODO
	result = PACMAN::WorldBuilder::createSceneActionBlockObject (nAB, prps);

	return (result);
}

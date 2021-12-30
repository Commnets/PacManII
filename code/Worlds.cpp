#include "Worlds.hpp"
#include "Scenes.hpp"
#include "Maps.hpp"
#include "Game.hpp"
#include "SceneActionBlocks.hpp"

// ---
PacManII::World::World (int c, const QGAMES::Scenes& s, const QGAMES::WorldProperties& p)
	: QGAMES::World (c, s, p)
{
#ifndef NDEBUG
	for (auto i : scenes ())
		assert (dynamic_cast <PacManII::Scene*> (i.second) != nullptr);
#endif
}

// ---		
QGAMES::SetOfOpenValues PacManII::World::runtimeValues () const
{ 
	const PacManII::Game* dG = dynamic_cast <const PacManII::Game*> (game ());
	assert (dG != nullptr); // Just in case...

	// Remember that world's runtime values are kept per player
	// Remember also that the runtime info of the characters and entities of a scene are not saved
	// But blocks do, and dragonwind blocks save also in many ocassions entities' info
	// So, e.g, if playing in the scene 2 when the runtime values of the blocks of the scene 1 are got
	// The info of the characters will be the one of the scene 2. This is not consistent
	QGAMES::SetOfOpenValues pOV = currentPlayerRuntimeValues ();

	// If there is no active scene then the info is simply returned
	if (activeScene () == NULL)
	{
		_fullConfPerPlayer.addSetOfOpenValues (dG -> currentPlayer (), pOV);
		return (_fullConfPerPlayer);
	}

	// Get the scenes info of the current runtime values...
	// Only the one for the active scene will be usefull...
	int lNE1 = pOV.lastNestedOpenValueId (); // It is supossed it is the scenes info...
	QGAMES::SetOfOpenValues pyScnOv1 = pOV.setOfOpenValues (lNE1);
	assert (pyScnOv1.name () == std::string (__QGAMES_RUNTIMEVALUESSCENESTYPE__)); // ...but double check just in case...
	
	// If it is the first time the player enters the world, there is nothing else to do...
	// Except removing the info not related with this specific scene!
	if (!_fullConfPerPlayer.existSetOfOpenValues (dG -> currentPlayer ()))
	{
		for (QGAMES::Scenes::const_iterator i = scenes ().begin (); i != scenes ().end (); i++)
		{
			if ((*i).first != activeScene () -> id () &&
				pyScnOv1.existSetOfOpenValues ((*i).first))
				pyScnOv1.removeSetOfOpenValues ((*i).first);
		}
	}

	// Otherwise the info of the previous visits have to be recovered!
	// From the info already saved in _fullConfPlayer!
	else
	{
		QGAMES::SetOfOpenValues pyWOv = _fullConfPerPlayer.setOfOpenValues (dG -> currentPlayer ());
		int lNE2 = pyWOv.lastNestedOpenValueId (); // It is supossed it is the scenes info...
		QGAMES::SetOfOpenValues pyScnOv2 = pyWOv.setOfOpenValues (lNE2);
		assert (pyScnOv2.name () == std::string (__QGAMES_RUNTIMEVALUESSCENESTYPE__)); // ...but double check just in case...

		// The older scenes' info is recovered...
		// ..and the information of the current scene is maintained!
		for (QGAMES::Scenes::const_iterator i = scenes ().begin (); i != scenes ().end (); i++)
		{
			if ((*i).first != activeScene () -> id ())
			{
				if (pyScnOv2.existSetOfOpenValues ((*i).first)) 
					pyScnOv1.addSetOfOpenValues ((*i).first, pyScnOv2.setOfOpenValues ((*i).first)); // Replace...
				else
				if (pyScnOv1.existSetOfOpenValues ((*i).first))
					pyScnOv1.removeSetOfOpenValues ((*i).first); // It is not necessary...
			}
		}
	}

	pOV.addSetOfOpenValues (lNE1, pyScnOv1);

	// Finally creates the info per player, and returns it...
	_fullConfPerPlayer.addSetOfOpenValues (dG -> currentPlayer (), pOV); // Replace the previous one...
	return (_fullConfPerPlayer);
}

// ---
void PacManII::World::setClapperBoard (bool o)
{
	// Only over the active scene if any...
	PacManII::Scene* scn = nullptr;
	if ((scn = dynamic_cast <PacManII::Scene*> (activeScene ())) != nullptr)
		scn -> setClapperBoard (o);
}

// ---
bool PacManII::World::clapperBoard () const
{
	const PacManII::Scene* scn = dynamic_cast <const PacManII::Scene*> (activeScene ());
	return ((scn != nullptr) ? scn -> clapperBoard () : false);
}

// ---
void PacManII::World::startBlinking (QGAMES::bdata bT, int nB)
{
	// Only over the active scene if any...
	PacManII::Scene* scn = nullptr;
	if ((scn = dynamic_cast <PacManII::Scene*> (activeScene ())) != nullptr)
		scn -> startBlinking (bT, nB);
}

// ---
bool PacManII::World::isBlinking () const
{
	// Only over the active scene if any...
	const PacManII::Scene* scn = dynamic_cast <const PacManII::Scene*> (activeScene ());
	return ((scn != nullptr) ? scn -> isBlinking () : false);
}

// ---
void PacManII::World::stopBlinking ()
{
	// Only over the active scene if any...
	PacManII::Scene* scn = nullptr;
	if ((scn = dynamic_cast <PacManII::Scene*> (activeScene ())) != nullptr)
		scn -> stopBlinking ();
}

// ---
int PacManII::World::maxNumberBallsToEat () const
{
	// Only over the active scene if any...
	const PacManII::Scene* scn = dynamic_cast <const PacManII::Scene*> (activeScene ());
	return ((scn != nullptr) ? scn -> maxNumberBallsToEat () : 0);
}

// ---
int PacManII::World::numberBallsEaten () const
{
	// Only over the active scene if any...
	const PacManII::Scene* scn = dynamic_cast <const PacManII::Scene*> (activeScene ());
	return ((scn != nullptr) ? scn -> numberBallsEaten () : 0);
}

// ---
std::string PacManII::World::ballsEatenStatus () const
{
	// Only over the active scene if any...
	const PacManII::Scene* scn = dynamic_cast <const PacManII::Scene*> (activeScene ());
	return ((scn != nullptr) ? scn -> ballsEatenStatus () : std::string (__NULL_STRING__));
}

// ---
void PacManII::World::setBallsEatenStatus (const std::string& st)
{
	// Only over the active scene if any...
	PacManII::Scene* scn = nullptr;
	if ((scn = dynamic_cast <PacManII::Scene*> (activeScene ())) != nullptr)
		scn -> setBallsEatenStatus (st);
}

// ---
void PacManII::World::playSiren (bool f)
{
	// Only over the active scene if any...
	PacManII::Scene* scn = nullptr;
	if ((scn = dynamic_cast <PacManII::Scene*> (activeScene ())) != nullptr)
		scn -> playSiren (f);
}

// ---
void PacManII::World::stopSiren ()
{
	// Only over the active scene if any...
	PacManII::Scene* scn = nullptr;
	if ((scn = dynamic_cast <PacManII::Scene*> (activeScene ())) != nullptr)
		scn -> stopSiren ();
}

// ---
void PacManII::World::initialize ()
{
	QGAMES::World::initialize ();

	PacManII::Game* dG = dynamic_cast <PacManII::Game*> (game ());
	assert (dG != nullptr);
	PacManII::Game::Conf* cfg = dynamic_cast <PacManII::Game::Conf*> (dG -> configuration ());
	assert (cfg != nullptr); // Just in case...

	// Gets back the information kept for that world, if it exists
	// Otherwise an empty element is set up...
	// The world keeps different information per player (id)
	_fullConfPerPlayer = QGAMES::SetOfOpenValues (std::string (__QGAMES_RUNTIMEVALUESELMNTTYPE__));
	if (cfg -> existConfValueForElement (id ()))
		_fullConfPerPlayer = cfg -> confValueForElement (id ());
	
	// Gets back the specific information of the world for the current player, if it exists
	// That information won't exist if the player hasn't ever played in this world!
	// So, in that situation there won't be anything to configure beyond the work
	// already done at initialization time...
	if (_fullConfPerPlayer.existSetOfOpenValues (dG -> currentPlayer ()))
	{
		// Remember that the characters and entities used in the scene are reused in other and with other players
		// On the other hand, the world is initialized any time a scene starts back
		// So the information kept must be used to initialize just the current scene forgetting the rest...
		QGAMES::SetOfOpenValues pyWOv = _fullConfPerPlayer.setOfOpenValues (dG -> currentPlayer ());
		int lNE = pyWOv.lastNestedOpenValueId (); // It is supossed it is the scenes info...
		QGAMES::SetOfOpenValues pyScnOv = pyWOv.setOfOpenValues (lNE);
		assert (pyScnOv.name () == std::string (__QGAMES_RUNTIMEVALUESSCENESTYPE__)); // ...but double check just in case...

		for (QGAMES::Scenes::const_iterator i = scenes ().begin (); i != scenes ().end (); i++)
			if ((*i).first != activeScene () -> id () && pyScnOv.existSetOfOpenValues ((*i).first))
				pyScnOv.removeSetOfOpenValues ((*i).first); 

		pyWOv.addSetOfOpenValues (lNE, pyScnOv);
		
		// The initialization then takes place...

		QGAMES::World::initializeRuntimeValuesFrom (pyWOv); 
	}
}

// ---
void PacManII::World::finalize ()
{
	// The current status of the game is stored...
	runtimeValues ();

	// ...and put it back into the configuration object...
	PacManII::Game::Conf* cfg = dynamic_cast <PacManII::Game::Conf*> (game () -> configuration ());
	assert (cfg != nullptr); // Just in case...
	cfg -> setConfValueForElement (id (), _fullConfPerPlayer);

	QGAMES::World::finalize ();
}

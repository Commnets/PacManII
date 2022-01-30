#include "DataGame.hpp"

// ---
PacManII::DataGame* PacManII::DataGame::mineDataGame (int nL)
{
	// TODO

	return (nullptr);
}

// ---
PacManII::DataGame* PacManII::DataGame::hardDataGame (int nL)
{
	// TODO

	return (nullptr);
}

// ---
PacManII::DataGame::DataGame (const PacManII::DataGame& d)
	: PACMAN::DataGame 
		([=](const PACMAN::DataGame::LevelDefinitions& ls) -> PACMAN::DataGame::LevelDefinitions 
			{ 
				PACMAN::DataGame::LevelDefinitions result; 
				for (auto i : ls)
				{
					// All must be of the same type...
					assert (dynamic_cast <PacManII::DataGame::LevelDefinition*> (i) != nullptr);
					result.push_back (new PacManII::DataGame::LevelDefinition 
						(*(static_cast <PacManII::DataGame::LevelDefinition*> (i))));
				}

				return (result);
			}(d.levels ()), 
		 d.everyToGetAnExtraLive ())
{
	// Nothing special to build up here...
}

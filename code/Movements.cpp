#include "Movements.hpp"
#include "Artists.hpp"

const QGAMES::bdata PacManII::MazeMovement::_CONSTANT = 2.5;

// ---
void PacManII::MazeMovement::initialize ()
{ 
	_speed = __BD 1;

	_lastDirection = QGAMES::Vector::_cero;

	_qLeft = __BD 0;
}

// ---
void PacManII::MazeMovement::initializeFrom (const QGAMES::Movement* m)
{
	if (dynamic_cast <const PacManII::MazeMovement*> (m) == nullptr)
		initialize ();
	else
	{
		assert (dynamic_cast <const PacManII::MazeMovement*> (m) != nullptr);

		const PacManII::MazeMovement* mM = dynamic_cast <const PacManII::MazeMovement*> (m);

		_speed = mM -> _speed;

		_lastDirection = mM -> _lastDirection;

		_qLeft = __BD 0;
	}
}

// ---
void PacManII::MazeMovement::move (const QGAMES::Vector& d, const QGAMES::Vector& a, QGAMES::Entity* e)
{
	PacManII::Artist* art = dynamic_cast <PacManII::Artist*> (e);
	assert (art != nullptr);

	// If there is no path to move into...
	if (art -> nextMazePosition (0) == QGAMES::MazeModel::_noPosition)
		return; // ...makes no sense to progress...

	// If the direction has changed out of the movement in a intermediate position
	// betwen two tiles. This only can happen with Pacman...(so far)
	if (art -> orientation () != _lastDirection)
		_lastDirection = art -> orientation ();

	bool stopCycle = false; // to notify only once when stopping...
	for (int i = (int) ((_speed * _CONSTANT) + _qLeft); i > 0 && !stopCycle; i--)
	{
		bool move = true;

		// When the artist matches a tile, many things have to be taken into account:
		// 1.- The path in the maze has always to be recalculated
		// 2.- Doing so, the new state of the artist (if it has changed) can be taken into account.
		// 3.- If there was a pending decision to be taken regarding the movement (is the case of Pacman e.g)
		//	   it will be also taken into account ehen calculating the path again.	
		if (art -> doesPositionMatchesTile (art -> position ()))
		{
			if (art -> nextMazePosition (0) != art -> currentMazePosition ())
				art -> recalculatePathInMaze ();
			art -> changeDirectionWhenPossibleTo (QGAMES::Vector::_cero); // If any, it was taken into account before...

			// If there is a next position to go to...
			if (art -> nextMazePosition (1) != QGAMES::MazeModel::_noPosition)
			{
				// But is a tunnel...
				// So a previous change in the position (leap) has to be done
				// and as a consequnce the path has to be calculated again!
				if (art -> isArtistEnteringATunnel ())
				{
					art -> setPosition 
						(art -> mazePositionToMapPosition (art -> nextMazePosition (1)) - 
							QGAMES::Vector (__BD (art -> visualLength () >> 1), __BD (art -> visualHeight () >> 1), __BD 0));
					art -> recalculatePathInMaze (); // After being moved, the path hast to be calculated...
				}

				// Depeding on the type of artist things can happen when it is on a new position...
				art -> whatToDoOnCurrentPosition ();

				_lastDirection = art -> nextMazePosition (1).asVector () - 
					art -> nextMazePosition (0).asVector (); 

				art -> setMove (_lastDirection, QGAMES::Vector::_cero); 

				// If the new position if the maze
				// implies a change in the previous orientation of the artist
				// a notification is launched for the owner to change also the status...
				if (art -> orientation () != _lastDirection)
				{
					QGAMES::SetOfOpenValues dC; 
					dC.addOpenValue (0, QGAMES::OpenValue ((std::stringstream () << _lastDirection).str ()));
					notify (QGAMES::Event (__PACMANII_MOVEMENTDORIENTATIONCHANGED__, this, dC)); 
				}
			}

			// If there is no other position to go to, 
			// it is time to stop. So, a notification in launched for the owner to execute!
			else
			{
				art -> whatToDoOnCurrentPosition ();

				stopCycle = true;

				_lastDirection = QGAMES::Vector::_cero;

				move = false;

				notify (QGAMES::Event (__PACMANII_LIMITMOVEMENTREACHED__, this));
			}
		}

		if (move && art -> canMove (_lastDirection, QGAMES::Vector::_cero))
			art -> setPosition (art -> position () + _lastDirection); // 1 pixel no more in each iteration...
	}

	// Keps the remaining for the next iteration...
	_qLeft = stopCycle 
		? __BD 0 : ((_speed * __BD _CONSTANT) + _qLeft) - __BD ((int) ((_speed * __BD _CONSTANT) + _qLeft));
}

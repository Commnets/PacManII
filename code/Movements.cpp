#include "Movements.hpp"
#include "Artists.hpp"

// ---
void PacManII::MazeMovement::initialize ()
{ 
	_speed = __BD 1;

	_pathToFollow = { };

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

		_pathToFollow = mM -> _pathToFollow;

		_lastDirection = mM -> _lastDirection;

		_qLeft = __BD 0;
	}
}

// ---
void PacManII::MazeMovement::move (const QGAMES::Vector& d, const QGAMES::Vector& a, QGAMES::Entity* e)
{
	assert (dynamic_cast <PacManII::Artist*> (e) != nullptr);
	PacManII::Artist* art = dynamic_cast <PacManII::Artist*> (e);

	// If there is no path to move into...
	if (art -> nextMazePosition (0) == QGAMES::MazeModel::_noPosition)
		return; // ...makes no sense to progress...

	// The initial direction of the movement is the one the artist have already...
	// But it can change along the cycle!
	_lastDirection = art -> direction ();

	// The number of pixels to advance in this cycle, will depend on the speed...
	for (int i = (int) ((_speed * __BD _CONSTANT) + _qLeft); i > 0; i--)
	{
		if (art -> positionMatchesTile (art -> position ()))
		{
			// If there is a next position possible, the movement has to continue
			// until there is no more pixels to move!
			if (art -> nextMazePosition (1) != QGAMES::MazeModel::_noPosition)
			{
				_lastDirection = art -> nextMazePosition 
					(1).asVector () - art -> nextMazePosition (0).asVector ();
				art -> setOrientation (_lastDirection); // It would change the aspect of the artist too...
				art -> setMove (_lastDirection, QGAMES::Vector::_cero); // Change the movement...
				art -> extractStepsFromPath (1); // The last step has already bn used...
			}
			else
				notify (QGAMES::Event (__PACMANII_LIMITMOVEMENTREACHED__, this));
		}

		if (art -> canMove (_lastDirection, QGAMES::Vector::_cero))
			art -> setPosition (art -> position () + _lastDirection); // 1 pixel...
	}

	// Moves what is left...
	_qLeft = ((_speed * __BD _CONSTANT) + _qLeft) - __BD ((int) ((_speed * __BD _CONSTANT) + _qLeft));
}

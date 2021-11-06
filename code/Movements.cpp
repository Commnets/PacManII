#include "Movements.hpp"
#include "Artists.hpp"


// ---
void PacManII::MazeMovement::initialize ()
{ 
	_currentMovDirection = _nextMovDirection = QGAMES::Vector::_cero;

	_position = QGAMES::Position::_cero;

	_firstMovement = true;

	_inc = __BD 0;
}

// ---
void PacManII::MazeMovement::initializeFrom (const QGAMES::Movement* m)
{
	assert (dynamic_cast <const PacManII::MazeMovement*> (m) != nullptr);

	const PacManII::MazeMovement* mM = dynamic_cast <const PacManII::MazeMovement*> (m);
}

// ---
void PacManII::MazeMovement::move (const QGAMES::Vector& d, const QGAMES::Vector& a, QGAMES::Entity* e)
{
	assert (dynamic_cast <PacManII::Artist*> (e) != nullptr);

	// The acceleration is not used at all...

	if (_firstMovement)
	{
		_firstMovement = false;
	}
	else
	{
	}

	_position = e -> position ();
	QGAMES::Vector tM = _currentMovDirection * _inc;
	int incAX = (int) tM.posX ();
	int incAY = (int) tM.posY ();

	bool lR = false;
	for (int i = 0; i < incAX && !lR; i++)
	{
	}

	if (!lR)
	{
		for (int i = 0; i < incAY && !lR; i++)
		{
		}
	}

	if (lR)
	{
	}
}

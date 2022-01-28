#include "Shield.hpp"
#include "Defs.hpp"

// ---
void PacManII::Shield::setType (int t)
{
	assert (t < __PACMANII_NUMBERTYPESSHIELD__);

	_type = t;

	switch (_type)
	{
		case 0:
			setCurrentState (__PACMANII_SHIELDTYPE1__);
			break;

		case 1:
			setCurrentState (__PACMANII_SHIELDTYPE2__);
			break;

		case 2:
			setCurrentState (__PACMANII_SHIELDTYPE3__);
			break;

		default:
			assert (false);
	};
}

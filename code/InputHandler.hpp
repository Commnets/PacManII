/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: InputHandler.hpp \n
 *	Game: PacManII \n
 *	Author: Ignacio Cea Forniés (Community Networks) \n
 *	Creation Date: 22/08/2021 \n
 *	Description: To manipulate the input handler of the PacManII game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_INPUTHANDLER__
#define __PACMANII_INPUTHANDLER__

#include "Defs.hpp"
#include <advancedArcade/adinclude.hpp>

namespace PacManII
{
	class InputHandlerBehaviour final : public QGAMES::InputHandler::StandardBehavior
	{
		public:
		/** To group notification Data. */
		struct NotificationData final
		{
			NotificationData ()
				: _force (0), // Fire force...
				  _upOn (false), _downOn (false), _rightOn (false), _leftOn (false)
							{ }

			NotificationData (const NotificationData& nD) = default;

			NotificationData& operator = (NotificationData& nD) = default;

			void clear () 
							{ _force = 0; 
							  _downOn = _upOn = _leftOn = _rightOn = false; }

			int _force;
			bool _upOn;
			bool _downOn;
			bool _leftOn;
			bool _rightOn;
		};

		InputHandlerBehaviour ();

		InputHandlerBehaviour (const InputHandlerBehaviour&) = delete;

		InputHandlerBehaviour& operator = (const InputHandlerBehaviour&) = delete;

		~InputHandlerBehaviour ()
							{ delete (_notificationData); }

		virtual void onKeyboardStatus (const std::vector <bool>& kS) override;
		virtual void manageKeyOnState (QGAMES::GameState* st, int kP) override { }
		virtual void manageJoystickMovementOnState (QGAMES::GameState* st, int nJ, const QGAMES::Vector& dr) override;
		virtual void manageJoystickButtonOnState (QGAMES::GameState* st, int nJ, bool p, int nB, int bF) override;

		private:
		// Implementation Data
		NotificationData* _notificationData;
		bool _force;
		bool _downMovementOn; 
		bool _upMovementOn;
		bool _rightMovementOn;
		bool _leftMovementOn;
	};
}

#endif
  
// End of the file
/*@}*/

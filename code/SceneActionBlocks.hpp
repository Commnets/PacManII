/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: Artists.hpp \n
 *	Game: PacManII \n
 *	Author: Ignacio Cea Forniés (Community Networks) \n
 *	Creation Date: 22/08/2021 \n
 *	Description: PacmanII artists main definition file. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_ACTIONBLOCKS__
#define __PACMANII_ACTIONBLOCKS__

#include "Defs.hpp"
#include "Artists.hpp"
#include <advancedArcade/adinclude.hpp>

namespace PacManII
{
	/** The behaviour of a monster. */
	class MonsterSceneActionBlock final : public QGAMES::SceneActionBlock
	{
		public:
		struct Properties
		{
			Properties ()
				: _entityId (0), _numberInMap (0), 
				  _offsetXInitPosition (__BD 0), _offsetYInitPosition (__BD 0)
							{ }

			Properties (int eId, int nM, QGAMES::bdata oX, QGAMES::bdata oY)
				: _entityId (eId), _numberInMap (nM), 
				  _offsetXInitPosition (oX), _offsetYInitPosition (oY)
							{ }

			Properties (const QGAMES::SceneActionBlockProperties& prps);

			Properties (const Properties&) = default;

			Properties& operator = (const Properties&) = default;

			int _entityId;
			int _numberInMap;
			QGAMES::bdata _offsetXInitPosition;
			QGAMES::bdata _offsetYInitPosition;
		};

		MonsterSceneActionBlock () = delete;

		MonsterSceneActionBlock (int id, const Properties& prps)
			: QGAMES::SceneActionBlock (id),
			  _properties (prps),
			  _monster (nullptr)
							{ }

		MonsterSceneActionBlock (const MonsterSceneActionBlock&) = delete;

		MonsterSceneActionBlock& operator = (const MonsterSceneActionBlock&) = delete; 

		const Properties& properties () const
							{ return (_properties); }

		virtual void initialize () override;
		virtual void updatePositions () override;
		virtual void finalize () override;

		private:
		/** The properties of the action block. */
		Properties _properties;

		private:
		// Implementation
		Monster* _monster;
	};

	/** The behaviour of the fruit. */
	class FruitSceneActionBlock final : public QGAMES::SceneActionBlock
	{
		public:
		struct Properties
		{
			Properties ()
				: _entityId (0),
				  _type (0), _points (200),
				  _secondsToAppear (__BD 7), _secondsToDisappear (__BD 7)
							{ }

			Properties (int eId, int tp, int p, QGAMES::bdata sA, QGAMES::bdata sD)
				: _entityId (eId),
				  _type (tp), _points (p),
				  _secondsToAppear (sA), _secondsToDisappear (sD)
							{ }

			Properties (const QGAMES::SceneActionBlockProperties& prps);

			Properties (const Properties&) = default;

			Properties& operator = (const Properties&) = default;

			int _entityId;
			int _type;
			int _points;
			QGAMES::bdata _secondsToAppear;
			QGAMES::bdata _secondsToDisappear;
		};

		FruitSceneActionBlock () = delete;

		FruitSceneActionBlock (int id, const Properties& prps)
			: QGAMES::SceneActionBlock (id),
			  _properties (prps),
			  _fruit (nullptr)
							{ }

		FruitSceneActionBlock (const FruitSceneActionBlock&) = delete;

		FruitSceneActionBlock& operator = (const FruitSceneActionBlock&) = delete; 

		const Properties& properties () const
							{ return (_properties); }

		virtual void initialize () override;
		virtual void updatePositions () override;
		virtual void finalize () override;

		private:
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters () override
							{ return (new Counters); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches)
		virtual QGAMES::OnOffSwitches* createOnOffSwitches () override
							{ return (new OnOffSwitches); }

		private:
		/** The properties. */
		Properties _properties;

		/** The counters and the switches. */
		static const int _COUNTERTOAPPEARFRUIT = 0;
		static const int _COUNTERTODISAPPEARFRUIT = 1;
		static const int _SWITCHFRUITONSCENE = 0;
		static const int _SWITCHFRUITSHOWN = 1;

		// Implementation
		Fruit* _fruit;
	};
}

#endif
  
// End of the file
/*@}*/
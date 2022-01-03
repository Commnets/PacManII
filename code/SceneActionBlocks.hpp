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
	class MonsterSceneActionBlock : public QGAMES::SceneActionBlock
	{
		public:
		struct Properties
		{
			Properties ()
				: _entityId (0), _numberInMap (0), _points (0)
							{ }

			Properties (int eId, int nM, int p)
				: _entityId (eId), _numberInMap (nM), _points (p)
							{ }

			Properties (const QGAMES::SceneActionBlockProperties& prps);

			Properties (const Properties&) = default;

			Properties& operator = (const Properties&) = default;

			int _entityId;
			int _numberInMap;
			int _points;
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

		// To manage the action block from the scene...
		/** To know the number of monster being managed here. */
		int monsterNumber () const
							{ return ((_monster == nullptr) ? -1 : _monster -> monsterNumber ()); }
		/** To know whether the monster is already moving. */
		bool isMonsterMoving () const
							{ return (onOffSwitch (_SWITCHMOVING) -> isOn ()); }
		/** When te scene determines a monster has to departure, 
			its number is communicated to all action blocks. 
			Returns true when the instruction was accepted. */
		bool timeToStart (int nM);

		virtual void initialize () override;
		virtual void updatePositions () override;
		virtual void finalize () override;

		virtual void processEvent (const QGAMES::Event& evnt) override;

		protected:
		/** To determine other potential conditions to start moving. 
			It could be overloaded later. By default takes into account the number of balls eaten. */
		virtual bool readyToStart (); 

		__DECLAREONOFFSWITCHES__ (OnOffSwitches)
		virtual QGAMES::OnOffSwitches* createOnOffSwitches () override
							{ return (new OnOffSwitches); }

		protected:
		/** The properties of the action block. */
		Properties _properties;

		/** The switches. */
		static const int _SWITCHTOSTART = 0;
		static const int _SWITCHMOVING = 1;

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
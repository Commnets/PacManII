/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: SceneActionBlocks.hpp \n
 *	Game: PacManII \n
 *	Author: Ignacio Cea Forniés (Community Networks) \n
 *	Creation Date: 29/01/2021 \n
 *	Description: Define the main action blocks of any PacManII game: \n
 *				 To control how a monster appears random in the maze and starts to move, 
 *				 and a shield to appear in a specific position once it has been decided so.
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_SCENEACTIONBLOCKS__
#define __PACMANII_SCENEACTIONBLOCKS__

#include <PacManLike/pcinclude.hpp>

namespace PacManII
{
	/** An element to appear in a random place of the maze and then anothr action block takes the control. */
	class ElementToAppearSceneActionBlock : public PACMAN::SceneActionBlock
	{
		public:
		struct Properties  
		{
			Properties ()
				: _entityId (0),
				  _position (QGAMES::MazeModel::_noPosition),
				  _ballsEatenToAppear (40),
				  _secondsMaxToAppear (__BD 2.0),
				  _otherProperties ()
							{ }

			Properties (int eId, const QGAMES::MazeModel::PositionInMaze& p, int bA, QGAMES::bdata sA,
					const QGAMES::SceneActionBlockProperties& prps)
				: _entityId (eId),
				  _position (p),
				  _ballsEatenToAppear (bA), 
				  _secondsMaxToAppear (sA),
				  _otherProperties (prps)
							{ assert (_position != QGAMES::MazeModel::_noPosition); }

			Properties (const QGAMES::SceneActionBlockProperties& prps);

			Properties (const Properties&) = default;

			Properties& operator = (const Properties&) = default;

			int _entityId;
			QGAMES::MazeModel::PositionInMaze _position;
			int _ballsEatenToAppear;
			QGAMES::bdata _secondsMaxToAppear;
			QGAMES::SceneActionBlockProperties _otherProperties;
		};

		ElementToAppearSceneActionBlock (int id, const Properties& prps)
			: PACMAN::SceneActionBlock (id),
			  _properties (prps),
			  _element (nullptr),
			  _afterAppearingActionBlock (nullptr)
							{ }

		virtual QGAMES::SetOfOpenValues runtimeValues () const override;
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg) override;

		// Never change properties after being initialized...
		void setProperties (const Properties& prps)
							{ _properties = prps; }
		const Properties& properties () const
							{ return (_properties); }

		virtual void initialize () override;
		virtual void updatePositions () override;
		virtual void finalize () override;

		virtual void processEvent (const QGAMES::Event& evnt) override;

		protected:
		/** To create the action block after appearing. \n 
			It has to be redefined later. */
		virtual PACMAN::SceneActionBlock* createAfterAppearingActionBlock () = 0;

		__DECLARECOUNTERS__ (Counters)
		virtual QGAMES::Counters* createCounters () override
							{ return (new Counters); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches)
		virtual QGAMES::OnOffSwitches* createOnOffSwitches () override
							{ return (new OnOffSwitches); }

		protected:
		/** The properties of the action block. */
		Properties _properties;

		/** The counters & switches. */
		static const int _COUNTERTOAPPEAR = 0;
		static const int _COUNTERTOFADE = 1;
		static const int _COUNTERFADE = 2;
		static const int _SWITCHBLOCKACTIVE = 0;
		static const int _SWITCHAPPEARING = 1;
		static const int _SWITCHAPPEARED = 2;
		static const int _FADE [14];

		// Implementation
		PACMAN::PacmanElement* _element;
		PACMAN::SceneActionBlock* _afterAppearingActionBlock;
	};

	/** An action block to appear and move a monster in a random plac of the maze. 
		avoiding always the home of the monsters. */
	class MonsterToAppearAndMoveSceneActionBlock final : public ElementToAppearSceneActionBlock
	{
		public:
		MonsterToAppearAndMoveSceneActionBlock (int id, const Properties& prps)
			: ElementToAppearSceneActionBlock (id, prps)
							{ }

		private:
		virtual PACMAN::SceneActionBlock* createAfterAppearingActionBlock () override
							{ return (new PACMAN::MonsterSceneActionBlock (id () * 1000, 
								PACMAN::MonsterSceneActionBlock::Properties (_properties._otherProperties))); }
	};

	/** To appear a shield. */
	class ShieldToAppearSceneActionBlock final : public ElementToAppearSceneActionBlock
	{
		public:
		ShieldToAppearSceneActionBlock (int id, const Properties& prps)
			: ElementToAppearSceneActionBlock (id, prps)
							{ }

		private:
		virtual PACMAN::SceneActionBlock* createAfterAppearingActionBlock () override
							{ return (new PACMAN::ThingSceneActionBlock (id () * 1000, 
								PACMAN::ThingSceneActionBlock::Properties (_properties._otherProperties))); }
	};
}

#endif
  
// End of the file
/*@}*/
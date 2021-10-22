/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: ScoreObjects.hpp \n
 *	Game: PacManII \n
 *	Author: Ignacio Cea Forniés (Community Networks) \n
 *	Creation Date: 22/08/2021 \n
 *	Description: Define the score objects of the PacManII game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_SCOREOBJECTS__
#define __PACMANII_SCOREOBJECTS__

#include "Defs.hpp"

#include <advancedArcade/adinclude.hpp>

namespace PacManII
{
	/** The numbers of lives left. */
	class Lives final : public QGAMES::ScoreObject
	{
		public:
		Lives (int lF = __PACMANII_PACMANLIVESFORM__);

		virtual Entity* clone () const override
							{ return (new Lives (_livesForm)); }

		/** To set the number of lives. */
		void setLives (int l);
		/** To know the number of lives. */
		int lives () const
							{ return (_lives); }

		virtual void initialize () override;
		virtual void drawOn (QGAMES::Screen* s, 
			const QGAMES::Position& p = QGAMES::Position::_noPoint) override;

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		protected:
		/** The form used to represent the lives. */
		int _livesForm;

		// Implementation
		/** The number of lives. */
		int _lives;
	};

	/** To represent the time counter. */
	class TimeCounter final : public QGAMES::ScoreObjectText
	{
		public:
		TimeCounter (int f = __PACMANII_NEWTOUR24GREENLETTERS__);

		virtual Entity* clone () const override
								{ return (new TimeCounter ()); }

		/** To convert a time passed as parameter into something to be written. */
		void setSeconds (int nS);

		virtual void updatePositions () override;

		virtual void processEvent (const QGAMES::Event& evnt) override;

		protected:
		virtual int formToDrawLetters () override
								{ return (_font); }

		protected:
		/** The font used. */
		int _font;
	};

	/** A class to represent the score counter. */
	class ScoreCounter final : public QGAMES::ScoreObjectNumber
	{
		public:
		ScoreCounter (int f = __PACMANII_NEWTOUR24BLUELETTERS__);

		virtual Entity* clone () const override
								{ return (new ScoreCounter ()); }

		virtual void processEvent (const QGAMES::Event& evnt) override;

		protected:
		virtual int formToDrawDigits () override
								{ return (_font); }

		protected:
		/** The font used. */
		int _font;
	};

	/** A class to represent the a fix text in the screen. 
		It used e.g. to display the name of the player in use now. */
	class FixText final : public QGAMES::ScoreObjectText
	{
		public:
		FixText (int id, const std::string& fT, int f = __PACMANII_NEWTOUR24YELLOWLETTERS__);

		virtual Entity* clone () const override
								{ return (new FixText (_id, _text, _font)); }

		protected:
		virtual int formToDrawLetters () override
								{ return (_font); }

		protected:
		/** The font used. */
		int _font;
	};
}

#endif
  
// End of the file
/*@}*/	

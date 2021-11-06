/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: Defs.hpp \n
 *	Framework: QGAMES \n
 *	Author: Ignacio Cea Forni�s (Community Networks) \n
 *	Creation Date: 21/08/2021 \n
 *	Description: Defines and general includes of PacManII
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANIIDEFINES__
#define __PACMANIIDEFINES__

#include <Room/rminclude.hpp>

// Configuration parameters
#define __PACMANII_MAXNUMBEROFLIVESPARAMETER__						"LIVES"
#define __PACMANII_DEFAULTMAXNUMBEROFLIVESPARAMETER__				"3"
#define __PACMANII_LEVELPARAMETER__									"LEVEL"
#define __PACMANII_DEFAULTLEVELPARAMETER__							"1"
#define __PACMANII_MAXNUMBERLEVELS__								100
#define __PACMANII_SHOWPFSPARAMETER__								"FPS"
#define __PACMANII_DEFAULTSHOWFPSPARAMETER__						"NO"
#define __PACMANII_MAXNUMBEROFLIVESINSCREEN__						4

// Important forms
#define __PACMANII_PACMANLIVESFORM__								50000

// Sounds
#define __PACMANII_SOUNDBEGINNING__									50000
#define __PACMANII_SOUNDCHOMP__										50001
#define __PACMANII_SOUNDDEATH__										50002
#define __PACMANII_SOUNDEATFRUIT__									50003
#define __PACMANII_SOUNDEATGHOST__									50004
#define __PACMANII_SOUNDEXTRAPAC__									50005
#define __PACMANII_SOUNDINTERMISSION__								50006
#define __PACMANII_SOUNDSIREN__										50007
#define __PACMANII_SOUNDSIRENFAST__									50007
#define __PACMANII_SOUNDSIRENVERYFAST__								50007

// Letters
#define __PACMANII_NEWTOURLETTERS__									60000
#define __PACMANII_NEWTOUR24BLUELETTERS__							60000
#define __PACMANII_NEWTOUR24GREENLETTERS__							60001
#define __PACMANII_NEWTOUR24REDLETTERS__							60002
#define __PACMANII_NEWTOUR24YELLOWLETTERS__							60003
#define __PACMANII_NEWTOUR36BLUELETTERS__							60010
#define __PACMANII_NEWTOUR36GREENLETTERS__							60011
#define __PACMANII_NEWTOUR36REDLETTERS__							60012
#define __PACMANII_NEWTOUR36YELLOWLETTERS__							60013
#define __PACMANII_NEWTOUR48BLUELETTERS__							60020
#define __PACMANII_NEWTOUR48GREENLETTERS__							60021
#define __PACMANII_NEWTOUR48REDLETTERS__							60022
#define __PACMANII_NEWTOUR48YELLOWLETTERS__							60023

// GUI Systems
// To ask for the name of the player
#define __PACMANII_PLAYERNAMEGUISYSTEM__							50000
#define __PACMANII_PLAYERNAMEOPTIONID__								50002
// GUI System used for the options when playing 
#define __PACMANII_OPTIONWHENPLAYINGGUISYSTEM__						50200
#define __PACMANII_OPTIONSGUISYSTEMMAINWIDGET__						50201
#define __PACMANII_GUISYSTEMSAVEGAMEWDT__							50202
#define __PACMANII_GUISYSTEMEXITGAMEWDT__							50203
#define __PACMANII_GUISYSTEMCONTINUEGAMEWDT__						50204

// Game states types
/** Name of the player playing */
#define __PACMANII_GAMESTATEPLAYERNAMETYPE__						50000
/** Name of the current level */
#define __PACMANII_GAMESTATELEVELTYPE__								50011
/** To create the Playing Game State Control */
#define __PACMANII_GAMESTATECONTROLPLAYINGTYPE__					50080
/** To create the Playing Game State (inside Playing Game State Control) */
#define __PACMANII_GAMESTATEPLAYINGTYPE__							50081
/** To create the Options when Playing Game State */
#define __PACMANII_GAESTATEOPTIONSWHENPLAYTYPE__					50082
/** Special version of the hall of the fame */
#define __PACMANII_GAMESTATEHALLOFFAMETYPE__						50070
/** Special version to control all the game */
#define __PACMANII_GAMESTATECTRLTYPE__								50101
/** To initialize the game, and to avoid doing so at init method in game class. */
#define __PACMANII_GAMESTATEINITIALIZEGAMETYPE__					50102
/** To show the battleship moving in the right side of the screen. */
#define __PACMANII_GAMESTATEMOVINGMONSTERSTYPE__					50103
#define __PACMANII_GAMESTATEMOVMONSTERATTRFADE__					"FADE"
#define __PACMANII_GAMESTATEMOVMONSTERATTRPOSITION__				"POSITION"
#define __PACMANII_GAMESTATEMOVMONSTERATTRITERATIONS__				"ITERATIONS"
#define __PACMANII_GAMESTATEMOVMONSTERATTRBIGPACMAN__				"BIGPACMAN"
/** To blink the map. */
#define __PACMANII_GAMESTATEBLINKMAZEMAPTYPE__						50104
#define __PACMANII_GAMESTATEBLINKMAZEATTRBLINKTIME__				"BLINKINGTIME"
#define __PACMANII_GAMESTATEBLINKMAZEATTRNUMBERBLINKS__				"NUMBERBLINKS"

// Game States
#define __PACMANII_STANDARDCONTROLGAMESTATE__						50000
#define __PACMANII_BEFOREPLAYINGGAMESTATE__							50101
#define __PACMNAII_SHOWINGTHELEVELGAMESTATE__						50102
#define __PACMANII_PLAYINGGAMESTATE__								50107
#define __PACMANII_OPTIONSWHENPLAYINGGAMESTATE__					50200
#define __PACMANII_PLAYERGAMEOVERGAMESTATE__						50022
#define __PACMANII_MENUGAMESTATE__									50009
#define __PACMANII_CONTROLPLAYINGGAMESTATE__						50100
#define __PACMANII_PLAYERDIEDGAMESTATE__							50019

// The entities of the game
// Score Objects
#define __PACMANII_LIVESSCOBJID__									60000
#define __PACMANII_GAMETIMECOUNTERID__								60001
#define __PACMANII_GAMESCOREID__									60002
#define __PACMANII_GAMESCOREFIXTEXTBASEID__							60003
// Characters
#define __PACMANII_NUMBERENTITIES__									10
// Pacman
#define __PACMANII_PACMANBASEENTITYID__								50000
#define __PACMANII_PACMANSTATESTANDLOOKINGRIGHT__					0
#define __PACMANII_PACMANSTATESTANDLOOKINGLEFT__					1
#define __PACMANII_PACMANSTATEEATINGLOOKINGRIGHT__					4
#define __PACMANII_PACMANSTATEEATINGLOOKINGLEFT__					5
#define __PACMANII_BIGPACMANSTATEEATINGLOOKINGRIGHT__				12
#define __PACMANII_BIGPACMANSTATEEATINGLOOKINGLEFT__				13
#define __PACMANII_PACMANSTAYMOVEMENTBASEID__						570000
#define __PACMANII_PACMANEATINGMOVEMENTBASEID__						550000
// Inky
#define __PACMANII_INKYBASEENTITYID__								50010
#define __PACMANII_INKYSTATECHASINGANDLOOKINGRIGHT__				0
#define __PACMANII_INKYSTATECHASINGANDLOOKINGLEFT__					1
#define __PACMANII_INKYSTATERUNNINGAWAY__							4
#define __PACMANII_INKYSTATEATHOMELOOKINGUP__						6
#define __PACMANII_INKYSTATEATHOMELOOKINGDOWN__						7
#define __PACMANII_INKYCHASINGMOVEMENTBASEID__						550010
#define __PACMANII_INKYRUNNINGAWAYMOVEMENTBASEID__					560010
#define __PACMANII_INKYAFTEREATENMOVEMENTBASEID__					570010
#define __PACMANII_INKYATHOMEMOVEMENTBASEID__						580010
// Blinky
#define __PACMANII_BLINKYBASEENTITYID__								50020
#define __PACMANII_BLINKYSTATECHASINGANDLOOKINGRIGHT__				0
#define __PACMANII_BLINKYSTATECHASINGANDLOOKINGLEFT__				1
#define __PACMANII_BLINKYSTATERUNNINGAWAY__							4
#define __PACMANII_BLINKYSTATEATHOMELOOKINGUP__						6
#define __PACMANII_BLINKYSTATEATHOMELOOKINGDOWN__					7
#define __PACMANII_BLINKYCHASINGMOVEMENTBASEID__					550020
#define __PACMANII_BLINKYRUNNINGAWAYMOVEMENTBASEID__				560020
#define __PACMANII_BLINKYAFTEREATENMOVEMENTBASEID__					570020
#define __PACMANII_BLINKYATHOMEMOVEMENTBASEID__						580020
// Pinky
#define __PACMANII_PINKYBASEENTITYID__								50030
#define __PACMANII_PINKYSTATECHASINGANDLOOKINGRIGHT__				0
#define __PACMANII_PINKYSTATECHASINGANDLOOKINGLEFT__				1
#define __PACMANII_PINKYSTATERUNNINGAWAY__							4
#define __PACMANII_PINKYSTATEATHOMELOOKINGUP__						6
#define __PACMANII_PINKYSTATEATHOMELOOKINGDOWN__					7
#define __PACMANII_PINKYCHASINGMOVEMENTBASEID__						550030
#define __PACMANII_PINKYRUNNINGAWAYMOVEMENTBASEID__					560030
#define __PACMANII_PINKYAFTEREATENMOVEMENTBASEID__					570030
#define __PACMANII_PINKYATHOMEMOVEMENTBASEID__						580030
// Clyde
#define __PACMANII_CLYDEBASEENTITYID__								50040
#define __PACMANII_CLYDESTATECHASINGANDLOOKINGRIGHT__				0
#define __PACMANII_CLYDESTATECHASINGANDLOOKINGLEFT__				1
#define __PACMANII_CLYDESTATERUNNINGAWAY__							4
#define __PACMANII_CLYDESTATEATHOMELOOKINGUP__						6
#define __PACMANII_CLYDESTATEATHOMELOOKINGDOWN__					7
#define __PACMANII_CLYDECHASINGMOVEMENTBASEID__						550040
#define __PACMANII_CLYDERUNNINGAWAYMOVEMENTBASEID__					560040
#define __PACMANII_CLYDEAFTEREATENMOVEMENTBASEID__					570040
#define __PACMANII_CLYDEATHOMEMOVEMENTBASEID__						580040

// Events
// Related with the score objects
#define __PACMANII_LIVESACTUALIZED__								60000
#define __PACMANII_LIVESVALUEPARAMETER__							0
#define __PACMANII_GAMETIMEACTUALIZED__								60001
#define __PACMANII_GAMETIMEVALUEPARAMETER__							0
#define __PACMANII_SCOREACTUALIZED__								60002
#define __PACMANII_SCOREVALUEPARAMETER__							0
// Related with the input handler...
#define __PACMANII_MOVINGUPEVENT__									50000
#define __PACMANII_MOVINGRIGHTEVENT__								50001
#define __PACMANII_MOVINGUPRIGHTEVENT__								50002
#define __PACMANII_MOVINGDOWNEVENT__								50003
#define __PACMANII_MOVINGDOWNRIGHTEVENT__							50004
#define __PACMANII_MOVINGLEFTEVENT__								50005
#define __PACMANII_MOVINGUPLEFTEVENT__								50006
#define __PACMANII_MOVINGDOWNLEFTEVENT__							50007
#define __PACMANII_NOMOVINGEVENT__									50008
#define __PACMANII_FIREEVENT__										50009
// Related with the situation of the pacman...
#define __PACMANII_PACMANDESTROYED__								50100
#define __PACMANII_PACMANREACHEDGOAL__								50101
// A ball has been eaten...
#define __PACMANII_BALLEATEN__										50200

// The World and Scenes used for the game
#define __PACMANII_WORLD__											50000
#define __PACMANII_BASICSCENE__										50000
#define __PACMANII_BASICMAP__										50000
// Name of the standard layers every defined map has to content minimum
#define __PACMANII_BACKGROUNDLAYERNAME__							"Background"
#define __PACMANII_LOCATIONSLAYERNAME__								"Locations"
#define __PACMANII_DIRECTIONSLAYERNAME__							"Directions"
#define __PACMANII_MAZELAYERNAME__									"Maze"
// Name of the different forms containing the tiles used to draw
#define __PACMANII_FORMTILESMAZENAME__								"corners"
// The types of tiles in pacman: either a limit, a normal ball or power...
#define __PACMANII_LIMITTILE__										50000
#define __PACMANII_PATH__											50001

#endif
  
// End of the file
/*@}*/
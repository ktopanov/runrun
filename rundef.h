/*

rundef.h - Definitions for runrun

Copyright (C) 1998 Serge A.Kasatkin

This program is free software; you can redistribute it and/or modify it under 
the terms of the GNU General Public License as published by the Free Software 
Foundation; either version 2 of the License, or (at your option) any later 
version. 

This program is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
PARTICULAR PURPOSE. See the GNU General Public License for more details. 

You should have received a copy of the GNU General Public License along with 
this program; if not, write to the Free Software Foundation, Inc., 675 Mass Ave, 
Cambridge, MA 02139, USA. 
*/

#ifndef _DEFINES_

/* --- Start #include section --- */

/* --- system independent includes --- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/timeb.h>

/* --- system dependent --- */

#ifdef __MSDOS__
#include <dos.h>
#include <conio.h>
#include <io.h>

#ifdef __BORLANDC__
#include <values.h>
#else
#include <limits.h>
#endif

#define _COMMANDCOM_
#endif

/* --------------------------- */

#ifdef	__WIN32__
#include <windows.h>
#include <wincon.h>
#include <limits.h>
#include <conio.h>
#include <io.h>
#define _COMMANDCOM_
#pragma warning(disable : 4996)
#pragma comment(lib, "winmm.lib")
#endif

/* --------------------------- */

#ifdef __unix__
#include <ncurses.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#define  NOfilelength	/* copy this if system hasn't filelength function */
#endif

/* --- End #include section --- */

#ifndef INT_MAX
#ifdef MAXINT
#define INT_MAX MAXINT
#else
#error Constant INT_MAX representing maximum integer value should be defined
#endif
#endif

#define		DIGLEN      5			/* char string image length of
									   unsigned short 2-byte integer */

/* keyboard ! SYSTEM DEPENDENT */

#define		ESCAPE      0x1b

#ifndef     __unix__

#define		CHIEF			'\t'			/* chief here button code */

#if defined(__MSDOS__) || defined(__WIN32__)

#define 		KB_RIGHT1		77
#define 		KB_UP1			72
#define 		KB_LEFT1		   75
#define 		KB_DOWN1			80

#define 		KB_RIGHT2		'6'
#define 		KB_UP2			'8'
#define 		KB_LEFT2		   '4'
#define 		KB_DOWN2			'5'

#define 		KB_RIGHT3		'j'
#define 		KB_UP3			'y'
#define 		KB_LEFT3			'g'
#define 		KB_DOWN3			'h'

#define 		KB_RIGHT4		'J'
#define 		KB_UP4			'Y'
#define 		KB_LEFT4			'G'
#define 		KB_DOWN4			'H'

#endif /* masados or win32 */

#else /* __unix__ */

#define		CHIEF			' '			/* chief here button code */

#ifdef __NCURSES_H

#define 		KB_RIGHT1		KEY_RIGHT
#define 		KB_UP1			KEY_UP
#define 		KB_LEFT1		   KEY_LEFT
#define 		KB_DOWN1			KEY_DOWN

#else /* __NCURSES_H */

#define 		KB_RIGHT1		77
#define 		KB_UP1			72
#define 		KB_LEFT1		   75
#define 		KB_DOWN1			80

#endif /* __NCURSES_H */

#define 		KB_RIGHT2		'6'
#define 		KB_UP2			'8'
#define 		KB_LEFT2		   '4'
#define 		KB_DOWN2			'5'

#define 		KB_RIGHT3		'l'
#define 		KB_UP3			'i'
#define 		KB_LEFT3			'j'
#define 		KB_DOWN3			'k'

#define 		KB_RIGHT4		'L'
#define 		KB_UP4			'I'
#define 		KB_LEFT4			'J'
#define 		KB_DOWN4			'K'

#endif /* __unix__ */

#define		YESU			((uchar)'Y')
#define		YESl			((uchar)'y')
#define		NOU			((uchar)'N')
#define		NOl			((uchar)'n')

#define		RECDMOU		((uchar)'R')
#define		RECDMOl		((uchar)'r')
#define		RUNDMOU		((uchar)'D')
#define		RUNDMOl		((uchar)'d')

/* Cat definitions */
#define     NORMCAT     "=0="      	/* normal cat */
#define     AFRCAT		"+++"			/* afraid cat */
#define     SKELET      "---"      	/* skeleton (eaten cat) */
#define		NCR1			NORMCAT     /* normal cat to right step 1 */
#define		NCR2			NORMCAT		/* normal cat to right step 2 */
#define		NCL1			NORMCAT		/* normal cat to left step 1 */
#define		NCL2			NORMCAT		/* normal cat to left step 1 */
#define		ACR1			">-<"			/* afraid cat to right step 1 */
#define		ACR2			"<->"			/* afraid cat to right step 2 */
#define		ACL1			">-<"			/* afraid cat to left step 1 */
#define		ACL2			"<->"			/* afraid cat to left step 1 */
#define		SCR1			SKELET		/* skeleton to right step 1 */
#define		SCR2			SKELET		/* skeleton cat to right step 2 */
#define		SCL1			SKELET		/* skeleton cat to left step 1 */
#define		SCL2			SKELET		/* skeleton cat to left step 1 */
#define		NORMAL		0
#define		AFRAID		1
#define		EATEN			2
#define		AFRTIME1		180
#define		AFRTIME2		110
#define		REDTIME		3000
#define		NOAFR			2250
#define		MAXNONAFR	3				/* maximum supercats number */
#define		SKIPST		4
#define     BRTHROGH    1

/* Mouse definitions */
#define		MFORM1      ">X<"
#define		MFORM2      "<X>"
#define 		ML1			MFORM1		/* mouse form 1 */
#define 		ML2			MFORM2		/* mouse form 2 */
#define		MR1			ML1		/* to left same as to right */
#define		MR2			ML2
#define 		_MFORM1		0
#define		_MFORM2		1
#define     AGONIA      "***"
#define		AGDLY		200
#define		AGCNT		4

/* Maze definitions */
#define 		VSIZE		24
#define		HSIZE		79
#define		PILL		((uchar)'@')
#define		DOT		((uchar)'.')
#define		SPACE		((uchar)' ')
#define     TRY      ((uchar)'X')
#define 		MAXMAZES	200			/* you need more?! */
#define		MINMAZES	4
#define		MAXPILLS	4
#define     MAXCATS     4			/* try 6 */
#define		CATBON		20
#define		SEQSTRT		3  		/* number of games when starting mazes
											will be given sequentaly (skill
                                 levels) */
#define		DARKTRY		6			/* number of retryes to recieve
									   	dark maze */
#define		DOTWEIGHT	1

/* Bonus definitions */
#define 		BONUS		"***"			/* bonus plase indicator */
#define		SCORE		"SCORE"		/* score plase indicator */
#define		BON1		"1*5"
#define		BON2		"3*0"
#define		BON3		"4*5"
#define		BON4		"6*0"
#define		BON5		"7*5"
#define		BON6		"9*0"
#define		MAXBONUS		5
#define		NOBONUS		1000
#define		ISBONUS     200
#define		SPACES		"   "
#define		BONCNT		2
#define		BONWEIGHT	15

/* Directions */
#define		RIGHT		0
#define		UP			1
#define		LEFT		2
#define		DOWN		3

/* Other */

#if !defined(TRUE)
#define		TRUE		1			/* O'k */
#endif
#if !defined(FALSE)
#define		FALSE		0
#endif

#define 		RDTXT		"rt"		/* mode for mazes file */

#define		CR			'\r'
#define		LF			'\n'
#define		BS			'\b'

#define		EOL		CR			/* END OF LINE IN TEXT FILE -
											WARNING !!! */

#define		TRYBINC		500	/* score increment for new mouse */
#define		DELAY			80		/* middle drawing delay */
#define		GDELAY		12		/* delay between steps msec */
#define		NORM_GAME_TIME		64 /* msecs per game stroke */
#define		DISPLAY_MAZE_TIME	10	/* msecs per each line of maze */
#define		DLYINTRVL	1		/* interval to change by + - */
#define		NAMLEN		16		/* players name length */
#define		TOPCAP		20		/* top 20 capacity */
#define		TOPHSH		2
#define		TOPWR		"wb"		/* mode for players file write */
#define		TOPRD		"rb"		/* -#- read */
#define		ZEROBYTE	'\0'		/* IT'S A ZERO!!! */
#define		CATSHOME	'H'		/* point of cat's acceptuation */
#define		SHITSCORE	2250

#define		GAMEBEG		0		/* game modes when chief gone */
#define		GAMEGO		1
#define 		GAMEEDT		2

#define 		STRETRY		3		/* starting mouses */
#define     GEN1        0
#define     GEN2        8

/* colors */

#ifdef		__MSDOS__

#ifdef		__BORLANDC__

#define		TXCOLOR     LIGHTGREEN
#define		MSCOLOR		LIGHTGREEN
#define		CTCOLOR		LIGHTGREEN
#define		AFCOLOR		LIGHTGREEN
#define     ETCOLOR		LIGHTGREEN
#define		BGCOLOR		BLACK
#define     PLCOLOR     LIGHTGREEN
#define		PNTCOLOR		LIGHTGREEN

#else       /* !__BORLANDC__*/

#define BLACK           0x0
#define LIGHTGREEN      0xa

#define     TXCOLOR     LIGHTGREEN
#define		MSCOLOR		LIGHTGREEN
#define		CTCOLOR		LIGHTGREEN
#define		AFCOLOR		LIGHTGREEN
#define     ETCOLOR		LIGHTGREEN
#define		BGCOLOR		BLACK
#define     PLCOLOR     LIGHTGREEN
#define     PNTCOLOR    LIGHTGREEN

#endif		/* __BORLANDC__ */

#endif		/* __MSDOS__ */

#ifdef		__WIN32__

#define		TXCOLOR     (FOREGROUND_GREEN|FOREGROUND_INTENSITY)
#define		MSCOLOR		(FOREGROUND_GREEN|FOREGROUND_INTENSITY)
#define		CTCOLOR		(FOREGROUND_GREEN|FOREGROUND_INTENSITY)
#define		AFCOLOR		(FOREGROUND_GREEN|FOREGROUND_INTENSITY)
#define     ETCOLOR		(FOREGROUND_GREEN|FOREGROUND_INTENSITY)
#define		BGCOLOR		(FOREGROUND_GREEN|FOREGROUND_INTENSITY)
#define     PLCOLOR     (FOREGROUND_GREEN|FOREGROUND_INTENSITY)
#define		PNTCOLOR		(FOREGROUND_GREEN|FOREGROUND_INTENSITY)

#endif		/* __WIN32__ */

#ifdef		__unix__

#define		TXCOLOR     0
#define		MSCOLOR		0
#define		CTCOLOR		0
#define		AFCOLOR		0
#define     ETCOLOR		0
#define		BGCOLOR		0
#define     PLCOLOR     0
#define		PNTCOLOR		0

#endif		/* __unix__ */

#define     DMOFNAME    "RUNDEMO.DAT"

#undef		_ISDEMO_

#define _DEFINES_
#endif


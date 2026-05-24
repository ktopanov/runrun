/*
runtyp.h - type defenitions for runrun

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
#include "rundef.h"
#endif

typedef	unsigned short int usint;
typedef  unsigned char uchar;

typedef struct {
	uchar vert;				/* vertical coord */
	uchar horz;				/* horizontal coord */
	uchar dir;				/* direction */
	uchar olddir;			/* old direction */
} BEAST;

typedef struct {
	BEAST body;
	uchar mode;				/* cat mode: NORMAL, AFRAID, EATEN */
	usint count;
	usint time;
   usint hungry;                    /* level of hungriment */
	unsigned int looking: 1;
	unsigned int avlb: 1;
	unsigned int shiting: 1;
} CAT;

typedef struct {
	BEAST body;
	unsigned int form: 1;	/* mouse form: 0 - MFORM1, 1 - MFORM2 */
} MOUSE;

typedef struct {
	uchar **lines;
	usint dotcnt;
	uchar svp;				/* score count v pos */
	uchar shp;               /* score count h pos */
	uchar bvp;				/* bonus v pos */
	uchar bhp;				/* bonus h pos */
	uchar hvp;    			/* cat home v pos */
	uchar hhp;               /* cat home h pos */
	uchar mvs;               /* mouse vertical start position */
	uchar mhs;               /*       horizontal              */
	uchar cvs[ MAXCATS ];    /* cats vertical                 */
	uchar chs[ MAXCATS ];    /*     horizontal                */
} MAZE;

typedef uchar dtmap[ VSIZE ][ HSIZE / 8 + 1 ];
							/* bit map for pos-s with dots and pills in maze */

typedef struct {
	uchar name[ NAMLEN ];	/* player's name */
	usint score;		/* player's result */
} TOP;

typedef struct {
	unsigned int number: 2;
} FVAL;


/*
envext.h - definitions of functions in runrun

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

extern void video_init( void );
extern uchar get_key( void );
extern int is_key( void );
extern void clear_screen( void );
extern void cursor( uchar, uchar );
extern void on_cursor( void );
extern void off_cursor( void );
extern void displch( uchar, uchar );
extern void displst( uchar * );
extern void rndinit( void );
extern usint rndvalue( usint );
extern uchar vcursor, hcursor;
extern int dir_key( uchar );
extern int get_dir( uchar );
extern void load_ch( void );
extern void mode3( void );
extern void specstart( void );
extern usint darkmaze;
extern void bye_user( int );
extern unsigned long curr_time( void );
extern void time_delay( usint );
extern unsigned long start_time( void );

#ifdef NOfilelength
extern long filelength(int fd);
#endif



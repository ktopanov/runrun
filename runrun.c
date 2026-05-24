/*
runrun.c - startup module for runrun

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
#include "runrun.h"

usint maznumb;               /* number of mazes */
MAZE cmaze;                 /* current maze */
usint cmpnts;				/* number of points left in current
									   maze */
uchar retryes;				/* number of mouses left (will be
									initialized) */
uchar *mazbuff;
uchar *mazes[ MAXMAZES ];
usint mazcnt;
usint gamecnt;
usint score;
static TOP *top20;
static uchar topfile[] = { "RUNRUN.TOP" };
int gamestat;
int ismouse;
extern int notuse;
extern int play_game( void  );
extern usint boncnt, bonus, trybon, boncycl;
usint keystrokes;
usint nmaze;
FILE *demofile;
char * mazesfile = { "RUNRUN.MAZ" };
usint mazestrlen;

int cmp_mazset( uchar *str, uchar *line )
{
usint count;

	for( count = 0; str[ count ] != 0; ++count )
		{
		if( line[ count ] != str[ count ] )
			return FALSE;
		}
	return TRUE;
}

void clr_mazset( uchar *line, usint len )
{
usint count;

	for( count = 0; count != len; ++count )
		line[ count ] = SPACE;
}

usint load_mazes( int argc, char * argv[] )
{
FILE *in;
usint count;
unsigned long flen;
uchar *scaner;

	fputs( "Initializing...\n", stderr );

   if( argc > 1 )
   	mazesfile = argv[ 1 ];

	if( ( in = fopen( mazesfile, "rb" ) ) == NULL )
		{
		perror( "Maze's file error" );
		bye_user( 1 );
		}
	if( ( flen = filelength( fileno( in ) ) ) > 0xffff - 8 )
		{
		fputs( "Too big maze's file\n", stderr );
		bye_user( 1 );
		}
	if( ( mazbuff = scaner = malloc( ( usint ) flen ) ) == NULL )
		{
		fputs( "No memory\n", stderr );
		bye_user( 1 );
		}
	if( ! fread( mazbuff, ( usint ) flen, 1, in ) )
		{
		perror( "Mazes read error" );
		bye_user( 1 );
		}
	fclose( in );

	for( count = 0; count != MAXMAZES; ++count )
		{
		usint vcnt;

		mazes[ count ] = scaner;
		for( vcnt = 0; vcnt != VSIZE; ++vcnt )
			{
         uchar *_scaner = scaner;

         while( *scaner >= SPACE )
         	++scaner;
         while( *scaner < SPACE )
         	{
            *( scaner++ ) = ( uchar ) 0;
            if( scaner == mazbuff + ( usint ) flen )
               {
               if( ++count < MINMAZES )
                  {
                  fputs( "Minimum number of mazes == 4", stderr );
                  bye_user( 1 );
                  }
               return count;
               }
            }
         mazestrlen = ( usint )( scaner - _scaner );
         }
		}

	return count;
}

void clrcmaze( void )
{
usint count;

	for( count = 0; count != VSIZE; ++count )
		memset( cmaze.lines[ count ], SPACE, HSIZE );
}

void inicmaze( void )
{
usint count;

	if( ( cmaze.lines = calloc( VSIZE, sizeof( uchar * ) ) ) == NULL )
		{
		fputs( "No memory\n", stderr );
		bye_user( 1 );
		}
	for( count = 0; count != VSIZE; ++count )
		{
		if( ( cmaze.lines[ count ] = malloc( HSIZE + 1 + 1 ) ) == NULL )
			{
			fputs( "No memory\n", stderr );
			bye_user( 1 );
			}
		cmaze.lines[ count ][ HSIZE ] =
      cmaze.lines[ count ][ HSIZE + 1 ] = ( uchar ) 0;
		}
}

int was_dot( usint vpos, usint hpos )
{
uchar val;

	return( ( val = *( mazes[ nmaze ] + ( vpos * mazestrlen ) + hpos ) ) == DOT
              || val == PILL );
}

void select_maze( void )
{
usint count;
usint pills = 0;
usint cats = 0;
int mouse = FALSE, home = FALSE, bonus = FALSE;
usint offs;
uchar *ptr;

   darkmaze = FALSE;	/* First selected maze isn't dark */
	ptr = ( gamecnt < SEQSTRT ) ? mazes[ nmaze = gamecnt++ ]
								: mazes[ nmaze = rndvalue( maznumb ) ];
	cmaze.dotcnt = 0;
	clrcmaze();
	++mazcnt;
	for( count = 0; count != VSIZE; ++count )
		{
		for( offs = 0; *ptr != ZEROBYTE; ++ptr, ++offs )
			{
			if( *ptr == DOT )
				{
				++cmaze.dotcnt;
				cmaze.lines[ count ][ offs ] = DOT;
				continue;
				}
			if( *ptr == PILL )
				{
				if( pills < MAXPILLS )
					{
					++pills;
					cmaze.lines[ count ][ offs ] = PILL;
					}
					else
						cmaze.lines[ count ][ offs ] = DOT;
				++cmaze.dotcnt;
				continue;
				}
			if( cmp_mazset( NORMCAT, ptr ) )
				{
				if( cats < MAXCATS )
					{
					cmaze.cvs[ cats ] = count;
					cmaze.chs[ cats++ ] = offs + 1;
					}
				ptr += sizeof( NORMCAT ) - 2;
				offs += sizeof( NORMCAT ) - 2;
				continue;
				}
			if( cmp_mazset( MFORM1, ptr ) )
				{
				if( ! mouse )
					{
					cmaze.mvs = count;
					cmaze.mhs = offs + 1;
					mouse = TRUE;
					}
				ptr += sizeof( MFORM1 ) - 2;
				offs += sizeof( MFORM1 ) - 2;
				continue;
				}
			if( *ptr == CATSHOME )
				{
				cmaze.hvp = count;
				cmaze.hhp = offs;
				home = TRUE;
				continue;
				}
			if( cmp_mazset( BONUS, ptr ) )
				{
				cmaze.bvp = count;
				cmaze.bhp = offs + 1;
				bonus = TRUE;
				ptr += sizeof( BONUS ) - 2;
				offs += sizeof( BONUS ) - 2;
				continue;
				}
			if( cmp_mazset( SCORE, ptr ) )
				{
				cmaze.svp = count;
				cmaze.shp = offs;
				ptr += sizeof( SCORE ) - 2;
				offs += sizeof( SCORE ) - 2;
				continue;
				}
			cmaze.lines[ count ][ offs ] = *ptr;
			}
      cmaze.lines[ count ][ offs ] = SPACE;
		while( *ptr <= ZEROBYTE )
      	++ ptr;
		}
	if( ! cats )
		{
		cmaze.cvs[ 0 ] = VSIZE / 2;
		cmaze.cvs[ 0 ] = HSIZE / 2;
		}
	if( cats < MAXCATS )
		{
		for( count = cats; count < MAXCATS; ++count )
			{
			cmaze.cvs[ count ] = cmaze.cvs[ 0 ];
			cmaze.chs[ count ] = cmaze.chs[ 0 ];
			}
		}
	if( ! mouse )
		{
		cmaze.mvs = VSIZE / 2 + 4;
		cmaze.mhs = HSIZE / 2;
		}
	if( ! home )
		{
		cmaze.hvp = cmaze.cvs[ 0 ];
		cmaze.hhp = cmaze.chs[ 0 ];
		}
	if( ! bonus )
		{
		cmaze.bvp = cmaze.hvp;
		cmaze.bhp = cmaze.hhp;
		}
}

void rdrwrt( void )
{
usint count;

	for( count = 0; count != retryes - 1; ++count )
		{
		cursor( count, HSIZE );
		displch( TRY, MSCOLOR );
      cmaze.lines[ count ][ HSIZE ] = TRY;
      if( count == VSIZE - 1 )
      	{
         usint index;

			for( index = 0; index != VSIZE; ++index )
				{
				cursor( index, 0 );
				displst( cmaze.lines[ index + ( index != VSIZE - 1 ) ] );
				}
         }
		}
	for( ; count != VSIZE; ++count )
		{
		cursor( count, HSIZE );
		displch( SPACE, MSCOLOR );
      cmaze.lines[ count ][ HSIZE ] = SPACE;
		}
}

void redraw_maze( usint can_dark )
{
usint count;
uchar buffer[ 100 ];

	if( can_dark && retryes == DARKTRY && rndvalue( 5 ) )
   	darkmaze = TRUE;
	for( count = 0; count != VSIZE; ++count )
		{
      unsigned long start = start_time(), value;

		cursor( count, 0 );
		displst( cmaze.lines[ count ] );

      if( ( value = curr_time() - start ) < DISPLAY_MAZE_TIME )
      	time_delay( DISPLAY_MAZE_TIME - ( usint )value );

		}
   darkmaze = FALSE;
	sprintf( (char*) buffer, "%s%u", "MAZE ", mazcnt );
	cursor( count, 0 );
	displst( buffer );
   cursor( cmaze.svp, cmaze.shp );
   displst( "     " );
	rdrwrt();
}

void shtop20( void )
{
usint count, index;
uchar buffer[ 2 + 2 + NAMLEN + 2 + DIGLEN + 1 ];
static uchar format[] = { "%-2d. %s  %-5u" };
static uchar *copywr[] = {
	"ORIGINAL - PDP-11, AUTHOR UNKNOWN",
	"SYSTEM INDEPENDED RELEASE PROGRAMED BY",
   "SERGE A.KASATKIN & FRIENDS 1992, 1997",
   "(to view license read GPL.TXT)",
   "RUSSIA ROSTOV-ON-DON chief@teletext.rnd.su",
   "FREEWARE GAMMA 3.62 (COPYWRONG :-)"
};

	cursor( 1, TOPHSH );
	displst( (uchar*) "-----TOP 20 (NOT MTV)-----" );
	for( count = 0; count != TOPCAP; ++count )
		{
		sprintf( (char*) buffer, (char*) format, count + 1, top20[ count ].name,
										 top20[ count ].score );
		cursor( count + 2, TOPHSH );
		displst( buffer );
		}
	cursor( count + 2, TOPHSH );
	displst( (uchar*) "---------OUTSIDER---------" );
	sprintf( (char*) buffer, (char*) format, count + 1, top20[ count ].name,
									 top20[ count ].score );
	cursor( count + 3, TOPHSH );
   for( count = 0; count != sizeof( copywr )/sizeof( copywr[0] ); ++count )
   	{
      cursor( count + VSIZE - sizeof( copywr )/sizeof( copywr[0] ) -1,
      		  (HSIZE / 2) - 3 );
      displst( copywr[ count ] );
      }
}

int agree( void )
{
uchar key = 0;

	gamestat = GAMEBEG;
	on_cursor();
	cursor( 12, 37 );
	displst( "BEGIN ?" );

	while( TRUE )
		{
		if( key != RUNDMOU &&
      	( ( key = get_key() ) == YESU || key == YESl ) )
			{
			off_cursor();
			break;
			}
		if( key == NOU || key == NOl )
			return FALSE;
		}
	return TRUE;
}

void save_top20( void )
{
FILE *out;

	if( ( out = fopen( topfile, TOPWR ) ) == NULL )
		return;

	fwrite( top20, sizeof( TOP ), TOPCAP, out );
	fclose( out );
}

void edit_name( uchar *name, uchar vpos )
{
usint offs;
uchar key;

	gamestat = GAMEEDT;
	while( TRUE )
		{
		cursor( vpos, TOPHSH + 4 );
		displst( name );

		for( offs = 0; name[ offs ] != SPACE &&
					   name[ offs ] != ( uchar ) 0; ++offs )
			;
		cursor( vpos, TOPHSH + 4 + offs );
		on_cursor();
      while( TRUE )
         {
         switch( key = get_key() )
            {
            case CR:				/* carriage return */
               return;
            case BS:				/* backspace */
               if( offs )
                  name[ --offs ] = SPACE;
               break;
            default:
               if( name[ offs ] == SPACE )
                  name[ offs++ ] = key;
            }
         break;
         }
		off_cursor();
		}
}

void top_20( void )
{
usint count;

   for( count = 0; count != TOPCAP; ++count )
      if( top20[ count ].score <= score )
         {
         usint index;

         for( index = TOPCAP; index >= count + 1; --index )
            top20[ index ] = top20[ index - 1 ];

         for( index = 0; index != NAMLEN - 1; ++index )
            top20[ count ].name[ index ] = SPACE;
         top20[ count ].name[ index ] = ( uchar ) 0;
         top20[ count ].score = score;

         shtop20();

         edit_name( top20[ count ].name, count + 2 );

         save_top20();

         return;
         }
	strcpy( (char*) top20[ count ].name, "YOU            " );
	top20[ count ].score = score;
	shtop20();
}

int load_top20( void )
{
FILE *in;
usint count, index;

	if( ( top20 = calloc( TOPCAP + 1, sizeof( TOP ) ) ) == NULL )
		{
		fputs( "No memory\n", stderr );
		bye_user( 1 );
		}
	for( count = 0; count <= TOPCAP; ++count )
		{
		for( index = 0; index != NAMLEN - 1; ++index )
			top20[ count ].name[ index ] = SPACE;
		top20[ count ].name[ index ] = ( uchar ) 0;
		}
	if( ( in = fopen( (char*) topfile, TOPRD ) ) != NULL )
		{
      static usint num;
		if( (num = fread( top20, sizeof( TOP ), TOPCAP, in )) != TOPCAP )
			{
			fclose( in );
			return FALSE;
			}
		fclose( in );
		}
	return TRUE;
}

void hello_user( void )
{
usint count, index;
static uchar m0[] = { "FROM AUTHOR:" };
static uchar *m1[] = {
   "            I KNOW              ",
	"YOU EXPECTED THIS GAME SO LONG",
	"           AND NOW...            ",
	"Y-A-H-O-O-O-O-O-O-O-O-O-O-O-O !!!"
};
static usint dly[] = {
	100, 1100, 100, 0
};
	off_cursor();
	cursor( VSIZE / 2, 9 );
	displst( m0 );
	for( count = 0; count != sizeof( m1 ) / sizeof( uchar * ); ++count )
		{
		cursor( VSIZE / 2, 10 + sizeof( m0 ) );
		for( index = 0; m1[ count ][ index ] != ( uchar ) 0; ++index )
			{
			displch( m1[ count ][ index ], TXCOLOR );
			time_delay( 40 );
			if( is_key() )
				{
				on_cursor();
				return;
				}
			}
		if( is_key() )
			{
			on_cursor();
			return;
			}
		time_delay( dly[ count ] );
		}
	on_cursor();
}

void main( int argc, char * argv[] )
{
	specstart();		/* specific actions during startup */

	if( ( maznumb = load_mazes( argc, argv ) ) == NULL )
		bye_user( 1 );

	inicmaze();

	load_top20();

	rndinit();
	video_init();
	clear_screen();
	hello_user();

	score = 0;
	clear_screen();
	shtop20();

	while( trybon = TRYBINC, retryes = STRETRY, gamecnt = 0, agree() ) /* :-) */
		{
		boncnt = BONCNT;
      bonus = 0;
      boncycl = NOBONUS;

		do
			{
			int notplayed;

			while( is_key() )
				get_key();

			select_maze();

			do
				{
				off_cursor();
				redraw_maze( TRUE );
				notplayed = play_game();	/* TRUE if has been eaten */
				}
			while( ( retryes -= notplayed ) != FALSE && notplayed );
			}
		while( retryes );
/*
		mode3();
*/
		clear_screen();
		top_20();
		score = 0;
		mazcnt = 0;
		}
	mode3();
	bye_user( 0 );
}


/*
mgff.c - Main runrun module

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

#define maze(v,h)       (cmaze.lines[(v)][(h)])
#define I_was(v,h,val)	maze(v,h)=val

#define cincv(vc)       (((vc)==(VSIZE-1))?0:(vc+1))
#define cinch(hc)       (((hc)==(HSIZE))?0:(hc+1))
#define cdecv(vc)       ((vc)?(vc-1):(VSIZE-1))
#define cdech(hc)       ((hc)?(hc-1):(HSIZE))
#define vinch(hc,vc)	(((hc)==(HSIZE))?(cincv(vc)):(vc))
#define vdech(hc,vc)	(((hc)!=0)?(vc):(cdecv(vc)))

extern MAZE cmaze;
extern usint score;
extern uchar retryes;
extern void rdrwrt( void );
extern int gamestat;
extern int was_dot( usint, usint );
extern usint mazestrlen;

extern usint keystrokes;
static uchar *bonuses[] = { BON1, BON2, BON3, BON4, BON5, BON6 };
usint trybon = TRYBINC;
static MOUSE mouse;
static CAT cat[ MAXCATS ];
static uchar bdirs[] = { LEFT, DOWN, RIGHT, UP };
static uchar *catbd1[2][3] = {
{ NCR1, ACR1, SCR1 },
{ NCL1, ACL1, SCL1 }
};
static uchar *catbd2[2][3] = {
{ NCR2, ACR2, SCR2 },
{ NCL2, ACL2, SCL2 }
};
usint bonus;
usint boncnt;
usint boncycl;

static int isbonus = FALSE;

uchar whathere( uchar v, uchar h, uchar dir )
{
	switch( dir )
		{
		case RIGHT:
			return maze( vinch( h, v ), cinch( h ) );
		case UP:
			return maze( cdecv( v ), h );
		case LEFT:
			return maze( vdech( h, v ), cdech( h ) );
		case DOWN:
			return maze( cincv( v ), h );
		}
	return NULL;	/* never */
}

int available( BEAST *body, uchar dir )
{
uchar val[ 3 ];
usint count;
uchar v, h;

	v = body->vert;
	h = body->horz;

	if( dir == RIGHT || dir == LEFT )
		{
		if( dir == RIGHT )
			{
			return ( val[ 0 ] = whathere( vinch( h, v ), cinch( h ), dir ) )
						  == SPACE || val[ 0 ] == DOT || val[ 0 ] == PILL;
			}
		return ( val[ 0 ] = whathere( vdech( h, v ), cdech( h ), dir ) )
						  == SPACE || val[ 0 ] == DOT || val[ 0 ] == PILL;
		}
	val[ 0 ] = whathere( vdech( h, v ), cdech( h ), dir );
	val[ 1 ] = whathere( v, h, dir );
	val[ 2 ] = whathere( vinch( h, v ), cinch( h ), dir );
	for( count = 0; count != 3; ++count )
		{
		if( val[ count ] > SPACE &&
			val[ count ] != DOT &&
			val[ count ] != PILL )
			return FALSE;
		}
	return TRUE;
}

void showbody( BEAST *who, uchar *string, uchar color )
{
usint count;

	cursor( vdech( who->horz , who->vert ), cdech( who->horz ) );
	for( count = 0; count != 3; ++count )
		displch( string[ count ], color );
}

void step( BEAST *_body, int dir )
{
register BEAST *body;
uchar v, h;
uchar c;

	body = _body;
	v = body->vert;
	h = body->horz;
	switch( dir )
		{
		case RIGHT:
			{
			uchar dv, dh;

			cursor( dv = vdech( h, v ), dh = cdech( h ) );
			c = maze( dv, dh );
			displch( c, c == PILL ? PLCOLOR : TXCOLOR );
			body->horz = cinch( h );
			body->vert = vinch( h, v );
			break;
			}
		case UP:
			cursor( vdech( h, v ), cdech( h ) );
			displch( SPACE, TXCOLOR );
			c = maze( v, h );
			displch( c, c == PILL ? PLCOLOR : TXCOLOR );
			c = maze( vinch( h, v ), cinch( h ) );
			displch( SPACE, TXCOLOR );
			body->vert = cdecv( v );
			break;
		case LEFT:
			{
			uchar iv, ih;

			cursor( iv = vinch( h, v ), ih = cinch( h ) );
			c = maze( iv, ih );
			displch( c, c == PILL ? PLCOLOR : TXCOLOR );
			body->horz = cdech( h );
			body->vert = vdech( h, v );
			break;
			}
		case DOWN:
			cursor( vdech( h, v ), cdech( h ) );
			c = maze( vdech( h, v ), cdech( h ) );
			displch( SPACE, TXCOLOR );
			c = maze( v, h );
			displch( c, c == PILL ? PLCOLOR : TXCOLOR );
			c = maze( vinch( h, v ), cinch( h ) );
			displch( SPACE, TXCOLOR );
			body->vert = cincv( v );
			break;
		}
}

void shcount( usint count )
{
uchar buffer[ DIGLEN + 1 ];

	sprintf( (char*)buffer, "%5u", count );
	cursor( cmaze.svp, cmaze.shp );
	displst( buffer );
}

void shall( void )
{
usint count;

	showbody( &mouse.body, ML1, MSCOLOR );
	for( count = 0; count != MAXCATS; ++count )
		showbody( &cat[ count ].body, catbd1[ cat[ count ].body.dir % 2 ]
										   [ cat[ count ].mode ], CTCOLOR );
}

void agonia( BEAST *body )
{
usint count;

	for( count = 0; count != AGCNT; ++count )
		{
		showbody( body, AGONIA, ETCOLOR );
		time_delay( AGDLY );

      if( is_key() )
			get_key();

		showbody( body, SKELET, ETCOLOR );
		time_delay( AGDLY );

		if( is_key() )
			get_key();

		}
	shall();
   while( is_key() )
   	get_key();
}

int _play_game( void )
{
uchar key;
int first = TRUE;
usint count;
int vd, hd;	/* distantions */
usint catseat = 0;
uchar xp;
static usint dly = GDELAY;
usint cycle = 0;
FVAL val;
static usint sttime[] = {
	110, 2341, 678, 7
	};
uchar headv, headh;
usint shitings = 0;
usint catsdelay = AFRTIME1;

	gamestat = GAMEGO;
	mouse.body.vert = cmaze.mvs;
	mouse.body.horz = cmaze.mhs;
	mouse.body.dir = mouse.body.olddir = RIGHT;
	mouse.form = _MFORM1;
	showbody( &mouse.body, ML1, MSCOLOR );

	for( count = 0; count != MAXCATS; ++count )
		{
		cat[ count ].body.vert = cmaze.cvs[ count ];
		cat[ count ].body.horz = cmaze.chs[ count ];
		cat[ count ].mode = NORMAL;
		cat[ count ].count = 0;
		cat[ count ].avlb =
		cat[ count ].looking =
		cat[ count ].shiting = FALSE;
		cat[ count ].time = sttime[ count ];
		showbody( &cat[ count ].body, NCL1, CTCOLOR );
		}

	shcount( score );

	for( ; TRUE; ++cycle )
		{
      /* ---- time synchro ---- */

      unsigned long value, st_time = start_time();

		/* -------------------- BONUSES --------------------- */
      if( ! --boncycl )
      	{
         if( isbonus )
         	{
            isbonus = FALSE;
            boncycl = NOBONUS;
				cursor( cmaze.bvp, cmaze.bhp - 1 );
				displst( SPACES );
            catsdelay -= MAXBONUS * 5;
            --boncnt;
            }
            else
            	{
					isbonus = TRUE;
               boncycl = ISBONUS;
               }
         }
      if( ! boncnt )
      	{
         if( bonus < MAXBONUS )
      		++bonus;
         boncnt = BONCNT;
         }
		if( isbonus )
      	{
         cursor( cmaze.bvp, cmaze.bhp - 1 );
         displst( bonuses[ bonus ] );
         }

		/* --------------------- CATS ----------------------- */

		for( count = 0; count != MAXCATS; ++count )
			{
			CAT _cat;
			usint dir, cnt, _dir[ 2 ], old, back, avl[ 4 ];
			usint isway = FALSE;
			int gohome = FALSE;

			_cat = cat[ count ];
			++( _cat.time );
			old = _cat.body.dir;

			if( _cat.mode == AFRAID && score > SHITSCORE )
				{
				if( ! _cat.shiting )
					if( ( score / (SHITSCORE) ) > shitings &&
               	 ! rndvalue( 15 ) )
						{
						++shitings;
						_cat.shiting = TRUE;
						}
				}
				else
					_cat.shiting = FALSE;

			if( _cat.body.vert == mouse.body.vert &&
				_cat.body.horz == mouse.body.horz )
				{
				if( _cat.mode == NORMAL )
					{
					agonia( &mouse.body );
					keystrokes = cycle + 2;
					return TRUE;
					}
				if( _cat.mode == AFRAID )
					{
					_cat.mode = EATEN;
					shcount( score += ++catseat * CATBON );
					if( score >= trybon )
						{
						++retryes;
						rdrwrt();
						trybon += TRYBINC;
						}
					}
				}

			back = bdirs[ _cat.body.dir ];

			switch( _cat.mode )
				{
				case NORMAL:
					if( _cat.looking )
						{
						vd = mouse.body.vert - _cat.body.vert;
						hd = mouse.body.horz - _cat.body.horz;
						}
						else
							{
							vd = cmaze.hvp - _cat.body.vert;
							hd = cmaze.hhp - _cat.body.horz;
							}
					break;
				case AFRAID:
					vd = _cat.body.vert - mouse.body.vert;
					hd = _cat.body.horz - mouse.body.horz;
					break;
				case EATEN:
					if( ! _cat.looking )
						{
						vd = cmaze.cvs[ count ] - _cat.body.vert;
						hd = cmaze.chs[ count ] - _cat.body.horz;
						break;
						}
					vd = cmaze.hvp - _cat.body.vert;
					hd = cmaze.hhp - _cat.body.horz;
					gohome = TRUE;
					break;
				}
			if( vd > 0 )
				{
				if( hd > 0 )
					if( vd > hd )
						{
						_dir[ 0 ] = DOWN;
						_dir[ 1 ] = RIGHT;
						}
						else
							{
							_dir[ 0 ] = RIGHT;
							_dir[ 1 ] = DOWN;
							}
					else
						if( vd > 0 - hd )
							{
							_dir[ 0 ] = DOWN;
							_dir[ 1 ] = LEFT;
							}
							else
								{
								_dir[ 0 ] = LEFT;
								_dir[ 1 ] = DOWN;
								}
				}
				else
					{
					if( hd > 0 )
						if( 0 - vd > hd )
							{
							_dir[ 0 ] = UP;
							_dir[ 1 ] = RIGHT;
							}
							else
								{
								_dir[ 0 ] = RIGHT;
								_dir[ 1 ] = UP;
								}
						else
							if( vd < hd )
								{
								_dir[ 0 ] = UP;
								_dir[ 1 ] = LEFT;
								}
								else
									{
									_dir[ 0 ] = LEFT;
									_dir[ 1 ] = UP;
									}
					}
			if( _cat.looking )
				{
				for( cnt = 0; cnt != 4; ++cnt )
					{
					avl[ cnt ] = FALSE;
					if( cnt != back ||
						( ! rndvalue( 200 ) && score > 2000
										   && _cat.mode != EATEN ) ||
						_cat.avlb )
						{
						if( available( &_cat.body, cnt ) )
							{
							if( cnt != old )
								dir = cnt;
							avl[ cnt ] = TRUE;
							isway = TRUE;
							}
						}
					}
				if( ( _cat.mode != AFRAID
									? avl[ mouse.body.dir ]
									: avl[ bdirs[ mouse.body.dir ] ] ) &&
					! rndvalue( _cat.mode == EATEN ? 8 : 12 ) )
					dir = _cat.mode != AFRAID ? mouse.body.dir
											  : bdirs[ mouse.body.dir ];
					else
						if( avl[ _dir[ 0 ] ] && rndvalue( ( count +1 )*2 ) )
							dir = _dir[ 0 ];	/* run to nearest way */
							else
								if( avl[ _dir[ 1 ] ] && rndvalue( ( count +1 )*(2+
                        														_cat.mode) ) )
									dir = _dir[ 1 ];	/* run to longest way */
									else
										if( avl[ bdirs[ _dir[ 1 ] ] ]
											&& ! rndvalue( 40 ) )
											dir = bdirs[ _dir[ 1 ] ];
											else
												if( ! isway &&
													! rndvalue( 12 ) )
													dir = back;	/* run away from mouse */
													else
														if( avl[ old ] )
															dir = old; /* don't change way */
				}
				else
					dir = _dir[ 0 ];

			if( _cat.mode != AFRAID ||
				( _cat.time % SKIPST ) != FALSE )
				{
				step( &_cat.body, _cat.body.dir = dir );

				showbody( &_cat.body, ( _cat.time % 2 ) == FALSE
					? catbd1[ dir > 1 ][ _cat.mode ]
					: catbd2[ dir > 1 ][ _cat.mode ], _cat.mode == AFRAID
															? AFCOLOR
															: CTCOLOR );

				_cat.avlb = FALSE;
				}

			if( _cat.shiting &&
						maze( _cat.body.vert, _cat.body.horz ) == SPACE &&
						was_dot( _cat.body.vert, _cat.body.horz ) )
				{
				I_was( _cat.body.vert, _cat.body.horz, DOT );
				++cmaze.dotcnt;
				}

			if( ( _cat.mode == AFRAID &&
				  --( _cat.count ) == 0 ) ||
				( _cat.mode == EATEN &&
				  _cat.body.vert == cmaze.cvs[ count ] &&
				  _cat.body.horz == cmaze.chs[ count ] ) )
				{
				_cat.mode = NORMAL;
				_cat.avlb = TRUE;
				}
			if( ! _cat.looking &&
				_cat.body.vert == cmaze.hvp &&
				_cat.body.horz == cmaze.hhp )
				_cat.looking = TRUE;
			if( gohome &&
				_cat.body.vert == cmaze.hvp &&
				_cat.body.horz == cmaze.hhp )
				_cat.looking = FALSE;


			if( _cat.body.vert == mouse.body.vert &&
				_cat.body.horz == mouse.body.horz )
				{
				if( _cat.mode == NORMAL )
					{
					agonia( &mouse.body );
					keystrokes = cycle + 2;
					return TRUE;
					}
				if( _cat.mode == AFRAID )
					{
					_cat.mode = EATEN;
					shcount( score += ++catseat * CATBON );
					if( score >= trybon )
						{
						++retryes;
						rdrwrt();
						trybon += TRYBINC;
						}
               if( _cat.shiting )
               	{
                  _cat.shiting = FALSE;
                  -- shitings;
                  }
					}
				}
			cat[ count ] = _cat;
			} /* cats */

		/* --------------------- MOUSE ----------------------- */

		if( is_key() )
			{
			while( is_key() )
				key = get_key();
			if( dir_key( key ) )
				{
				mouse.body.dir = get_dir( key );
				first = FALSE;
				}
			}

		if( ! first )
			{
			if( available( &mouse.body, mouse.body.dir ) )
				{
				step( &mouse.body, mouse.body.dir );
				showbody( &mouse.body, ( mouse.body.dir <= 1 )
								? ( mouse.form++ ? MR1 : MR2 )
								: ( mouse.form++ ? ML1 : ML2 ), MSCOLOR );
				mouse.body.olddir = mouse.body.dir;
				}
				else
					{
					if( available( &mouse.body, mouse.body.olddir ) )
						{
						step( &mouse.body, mouse.body.olddir );
						showbody( &mouse.body, ( mouse.body.olddir <= 1 )
										? ( mouse.form++ ? MR1 : MR2 )
										: ( mouse.form++ ? ML1 : ML2 ), MSCOLOR );
						}
						else
							showbody( &mouse.body,
										( mouse.body.olddir <= 1 ) == FALSE
										? MR1 : MR2, MSCOLOR );
					}
			}

		switch( mouse.body.olddir )
			{
			case RIGHT:
				headv = vinch( mouse.body.horz, mouse.body.vert );
				headh = cinch( mouse.body.horz );
				break;
			case LEFT:
				headv = vdech( mouse.body.horz, mouse.body.vert );
				headh = cdech( mouse.body.horz );
				break;
			default :
				headv = mouse.body.vert;
				headh = mouse.body.horz;
				break;
			}
		if( ( xp = maze( headv, headh ) ) == PILL )
			{
			usint nonafr;

         if( ( nonafr = score / NOAFR ) > MAXNONAFR )
				nonafr = MAXNONAFR;
			catseat = 0;
			val.number = rndvalue( 4 );
         for( count = 0; count != MAXCATS; ++count, ++val.number )
            {
            CAT _cat;

            _cat = cat[ val.number ];

            if( _cat.mode == AFRAID || _cat.mode != EATEN &&
               _cat.looking &&
               ( nonafr ? rndvalue( abs( MAXCATS * 2 - nonafr ) )
                        : TRUE ) )
               {
               _cat.mode = AFRAID;
               _cat.count = catsdelay;
               _cat.avlb = TRUE;
               }
            if( nonafr )
               --nonafr;
            cat[ val.number ] = _cat;
            }
			shcount( score += DOTWEIGHT );
			if( --cmaze.dotcnt == 0 )
				{
				keystrokes = cycle + 2;
				return FALSE;
				}
			}
		if( xp == DOT )
			{
			shcount( score += DOTWEIGHT );
			if( --cmaze.dotcnt == 0 )
				{
				keystrokes = cycle + 2;
				return FALSE;
				}
			}
		if( isbonus && headv == cmaze.bvp && headh == cmaze.bhp )
			{
			shcount( score += BONWEIGHT * ( bonus + 1 ) );
			isbonus = FALSE;
			cursor( cmaze.bvp, cmaze.bhp - 1 );
			displst( SPACES );
			boncycl = NOBONUS;
         catsdelay -= MAXBONUS * 5;
         --boncnt;
			}
		I_was( headv, headh, SPACE );
		if( score >= trybon )
			{
			++retryes;
			rdrwrt();
			trybon += TRYBINC;
			}

      if( ( value = curr_time() - st_time ) < NORM_GAME_TIME )
      	time_delay( NORM_GAME_TIME - ( usint )value );

		} /* game stroke */
}

int play_game( void )
{
int retval = _play_game();

	if( isbonus )
   	--boncnt;
   isbonus = FALSE;
	while( is_key() )
      get_key();
   return retval;
}

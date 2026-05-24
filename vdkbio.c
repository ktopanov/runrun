/*

vdkbio.c - system dependent routines for runrun

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

#include "runtyp.h"

#ifndef 	randomize
#define  randomize()	(srand((unsigned)time(NULL)))
#endif

#ifndef	random
#define	random(VAL)	((rand())%(VAL))
#endif

#ifdef	__WIN32__

HANDLE hWnd = INVALID_HANDLE_VALUE;
CONSOLE_SCREEN_BUFFER_INFO csbiInfo;   /* Console information */

#endif

#if defined( __MSDOS__ )
static int vpage;
static int _timer_on_start;
#endif

uchar vcursor, hcursor;
extern int gamestat;
extern void shtop20( void );
extern void shall( void );
extern void redraw_maze( void );

int mivert, mihorz, snvert = 10, snhorz = 10;
int notuse = TRUE;

static uchar avlkeys[] = {
KB_UP1, KB_UP2, KB_UP3, KB_UP4,
KB_DOWN1, KB_DOWN2, KB_DOWN3, KB_DOWN4,
KB_LEFT1, KB_LEFT2, KB_LEFT3, KB_LEFT4,
KB_RIGHT1, KB_RIGHT2, KB_RIGHT3, KB_RIGHT4,
};

usint darkmaze = FALSE;

void displst( uchar * );	/* just only prototypes */
int is_key( void );
unsigned long start_time( void );

/* ----------- unix specific code (but not yet all) -------------- */

#ifdef __unix__

off_t filelength (int fd)
{
  struct stat sb;

  fstat (fd, &sb);
  return sb.st_size;
}

int kbhit ()
{
  fd_set inp;
  struct timeval timeout;

  timeout.tv_sec =  timeout.tv_usec = 0;
  FD_ZERO (&inp);
  FD_SET (0, &inp);
  select (1, &inp, 0, 0, &timeout);
  return FD_ISSET (0, &inp);
}

/** #error Perhaps you should remove this function (look at rundef.h) **/

#if 0

int Getch ()
{
  int key;

  switch ((key = getch())) {

#if defined(__NCURSES_H)
  case KEY_LEFT:
#endif
  case 'j':
  case 'J':
    return KB_LEFT1;
    break;

#if defined(__NCURSES_H)
  case KEY_RIGHT:
#endif
  case 'l':
  case 'L':
    return KB_RIGHT1;
    break;

#if defined(__NCURSES_H)
  case KEY_DOWN:
  case KEY_B2:
#endif
  case 'k':
  case 'K':
    return KB_DOWN1;
    break;

#if defined(__NCURSES_H)
  case KEY_UP:
#endif
  case 'i':
  case 'I':
    return KB_UP1;
    break;

#if defined(__NCURSES_H)
  case KEY_BACKSPACE:
#endif
  case 9:
    return CHIEF;
    break;

  default:
    return key;
  }
}

#endif

void reset_screen()
{
        keypad(stdscr, FALSE);
        nl();
        nocbreak();
        echo();
        endwin();
}

#endif	/* __unix__ */

/* ----------- MS-DOS specific code (but not yet all) -------------- */

#ifdef __MSDOS__

static void _restore_timer( void )
{
    _asm{
    cli

    mov	ax, _timer_on_start
    out	42h, al
    mov	al, ah
    out	42h, al

    sti
   }
}

static void _store_timer( void )
{
	_asm{
   cli

   in			al, 61h
   xor		al, 02h
   out		61h, al

	in		al, 42h
   mov	ah, al
   in		al, 42h
   xchg	al, ah
   mov	_timer_on_start, ax

   sti
   }
}

#endif

/* ------------------ system depended functions ------------------ */

void specstart( void )		/* Placeholder for special initialization */
									/* but _NOT_ for display initialization */
{
#if defined(__unix__) && defined(DEBUG)
  printf ("Pid=%d, ready? ", getpid());
  getchar();
#endif

#ifdef __WIN32__

	timeBeginPeriod( 1 );

#endif /* __WIN32__ */

#ifdef __MSDOS__

    _store_timer();

#endif
}

void bye_user( int val )		/* upon exit function */
{
#ifdef __WIN32__

	timeEndPeriod( 1 );

#endif /* __WIN32__ */

#ifdef __MSDOS__

	_restore_timer();

#endif

	exit( val );
}

/* function to set standart console mode (80x25) in output window\screen */

void mode3( void )
{
#ifdef __MSDOS__

#ifdef __BORLANDC__

struct REGPACK r;

	r.r_ax = 0x0003;
	intr( 0x10, &r );

#else

    union REGS rg;

	rg.h.ah = 0x00;
   rg.h.al = 0x03;
	int86( 0x10, &rg, &rg);

#endif	/* __BORLANDC__ */

#endif	/* __MSDOS__ */

#ifdef __WIN32__

   if( hWnd == INVALID_HANDLE_VALUE )
    	{
    	hWnd = GetStdHandle( STD_OUTPUT_HANDLE );
      GetConsoleScreenBufferInfo( hWnd, &csbiInfo );
    	}
	if( hWnd == INVALID_HANDLE_VALUE )
   	{
      displst( "Console error" );
      bye_user( 0 );
      }
#endif /* __WIN32__ */

#ifdef __unix__

        clear();
        refresh();

#endif	/* __unix__ */

}

/* function to initilize computers's window\screen system */

void video_init( void )
{
#ifdef __MSDOS__

union REGS out;

        out.h.ah = 0xf;
        int86( 0x10, &out, &out );
        vpage = out.h.bh;

#endif	/* __MSDOS__ */

#ifdef __unix__
        initscr();      /* initialize the curses library */
        keypad(stdscr, TRUE);  /* enable keyboard mapping */
        nonl();         /* tell curses not to do NL->CR/NL on output */
        cbreak();       /* take input chars one at a time, no wait for \n */
        noecho();       /* don't echo input */
        refresh();
        atexit (reset_screen);
#endif

        mode3();
}

/* function to set cursor position in window */

void cursor( uchar v, uchar h )
{
#ifdef __MSDOS__

union REGS in;

	in.h.ah = 2;
	in.h.bh = vpage;
	in.h.dh = v;
	in.h.dl = h;
	int86( 0x10, &in, &in );

#endif	/* __MSDOS__ */

#ifdef __WIN32__

COORD pos;

   pos.X = h;
   pos.Y = v;
   SetConsoleCursorPosition( hWnd, pos );

#endif	/* __WIN32__ */

#ifdef __unix__

   move (v, h);

#endif	/* __unix__ */

#if	0	/* unknown */

   gotoxy( h+1, v+1 );

#endif	/* unknown */

	vcursor = v;
	hcursor = h;
}

/* function to turn cursor off, if possible */

void off_cursor( void )
{
#ifdef __MSDOS__

union REGS in;

	in.h.ah = 1;
	in.x.cx = 0x2020;
	int86( 0x10, &in, &in );

#endif	/* __MSDOS__ */

#ifdef	__WIN32__

CONSOLE_CURSOR_INFO CurInfo;

    GetConsoleCursorInfo( hWnd, &CurInfo );
    CurInfo.bVisible = FALSE;
    SetConsoleCursorInfo( hWnd, &CurInfo );

#endif

#ifdef 	__unix__

    int y, x;
    getyx (stdscr, y, x);
    leaveok (stdscr, FALSE);
    move (0, 0);
    refresh();
    leaveok (stdscr, TRUE);
    move (y, x);

#endif	/* __unix__ */

}

/* function to turn cursor on, if possible */

void on_cursor( void )
{
#ifdef __MSDOS__

union REGS in;

	in.h.ah = 1;
	in.h.ch = 6;
	in.h.cl = 7;
	int86( 0x10, &in, &in );

#endif	/* __MSDOS__ */

#ifdef	__WIN32__

CONSOLE_CURSOR_INFO CurInfo;

	mode3();
	GetConsoleCursorInfo( hWnd, &CurInfo );
   CurInfo.bVisible = TRUE;
   SetConsoleCursorInfo( hWnd, &CurInfo );

#endif	/* __WIN32__ */

#ifdef __unix__

    leaveok (stdscr, FALSE);
    refresh();

#endif	/* __unix__ */

}

/* function to clear window\screen */

void clear_screen( void )
{
#ifdef	__MSDOS__

union REGS reg;

	reg.h.ah = 6;
	reg.h.ch = reg.h.cl = reg.h.al = ( uchar ) NULL;
	reg.h.bh = BGCOLOR * 16 + PNTCOLOR;
	reg.h.dh = 24;
	reg.h.dl = 79;

	int86( 0x10, &reg, &reg );

#else

#ifdef __WIN32__

DWORD    dummy;
COORD    Home = { 0, 0 };

    FillConsoleOutputCharacter( hWnd, ' ',
    									  csbiInfo.dwSize.X * csbiInfo.dwSize.Y,
                                Home, &dummy );
    FillConsoleOutputAttribute( hWnd, FOREGROUND_RED | FOREGROUND_BLUE
    											  | FOREGROUND_GREEN,
    									  csbiInfo.dwSize.X * csbiInfo.dwSize.Y,
                                Home, &dummy );
#else

	mode3();

#endif	/* WIN32 */

#endif	/* MSDOS */
}

/* function to call something when CHIEF HERE button pressed */

void do_chief( void )
{
uchar _vc, _hc;

/* #ifdef _M_IX86 */
#ifdef _COMMANDCOM_
uchar buffer[ 127 ];
uchar *ptr;
#endif

	clear_screen();
	_vc = vcursor;
	_hc = hcursor;

	mode3();

#ifdef __MSDOS__

	_restore_timer();

#endif

#ifdef _COMMANDCOM_
	strcpy( (char*)buffer, ( (char*)ptr = getenv( "COMSPEC" ) )
   												== NULL ? "C:\\COMMAND"
													 	  	  : (char*)ptr );
	system( (char*)buffer );
#endif

#ifdef __MSDOS__

	_store_timer();

#endif

#ifdef	__unix__
        endwin();
        system(getenv("SHELL"));
        refresh();
        nonl();
        cbreak();
        noecho();

#endif

	mode3();


	clear_screen();
	switch( gamestat )
		{
		case GAMEBEG:
			shtop20();
			cursor( 12, 37 );
			displst( (uchar*)"BEGIN ?" );
			on_cursor();
			break;
		case GAMEGO:
			off_cursor();
			redraw_maze();
			shall();
      	getch();
			break;
		case GAMEEDT:
			shtop20();
			cursor( _vc, _hc );
			on_cursor();
			break;
		}
}

/* function to tell is any key code in system's buffer */

int is_key( void )
{
  return( kbhit() );
}

/* function to take code from buffer */

uchar get_key( void )
{
uchar retval = ((uchar)CR);
int okkey = FALSE;

	do
		{
/*
#ifdef	__MSDOS__
		union REGS in, out;

		in.h.ah = 0;
		int86( 0x16, &in, &out );
		retval = out.h.al ? out.h.al : out.h.ah;
#else
*/
      retval = getch();
/*
#endif
*/
		switch( retval )
			{
			case ESCAPE:
				clear_screen();
				mode3();
				bye_user( 1 );
			case CHIEF:
				do_chief();
            retval = ' ';
            okkey = FALSE;
            break;
			default :
				okkey = TRUE;
			}
		}
	while( ! okkey );

	return retval;
}

/* function to display character with color\attribute */

void displch( uchar val, uchar color )
{
#ifdef __MSDOS__

#ifdef __BORLANDC__

struct REGPACK r;

	*( ( ( uchar * ) &r.r_ax ) + 1 ) = 0x9;
	*( ( uchar * ) &r.r_ax ) = val;
	*( ( ( uchar * ) &r.r_bx ) + 1 ) = vpage;
	*( ( uchar * ) &r.r_bx ) = color + BGCOLOR * 16;
	r.r_cx = 1;
	intr( 0x10, &r );
	r.r_ax = 0x200;
	*( ( ( uchar * ) &r.r_bx ) + 1 ) = vpage;
	*( ( ( uchar * ) &r.r_dx ) + 1 ) = vcursor;
	*( ( uchar * ) &r.r_dx ) = ++hcursor;
	intr( 0x10, &r );

#else /* !__BORLANDC__ */

    union REGS rg;

    rg.h.ah = 0x09;
    rg.h.al = val;
    rg.h.bl = color + BGCOLOR * 16;
    rg.h.bh = vpage;
    rg.x.cx = 1;
	int86( 0x10, &rg, &rg);

    rg.h.ah = 0x02;
    rg.h.bh = vpage;
    rg.h.dh = vcursor;
    rg.h.dl = ++hcursor;
    int86( 0x10, &rg, &rg);

#endif	/* __BORLANDC__ */

#endif		/* __MSDOS__ */

#ifdef __unix__

        addch (val);
        refresh();

#endif		/* __unix__ */

#ifdef __WIN32__

    putchar( val );
    ++hcursor;

#endif
}

#if defined( __MSDOS__ )

void OsSleep( long ms )
{
    union REGS rg;
    int86( 0x28, &rg, &rg );    /* DOS is free */
}

#endif /* __MSDOS__ */

/* function to take current time in msecs (don't care system timer's starting
   point) should return some absolute value, if start_time returns absolute
   value too, or interval from start_time call */

unsigned long curr_time( void )
{
#ifdef __WIN32__

	return timeGetTime();

#endif

#if defined( __MSDOS__ )

usint retval;

	_asm
   {
   cli
   in 	al, 42h
   mov   bl, al
   in		al, 42h
   mov	bh, al
   sti
   mov	retval, bx
   }

   return ( 0xffff - retval ) / 1193;

#endif /* __MSDOS__ */

#if defined( __unix )

    struct timeb systime;
    ftime( &systime );
    return systime.time * 1000 + systime.millitm;

#endif
}

/* function to take time value in msecs (don't care system's timer starting
   point) should return absolute starting measurement time, if curr_time
   returns absolute time too, or 0, if curr_time returns interval value
   from call to start_time */

unsigned long start_time( void )
{
#ifdef __MSDOS__

	_asm
   {
	cli
	mov		al, 0b6h
	out		43h, al

   mov		al, 0ffh
   out		42h, al
   out		42h, al
   sti
   }

   return 0;
#else

	return curr_time();

#endif
}

/* function to produce time delay in msecs, if possible shouldn't
   use start_time or curr_time functions */

void time_delay( usint value )
{
#ifdef __MSDOS__
#if defined( __BORLANDC__ )

	if( ( _CX = value ) == FALSE )
		return;

	asm {
		cli
		mov		al, 0b6h
		out		43h, al
		sti
		}

	do
		{
		asm {
			in			al, 40h
			mov		bl, al
			in			al, 40h
			mov		bh, al
			}
		do
			{
			asm {
				in			al, 40h
				mov		ah, al
				in			al, 40h
				xchg    	ah, al
				sub		ax, bx
				neg		ax
				}
			}
		while( ( usint ) _AX < ( usint ) 1193*2 );
		}
	while( --_CX );

#elif defined( _MSVC )

    if( value == 0 )
        return;

    _asm
    {
        mov     cx, value
		cli
		mov		al, 0b6h
		out		43h, al
		sti
    }
    _asm
    {
    m1:
        in      al, 40h
        mov     bl, al
        in      al, 40h
        mov     bh, al

    m2:
        in      al, 40h
        mov     ah, al
        in      al, 40h
        xchg    ah, al
        sub     ax, bx
        neg     ax
        cmp     ax, 1193*2
        jc      m2

        dec     cx
        jne     m1
    }

#else /* !__BORLANDC__ && !_MSVC in msdos */

    unsigned long st, t;
    st = curr_time();

    do
    {
        OsSleep( 1 );
    }
    while( labs( curr_time() - st ) <= value );

#endif /* __BORLANDC__ */

#endif		/* __MSDOS__ */

#ifdef __unix__
        /** Providing millisecond value **/
        usleep (1000*value);
#endif

#ifdef __WIN32__

	Sleep( value );

#endif

#if 0		/* unknown without delay function */

unsigned long start;

    start = curr_time();
    while( labs( curr_time() - start ) <= value )
        ;

#endif	/* unknown */
}

/* ------------ SYSTEM INDEPENDED FUNCTIONS (HANDS OFF) --------------- */

int dir_key( uchar key )
{
usint index;

	for( index = 0; index != sizeof( avlkeys ); ++index )
   	if( avlkeys[ index ] == key )
      	return TRUE;
   return FALSE;
/*
	switch( key )
		{
		case KB_UP1:
		case KB_UP2:
		case KB_UP3:
		case KB_UP4:
		case KB_DOWN1:
		case KB_DOWN2:
		case KB_DOWN3:
		case KB_DOWN4:
		case KB_LEFT1:
		case KB_LEFT2:
		case KB_LEFT3:
		case KB_LEFT4:
		case KB_RIGHT1:
		case KB_RIGHT2:
		case KB_RIGHT3:
		case KB_RIGHT4:
			return TRUE;
		}
	return FALSE;
*/
}

int get_dir( uchar key )
{
static uchar __direction[] = { UP, DOWN, LEFT, RIGHT };
usint index;

	for( index = 0; index != sizeof( avlkeys ); ++index )
   	if( avlkeys[ index ] == key )
      	return __direction[ index / ( sizeof( __direction ) ) ];
/*
	switch( key )
		{
		case KB_UP1:
      case KB_UP2:
      case KB_UP3:
      case KB_UP4:
			return UP;
		case KB_DOWN1:
		case KB_DOWN2:
		case KB_DOWN3:
		case KB_DOWN4:
			return DOWN;
		case KB_LEFT1:
		case KB_LEFT2:
		case KB_LEFT3:
		case KB_LEFT4:
			return LEFT;
		case KB_RIGHT1:
		case KB_RIGHT2:
		case KB_RIGHT3:
		case KB_RIGHT4:
			return RIGHT;
		}
	return FALSE;
*/
	return 0;
}

void displst( uchar *string )
{
static usint count = 0;

	if( darkmaze )
   	{
		for( count = 0; string[ count ] >= SPACE; ++count )
      	{
         uchar curchar = string[ count ];

         switch ( curchar )
         	{
         	case PILL:
            case DOT:
            	break;
            default:
            	curchar = SPACE;
            }
			displch( curchar, curchar == PILL ? PLCOLOR : TXCOLOR );
         }
      }
   	else
			for( count = 0; string[ count ] >= SPACE; ++count )
				displch( string[ count ],
          	  	   	string[ count ] == PILL ? PLCOLOR : TXCOLOR );
}

void rndinit( void )
{
	randomize();
}

usint rndvalue( usint from )
{
	return random( from );
}

usint ghost( usint par )
{
	return par;
}


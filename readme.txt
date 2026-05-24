***some words about license
Please read the GPL.TXT in runrun.zip. Here is part of GPL.TXT -
***
NO WARRANTY 

BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY FOR THE 
PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW. EXCEPT WHEN OTHERWISE STATED 
IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM "AS 
IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT 
NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
PARTICULAR PURPOSE. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE 
PROGRAM IS WITH YOU. SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF 
ALL NECESSARY SERVICING, REPAIR OR CORRECTION. 

IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING WILL ANY 
COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR REDISTRIBUTE THE 
PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY GENERAL, 
SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OR INABILITY 
TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF DATA OR DATA BEING 
RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD PARTIES OR A FAILURE OF 
THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS), EVEN IF SUCH HOLDER OR OTHER 
PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. 
***
DON'T redistribute runrun.zip with different 
name. DON'T redistribute runrun.zip as whole or any file from it without 
GPL.TXT. THERE IS NO special features to view GPL.TXT throw executable.
At least one copy of GPL.TXT should be redistributed with any file of this 
package on any storage in plane text form.

***end of license staff

=0==0==0==0==0==0==0==0==0==0==0==0==0==0==0==0==0==0==0==0==0==0==0=
RUNRUN (SP21, SL22, TARAK, KROL, CATS, RAT...) gamma 3.62

				"For experts thing" 
				(C) M. Zhvanetsky.

	First I want say "Thanks" for my friends - Serge Werner and Konstantin
Topanov. They helped me remember original and tested this game.

	This game first appeared... somebody wrote it for RT11 (DEC) operating 
system several years ago. It is sequel of well known PAKMAN, but this release had 
(and has) very clever strategy for animals. Sorry - it is console game. If you 
never spend hours running >X< in maze - probably it's not interesting for you. 
But try :-). 
	First release I made in 1992. Cats (=0=) was so stuped.
This release is raser different. And this source is portable (tested in 
FreeBSD, Linux, DOS and... so on :-). Here I include dynamical ELF-binary
for Linux.
	There are differencies from original game. For beginners -
some features during game looked like bugs. But they are not. :-) I
tried make it as closer to original as possible.

* First difference from original (not going to chage)

	In original - if newlive appeared to shut right maze hyperthroway 
then way didn't open after death. Because of internal reasons here it opens.
	
	Other differencies if they are can relate to time intervals
an random magic.

* Keyboard

	I i 8 <GreyUP>          - up
	J j 4 <GreyLeft>        - left
	K k 5 <GreyDown>        - down (depends of NumLock for gray pad)
	L l 6 <GreyRight>       - right

(in _other_ OS used YyGgHhJj instead - you shold not change anything
in source - just compile)

	Space                   - chief here (Tab in _other_ OS)
	Esc                     - escape right now (no savings)

Name in Top20 (if you was good player :-) ) could be edited by <BackSpace>.

* Mazes

	Mazes are stored in text file RUNRUN.MAZ (Warning! Uppercase
letters for compatability). They could be edited with any tool not
cutting trailing spaces. It is allowed to create own mazes using
several rules:

- minimal number of mazes per file - 4, maximum - 200 (enough I hope :-)
- each maze should have 24 lines, each line 79 characters (except
  line breaks), empty lines should be filled by spaces to 79
- should not be empty lines between mazes
- number of line breaking characters doesn't care - you can use same
  maze file in _other_ OS
- each maze should have
  + no more than 4 cats =0= (if less, there will be 4 instead :-)
  + one mouse >X<
  + cat's acceptuation point H - cats will go to this point after exiting
    home throw walls; after that point they will look for a mouse
  + word SCORE - place to show your score
  + no more than 4 beastpills @
  + sequence *** - place for bonus

Warning! Mouse eats by it's _center_ - therefore not allowed put bread pieces
or beastpills close to vertical wall. Maze with those things never could be thrown.

- things '.' and '@' could be eaten; all other except listed are walls
  (it is able write something like "Mouse Horror" in maze) 
- mazes can have hyperthroways not only horizontal but vertical too
  (for horizontal hyperthroway left hole should be one line lower than right,
  for vertical - on same line and be at least 3 characters wide - look
  at TST.MAZ)
- to load new maze file - type it's name after runrun in command line
  othervise RUNRUN.MAZ will be looked in current directory

Game creates RUNRUN.TOP in CURRENT directory - it is top 20 file. Don't
edit it by text editor.

* Some words about source

All system depending settings are in rundef.h. All system depending functions
are in vdkbio.c. Hope - in *ix you should get working binary only after
editing Makefile. In Linux or FreeBSD just type 'make'. If they are problems - 
you are free to change rundef.h and vdkbio.c. But if you are going port entire 
game to new OS for distribution - let me know first. 

You will good boy if you keep other files unchanged.

* That's all. Joy!

(C) Serge A.Kasatkin (I_am_Hungry).
Russia, Rostov-on-Don, 1998.

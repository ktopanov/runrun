# Makefile for runrun
#Copyright (C) 1998 Serge A.Kasatkin
#
#This program is free software; you can redistribute it and/or modify it under 
#the terms of the GNU General Public License as published by the Free Software 
#Foundation; either version 2 of the License, or (at your option) any later 
#version. 
#
#This program is distributed in the hope that it will be useful, but WITHOUT ANY 
#WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
#PARTICULAR PURPOSE. See the GNU General Public License for more details. 
#
#You should have received a copy of the GNU General Public License along with 
#this program; if not, write to the Free Software Foundation, Inc., 675 Mass Ave, 
#Cambridge, MA 02139, USA. 


#DEBUG_FLAGS= -DDEBUG -g
#DEBUG_LIBS= -lefence


CC=	gcc
CFLAGS= -I/usr/local/include -I/usr/include/ncurses -O $(DEBUG_FLAGS)

HDRS=	envext.h \
	rundef.h \
	runtyp.h

LD=	$(CC)


# FreeBSD
#LIBS=	-L/usr/local/lib -lncurses -lcompat $(DEBUG_LIBS)

# Solaris
#LIBS=	-lncurses -L/usr/ucblib -lucb $(DEBUG_LIBS)

# Linux
LIBS=	-lncurses $(DEBUG_LIBS)

MAKEFILE= makefile

OBJS=	mgff.o \
	runrun.o \
	vdkbio.o

PROGRAM= runrun

SHELL=	/bin/sh

SRCS=	mgff.c \
	runrun.c \
	vdkbio.c

all:		$(PROGRAM)

$(PROGRAM):     $(OBJS)
		$(LD) $(LDFLAGS) $(OBJS) $(LIBS) -o $(PROGRAM) 2>erros

clean:;		@rm -f $(OBJS) core

# DO NOT DELETE

mgff.o: runrun.h rundef.h runtyp.h envext.h
runrun.o: runrun.h rundef.h runtyp.h envext.h
vdkbio.o: runtyp.h


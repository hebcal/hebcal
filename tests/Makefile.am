#Makefile for program tests directory in hebcal
## Copyright (C) 2004 Danny Sadinoff
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
## USA.

## Process this file with automake to produce Makefile.in.

#I sure wish I knew how to create a test program that doesn't get installed... -djs
check_PROGRAMS = sanity-check

# sanity_check_SOURCES = $(top_srcdir)/common.c $(top_srcdir)/danlib.c \
#               $(top_srcdir)/error.c $(top_srcdir)/greg.c sanity-check.c

sanity_check_SOURCES = sanity-check.c

sanity_check_LDADD  = ../common.o ../danlib.o ../error.o ../greg.o 
AM_CPPFLAGS  = -I $(top_srcdir)

TESTS = hebcal-1 hebcal-2 hebcal-3

TESTS_okayfiles = hebcal-test1.ok hebcal-test2.ok hebcal-test3.ok
TESTS_outputfiles = hebcal-test1.out hebcal-test2.out hebcal-test3.out
MOSTLYCLEANFILES = $(TESTS_outputfiles)

EXTRA_DIST = $(TESTS) $(TESTS_okayfiles)

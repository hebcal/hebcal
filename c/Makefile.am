## Process this file with automake to produce Makefile.in
#$Header$

# Copyright(c) 1994-2004  Danny Sadinoff
# Portions Copyright (c) 2002 Michael J. Radwin. All Rights Reserved.
# See section COPYING for conditions for redistribution

#The CITY macro should be set to one of the following:
#ATLANTA AUSTIN BERLIN BOGOTA BOSTON BUENOS_AIRES BUFFALO CHICAGO
#CINCINNATI CLEVELAND DALLAS DENVER DETROIT HAWAII HOUSTON JERUSALEM
#JOHANNESBURG LONDON LOS_ANGELES MIAMI MEXICO_CITY NEW_YORK OMAHA
#PHILADELPHIA PHOENIX PITTSBURGH SAINT_LOUIS SAN_FRANCISCO SEATTLE
#TORONTO VANCOUVER WASHINGTON_DC

##  this sets the default latitude, longitude and timezone.
##  the code affected is in rise.c, so modify as necessary.  
##  See the README file for more information
CITY = NEW_YORK


VERSION = 3.4beta1

#there is no need to change anything below this line
#-----------------------------------------------------------------

AM_CPPFLAGS = -DCITY='"$(CITY)"' 
AM_CPPFLAGS += -DVERSION='"$(VERSION)"'

# AM_CFLAGS = -Wall

SUBDIRS = tests doc

bin_PROGRAMS = hebcal

#sanity_check_SOURCES = common.c danlib.c error.c greg.c sanity-check.c

hebcal_SOURCES = common.c danlib.c error.c mygetopt.c greg.c hebcal.c\
         holidays.c rise.c sedra.c start.c gnu.c format.c


## Process this file with automake to produce Makefile.in
#$Header$

# Copyright(c) 1994-2004  Danny Sadinoff
# Portions Copyright (c) 2002 Michael J. Radwin. All Rights Reserved.
# See section COPYING for conditions for redistribution

#The CITY macro should be set to one of the cities mentioned in cities.h,
#with spaces->underscore for ease of quoting.

##  this sets the default latitude, longitude and timezone.
##  the code affected is in rise.c, so modify as necessary.  
##  See the README file for more information
CITY = New_York


VERSION = 3.4-beta2

#there is no need to change anything below this line
#-----------------------------------------------------------------

AM_CPPFLAGS = -DCITY='"$(CITY)"' 
AM_CPPFLAGS += -DVERSION='"$(VERSION)"'

# AM_CFLAGS = -Wall

SUBDIRS = tests doc

bin_PROGRAMS = hebcal

#sanity_check_SOURCES = common.c danlib.c error.c greg.c sanity-check.c

hebcal_c_files = common.c danlib.c error.c mygetopt.c greg.c hebcal.c\
         holidays.c rise.c sedra.c start.c gnu.c format.c

hebcal_h_files = common.h  danlib.h format.h greg.h hebcal.h \
          myerror.h mygetopt.h mymath.h mystdio.h mystring.h \
          mytime.h rise.h sedra.h cities.h

hebcal_SOURCES = $(hebcal_c_files) $(hebcal_h_files)

BUILT_SOURCES =$(srcdir)/package.m4

$(srcdir)/package.m4: $(top_srcdir)/configure.ac
	{                                      \
	echo '# Signature of the current package.'; \
	echo 'm4_define([AT_PACKAGE_NAME],      [@PACKAGE_NAME@])'; \
	echo 'm4_define([AT_PACKAGE_TARNAME],   [@PACKAGE_TARNAME@])'; \
	echo 'm4_define([AT_PACKAGE_VERSION],   [@PACKAGE_VERSION@])'; \
	echo 'm4_define([AT_PACKAGE_STRING],    [@PACKAGE_STRING@])'; \
	echo 'm4_define([AT_PACKAGE_BUGREPORT], [@PACKAGE_BUGREPORT@])'; \
	} >$(srcdir)/package.m4



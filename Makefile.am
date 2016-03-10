## Process this file with automake to produce Makefile.in
#$Header$

# Copyright(c) 1994-2004  Danny Sadinoff
# Portions Copyright (c) 2002 Michael J. Radwin. All Rights Reserved.
# See section COPYING for conditions for redistribution

# FIX do something with
# groff -T html -man man/man1/hebcal.1 >hebcal-manpage.html
#-----------------------------------------------------------------

#AM_CPPFLAGS += -DHEBCAL_OPTS='"$(HEBCAL_OPTS)"'
AM_CPPFLAGS = -DVERSION='"$(PACKAGE_VERSION)"' -DHAVE_STDLIB_H -DHAVE_STRING_H -DHAVE_INTTYPES_H

# AM_CFLAGS = -Wall
AM_LDFLAGS = $(LIBINTL)

DEFS = -DLOCALEDIR=\"@localedir@\" \
	   @DEFS@

SUBDIRS = po tests

bin_PROGRAMS = hebcal

#sanity_check_SOURCES = common.c danlib.c error.c greg.c sanity-check.c

hebcal_c_files = common.c danlib.c error.c cargo.c greg.c hebcal.c\
         timelib.c astro.c tm2unixtime.c unixtime2tm.c dow.c parse_tz.c \
         holidays.c sedra.c start.c gnu.c dafyomi.c

hebcal_h_files = common.h  danlib.h greg.h hebcal.h \
          myerror.h cargo.h \
          timezonedb.h timelib.h astro.h timelib_structs.h \
          sedra.h cities.h

hebcal_SOURCES = $(hebcal_c_files) $(hebcal_h_files)

man1_MANS = hebcal.1
EXTRA_DIST = config.rpath m4/ChangeLog  $(man1_MANS)

BUILT_SOURCES =$(srcdir)/package.m4


VOLUME_NAME = @PACKAGE_NAME@-@PACKAGE_VERSION@
BINARY_DIR = $(VOLUME_NAME).bin
DISK_IMAGE_NAME = @PACKAGE_NAME@-osx-ppc-@PACKAGE_VERSION@.dmg
HDIUTIL = /usr/bin/hdiutil
binary-osx: hebcal hebcal.1 README.md NEWS
	rm -fr $(BINARY_DIR)
	mkdir -p $(BINARY_DIR)/bin $(BINARY_DIR)/man/man1
	cp hebcal  $(BINARY_DIR)/bin
	cp hebcal.1 $(BINARY_DIR)/man/man1
	cp NEWS $(BINARY_DIR)/NEWS.txt
	cp README.md $(BINARY_DIR)/README.md
	echo "The source for hebcal is available for free at https://github.com/hebcal/hebcal" > $(BINARY_DIR)/SOURCE
	$(HDIUTIL) create -ov -volname $(VOLUME_NAME) -fs HFS+ -srcfolder $(BINARY_DIR) $(DISK_IMAGE_NAME)



$(srcdir)/package.m4: $(top_srcdir)/configure.ac
	{                                      \
	echo '# Signature of the current package.'; \
	echo 'm4_define([AT_PACKAGE_NAME],      [@PACKAGE_NAME@])'; \
	echo 'm4_define([AT_PACKAGE_TARNAME],   [@PACKAGE_TARNAME@])'; \
	echo 'm4_define([AT_PACKAGE_VERSION],   [@PACKAGE_VERSION@])'; \
	echo 'm4_define([AT_PACKAGE_STRING],    [@PACKAGE_STRING@])'; \
	echo 'm4_define([AT_PACKAGE_BUGREPORT], [@PACKAGE_BUGREPORT@])'; \
	} >$(srcdir)/package.m4

check-recursive:  all

ACLOCAL_AMFLAGS = --install -I m4

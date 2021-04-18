## Process this file with automake to produce Makefile.in

# Copyright(c) 1994-2004  Danny Sadinoff
# Portions Copyright (c) 2002 Michael J. Radwin. All Rights Reserved.
# See section COPYING for conditions for redistribution

# FIX do something with
# groff -T html -man man/man1/hebcal.1 >hebcal-manpage.html
#-----------------------------------------------------------------

SUBDIRS = tests

bin_PROGRAMS = hebcal

# ls po/*.po

pofiles = po/ashkenazi_litvish.po po/ashkenazi.po			\
	 po/ashkenazi_poylish.po po/ashkenazi_standard.po po/de.po po/es.po	\
	 po/fi.po po/fr.po po/he.po po/hu.po po/pl.po po/ru.po

# ls po/*.po | sed 's:^po/:strings_:' | sed 's:[.]po$:.c:'

BUILT_SOURCES = strings_ashkenazi_litvish.c strings_ashkenazi.c		\
         strings_ashkenazi_poylish.c strings_ashkenazi_standard.c	\
         strings_de.c strings_es.c strings_fi.c strings_fr.c strings_he.c		\
         strings_hu.c strings_pl.c strings_ru.c

BUILT_SOURCES += translations.c translations.h

$(BUILT_SOURCES): stamp-gperf

stamp-gperf:
	$(top_srcdir)/po2gperf.pl $$(echo $(pofiles) | sed 's:po/:$(top_srcdir)/po/:g')
	touch $@

hebcal_SOURCES = common.c danlib.c error.c greg.c hebcal.c timelib.c	\
         astro.c tm2unixtime.c unixtime2tm.c dow.c parse_tz.c		\
         holidays.c sedra.c start.c gnu.c dafyomi.c $(BUILT_SOURCES)	\
         common.h dafyomi.h danlib.h greg.h hebcal.h myerror.h		\
         timezonedb.h timelib.h astro.h timelib_private.h		\
         timelib_structs.h sedra.h cities.h

man1_MANS = hebcal.1

dist_doc_DATA = NEWS.md README.md

EXTRA_DIST = po2gperf.pl $(pofiles)

VOLUME_NAME = $(PACKAGE_NAME)-$(PACKAGE_VERSION)
BINARY_DIR = $(VOLUME_NAME).bin
DISK_IMAGE_NAME = $(PACKAGE_NAME)-osx-ppc-$(PACKAGE_VERSION).dmg
HDIUTIL = /usr/bin/hdiutil
binary-osx: hebcal hebcal.1 README.md NEWS
	rm -fr $(BINARY_DIR)
	mkdir -p $(BINARY_DIR)/bin $(BINARY_DIR)/man/man1
	cp hebcal  $(BINARY_DIR)/bin
	cp hebcal.1 $(BINARY_DIR)/man/man1
	cp NEWS $(BINARY_DIR)/NEWS.txt
	cp README.md $(BINARY_DIR)/README.md
	echo "The source for hebcal is available for free at $(PACKAGE_URL)" > $(BINARY_DIR)/SOURCE
	$(HDIUTIL) create -ov -volname $(VOLUME_NAME) -fs HFS+ -srcfolder $(BINARY_DIR) $(DISK_IMAGE_NAME)

check-recursive:  all

# Copyright(c) 1994  Danny Sadinoff
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

#if you run into compilation problems, try changing the cc to gcc
CC = gcc

VERSION = 3.3beta9

#there is no need to change anything below this line
#-----------------------------------------------------------------

CFLAGS =  -Wall -g
LDFLAGS = -lm

OBJ = start.o hebcal.o greg.o error.o danlib.o mygetopt.o rise.o \
      common.o holidays.o sedra.o gnu.o

CFILES = common.c danlib.c error.c mygetopt.c greg.c hebcal.c\
         holidays.c rise.c sedra.c start.c gnu.c

HFILES = common.h danlib.h myerror.h mygetopt.h\
        greg.h hebcal.h mymath.h mystring.h mytime.h mystdio.h\
	rise.h sedra.h 

FILES = $(CFILES) $(HFILES) Makefile hebcal.1  Manual Manual.html \
	README hebcal.dsw hebcal.dsp


hebcal:  $(OBJ)
	 $(CC) $(OBJ) -o hebcal $(CFLAGS) $(LDFLAGS)

rise.c:	rise.h 

hebcal.o:	hebcal.h common.h danlib.h myerror.h rise.h greg.h\
                    sedra.h hebcal.c

sedra.o:	 sedra.h hebcal.h sedra.c

gnu.o:		 gnu.c

greg.o:		 greg.h myerror.h  greg.c mytime.h mystring.h

error.o:	 myerror.h error.c

danlib.o:	 danlib.h myerror.h  danlib.c

mygetopt.o:	 mygetopt.h mygetopt.c

common.o:	 common.h greg.h hebcal.h myerror.h common.c

start.o:	 start.c hebcal.h common.h mygetopt.h danlib.h greg.h myerror.h\
                   mystring.h mymath.h Makefile
	 $(CC) $(CFLAGS) -c start.c -o start.o -DCITY='"$(CITY)"' -DVERSION='"$(VERSION)"'

print:	$(FILES) hebcal.1 Manual
	enscript -G2r -pprintout1.ps $? Manual
	ptroff -t -man hebcal.1 > printout2.ps
	touch print
	can Manual

TARDIR =  hebcal-$(VERSION)

tar:
	mkdir $(TARDIR)
	cp $(FILES) $(TARDIR)
	tar cvf - $(TARDIR) | gzip -c > $(TARDIR).tar.gz 
	rm -fr $(TARDIR)

zip:
	mkdir $(TARDIR)
	cp $(FILES) $(TARDIR)
	zip -r $(TARDIR).zip $(TARDIR)
	rm -fr $(TARDIR)

clean:
	rm -f *.o *~ core
	du

indent: $(CFILES)
	gnu-indent $<

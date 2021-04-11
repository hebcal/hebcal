/*
   Hebcal - A Jewish Calendar Generator
   Copyright (C) 1994-2012  Danny Sadinoff
   Portions Copyright (c) 2002 Michael J. Radwin. All Rights Reserved.

   https://github.com/hebcal/hebcal

     This program is free software; you can redistribute it and/or
     modify it under the terms of the GNU General Public License
     as published by the Free Software Foundation; either version 2
     of the License, or (at your option) any later version.

     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with this program; if not, write to the Free Software
     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

     Danny Sadinoff can be reached at
     danny@sadinoff.com
*/

#ifndef __HEBCAL__
#define __HEBCAL__

#include "stdio.h"
#include "greg.h"
#include "myerror.h"
#include "timelib.h"
#include "config.h"
#include "translations.h"

#ifndef ENV_CITY_STR
#define ENV_CITY_STR "HC_CITY"
#endif

#define MAXDAYS 31
#define GREGYR_2_HEBYR(x) ((x)+ 3760)
#define MAX_LINE_LEN 100
#define LEAP_YR_HEB(x) ((1L + (long)(x)* 7L) % 19L < 7L ? 1 : 0)
#define MONTHS_IN_HEB(x) (LEAP_YR_HEB(x) ? 13 :12)
#define LANGUAGE2(str) ((char *)lookup_translation(str))

/* implied */
#define GREG_DATEFORMAT_AMERICAN 0
#define GREG_DATEFORMAT_EURO 1
#define GREG_DATEFORMAT_ISO 2
extern FILE *inFile, *yFile;

extern int
    ashkenazis_sw,
    dafYomi_sw,
    utf8_hebrew_sw,
    candleLighting_sw,
    gregDateOutputFormatCode_sw,
    hebrewDates_sw,
    inputFile_sw,
    israel_sw,
    latdeg, latmin, latsec, longdeg, longmin, longsec,
    latlong_sw,
    printOmer_sw,
    printHebDates_sw,
    printSomeHebDates_sw,
    printMolad_sw,
    printSunriseSunset_sw,
    sedraAllWeek_sw,
    sedrot_sw,
    noGreg_sw,
    noHolidays_sw,
    suppress_rosh_chodesh_sw,
    suppressModern_sw,
    tabs_sw,
    weekday_sw,
    abbrev_sw,
    yearDigits_sw,
    yahrtzeitFile_sw,
   default_zemanim;
extern int twentyFourHour_sw;

extern timelib_tzinfo *TZ_INFO;

extern int havdalah_minutes,
   light_offset;

typedef struct hebrew_year {
   int first_day_of_week;
   int leap_p;
} year_t;

/* holiday typemask entries */
#define USER_EVENT 1
#define LIGHT_CANDLES 2
#define YOM_TOV_ENDS 4
#define CHUL_ONLY 8		/* chutz l'aretz (Diaspora) */
#define IL_ONLY 16		/* b'aretz (Israel) */
#define LIGHT_CANDLES_TZEIS 32
#define CHANUKAH_CANDLES 64

typedef struct hinode{   /* holiday input structure */
    date_t date;
    char *name;
    unsigned int typeMask;
    struct hinode *next;
} holinput_t, *holinputp_t;

typedef struct hsnode{  /* holiday storage structure */
    char *name;
    unsigned int typeMask;
    struct hsnode *next;
} holstore_t, *holstorep_t;


#define ZMAN_ALOT_HASHACHAR (1 <<  0)
#define ZMAN_MISHEYAKIR     (1 <<  1)
#define ZMAN_SUNRISE        (1 <<  2)
#define ZMAN_SZKS           (1 <<  3)
#define ZMAN_TEFILAH        (1 <<  4)
#define ZMAN_CHATZOT        (1 <<  5)
#define ZMAN_MINCHA_GEDOLA  (1 <<  6)
#define ZMAN_MINCHA_KETANA  (1 <<  7)
#define ZMAN_PLAG_HAMINCHA  (1 <<  8)
#define ZMAN_SUNSET         (1 <<  9)
#define ZMAN_CANDLES_BEFORE (1 << 10)
#define ZMAN_CANDLES_AFTER  (1 << 11)
#define ZMAN_TZAIT_42       (1 << 12)
#define ZMAN_TZAIT_72       (1 << 13)
#define ZMAN_HAVDALAH       (1 << 14)

year_t yearData( int );
date_t nextHebDate( date_t );
date_t prevHebDate( date_t );
struct hsnode *getHolstorep( void );
int PushHoliday( struct hsnode *, struct hsnode ** );
void init_holidays( int );
int getHebHolidays( date_t, struct hsnode ** );
void incHebGregDate( date_t *, date_t *,long *,int *,year_t * );
void PrintGregDate( date_t );
void main_calendar( long,long );
void print_candlelighting_times( int, int, date_t );
void print_sunrise_sunset(date_t);
void reset_Omer( int hYear );

extern const char * license[];
extern const char * warranty[];
#endif


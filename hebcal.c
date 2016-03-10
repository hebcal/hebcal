/*
   Hebcal - A Jewish Calendar Generator
   Copyright (C) 1994-2004  Danny Sadinoff
   Portions Copyright (c) 2010 Michael J. Radwin. All Rights Reserved.

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "hebcal.h"
#include "common.h"
#include "danlib.h"
#include "myerror.h"
#include "timelib.h"
#include "sedra.h"
#include "greg.h"
#include "dafyomi.h"

#define _(String) gettext(String)

#define NM_LEN 60

int
  ashkenazis_sw, candleLighting_sw, euroDates_sw, hebrewDates_sw, inputFile_sw,
  israel_sw, latlong_sw, printOmer_sw, printMolad_sw, printSunriseSunset_sw, sedraAllWeek_sw, sedrot_sw, noGreg_sw,
  printHebDates_sw, printSomeHebDates_sw, noHolidays_sw, tabs_sw, weekday_sw,  suppress_rosh_chodesh_sw,
  sunsetAlways_sw, sunriseAlways_sw, default_zemanim,  
  abbrev_sw, first_weekday, this_weekday,
  dafYomi_sw,
  yearDigits_sw, yahrtzeitFile_sw;
timelib_tzinfo *TZ_INFO;
int latdeg, latmin, longdeg, longmin;

int twentyFourHour_sw;
long beginOmer, endOmer;
FILE *inFile, *yFile;
extern holstorep_t holidays[14][MAXDAYS], union_Adar[MAXDAYS];

#define SIMCHAT_DAY 23
int havdalah_minutes = 72;      /* default */
int light_offset = -18;         /* outside jerusalem */

/*-------------------------------------------------------------------------*/

year_t yearData( int hebyr )
{
    date_t tempDate;
    year_t retYear;
    
    tempDate.yy = hebyr;
    tempDate.mm = TISHREI;
    tempDate.dd = 1;
    
    retYear.first_day_of_week = (int) (hebrew2abs (tempDate) % 7L);
    retYear.leap_p = LEAP_YR_HEB (hebyr);
    return retYear;
}

/*-------------------------------------------------------------------------*/

date_t nextHebDate( date_t dth )
{
    dth.dd++;
    if (dth.dd > max_days_in_heb_month (dth.mm, dth.yy))
    {
        if (dth.mm == MONTHS_IN_HEB (dth.yy))
        {
            dth.mm = NISAN;
            dth.dd = 1;
        }
        else
        {
            dth.mm++;
            dth.dd = 1;
            if (dth.mm == TISHREI)
            {
                dth.yy++;
            }
        }
    }
    return dth;
}

/*-------------------------------------------------------------------------*/

/* end-of-loop increment routine */
void incHebGregDate( 
    date_t *dtHeb,
    date_t *dtGreg, 
    long *dtAbs, 
    int *wkday, 
    year_t *theYear
    )
{
  incDate (dtGreg, 1L);
  *dtHeb = nextHebDate (*dtHeb);
  (*dtAbs)++;
  *wkday = (*wkday + 1) % 7;

  if (TISHREI == dtHeb->mm &&   /* if Rosh Hashana then reset YearData */
      1 == dtHeb->dd)
    {
      *theYear = yearData (dtHeb->yy);
      if (sedraAllWeek_sw || sedrot_sw)
        reset_sedra (dtHeb->yy);
      if (printOmer_sw)
        reset_Omer (dtHeb->yy);
      init_holidays(dtHeb->yy);
    }
}

/*-------------------------------------------------------------------------*/

void PrintGregDate( date_t dt )
{
    if (!noGreg_sw)
    {
        if (euroDates_sw)
            printf ("%d.%d.", dt.dd, dt.mm);        /* dd/mm/yyyy */
        else
            printf ("%d/%d/", dt.mm, dt.dd);        /* mm/dd/yyyy */
        
        if (yearDigits_sw)
            printf ("%d", dt.yy % 100);
        else
            printf ("%d", dt.yy);
        
        if (tabs_sw)
            putchar ('\t');
        else
            putchar (' ');
    }
    
    if (weekday_sw)
        printf ("%s, ", ShortDayNames[dayOfWeek (dt)]);
}

/*-------------------------------------------------------------------------*/
/* 
 * zemanim TODO: 
 * -  more iso8859 text.
 * -  add times for chametz on erev pesach.
 * -  add midnight on the seder nights.
 * -  allow user to choose between mogen avraham and gra
 *    (mogen avraham counts the 12 hrs from alos to 72 tzais. 
 *     gra counts sunrise to sunset.)
 */
static struct _zman {
   int flags;
   char *name_sfrd;
   char *name_ashk;
   char *name_8859_8;
   double variable_hour;
   int min_offset;
} zemanim[] = {
   { ZMAN_ALOT_HASHACHAR, "Alot HaShachar", "Alos HaShachar", "Alot HaShachar", 0, -72},
   { ZMAN_MISHEYAKIR,     "Misheyakir", "Misheyakir", "Misheyakir", 0, -45},
   { ZMAN_SUNRISE,        "Sunrise", "Sunrise", "Sunrise", 0, 0, },
   { ZMAN_SZKS,           "Kriat Shema, sof zeman", "Krias Shema, sof zeman", "Kriat Shema, sof zeman", 3, 0 },
   { ZMAN_TEFILAH,        "Tefilah, sof zeman", "Tefilah, sof zeman", "Tefilah, sof zeman", 4, 0 }, 
   { ZMAN_CHATZOT,        "Chatzot hayom", "Chatzos hayom", "Chatzot hayom", 6, 0 },
   { ZMAN_MINCHA_GEDOLA,  "Mincha Gedolah", "Mincha Gedolah", "Mincha Gedolah", 6.5, 0 },
   { ZMAN_MINCHA_KETANA,  "Mincha Ketanah", "Mincha Ketanah", "Mincha Ketanah", 9.5, 0 },
   { ZMAN_PLAG_HAMINCHA,  "Plag HaMincha", "Plag HaMincha", "Plag HaMincha", 10.75, 0 },
   { ZMAN_CANDLES_BEFORE,  "Candle lighting", "Candle lighting", "\344\343\354\367\372 \360\370\345\372" , 12, -18 },
   { ZMAN_SUNSET,          "Sunset", "Sunset", "Sunset", 12, 0 },
   { ZMAN_TZAIT_42,        "Tzait HaKochavim", "Tzais HaKochavim", "Tzait HaKochavim", 12, 42 },
   { ZMAN_TZAIT_72,        "Tzait HaKochavim", "Tzais HaKochavim", "Tzait HaKochavim", 12, 72 },
   { ZMAN_HAVDALAH,        "Havdalah", "Havdalah", "\344\341\343\354\344", 12, 72 },
   { ZMAN_CANDLES_AFTER,   "Candle lighting", "Candle lighting", "\344\343\354\367\372 \360\370\345\372" , 12, 72 }
};

/* For computational purposes, sunrise or sunset is defined to occur
 * when the geometric zenith distance of center of the Sun is 90.8333
 * degrees. That is, the center of the Sun is geometrically 50
 * arcminutes below a horizontal plane. For an observer at sea level
 * with a level, unobstructed horizon, under average atmospheric
 * conditions, the upper limb of the Sun will then appear to be tangent
 * to the horizon. The 50-arcminute geometric depression of the Sun's
 * center used for the computations is obtained by adding the average
 * apparent radius of the Sun (16 arcminutes) to the average amount of
 * atmospheric refraction at the horizon (34 arcminutes).
 *
 * http://aa.usno.navy.mil/faq/docs/RST_defs.php
 *
 * Update Jan 2014: since timelib_astro_rise_set_altitude() subtracts 16
 * arcminutes to account for the apparent radius of the sun ("upper limb
 * correction"), we should use 34 minutes and not 50 mintues here.
 */
#define SUNRISE_SUNSET_ALTITUDE ((double)(-34.0/60.0))

static int get_rise_set(date_t todayGreg, double *h_rise, double *h_set, double *gmt_offset)
{
    double latitude, longitude;
    timelib_sll rise, set, transit;
    int             rs;
    timelib_time   *t;
    double n_offset;
    int hour, minute;

    latitude = (double) latdeg + (latmin / 60.0);
    longitude = (longdeg * -1.0) + ((longmin * -1.0) / 60.0);

    t = timelib_time_ctor();
    t->y = todayGreg.yy;
    t->m = todayGreg.mm;
    t->d = todayGreg.dd;
    t->h = 17;			/* assume 5pm */

    timelib_set_timezone(t, TZ_INFO);
    /* t->tz_info = TZ_INFO; */
    /* t->zone_type = TIMELIB_ZONETYPE_ID; */
    /* t->is_localtime = 1; */
    /* t->have_zone = 1; */
    timelib_update_ts(t, TZ_INFO);

    *gmt_offset = timelib_get_current_offset(t) / 3600;
    rs = timelib_astro_rise_set_altitude(t, longitude, latitude,
					 SUNRISE_SUNSET_ALTITUDE, 1,
					 h_rise, h_set, &rise, &set, &transit);
    timelib_time_dtor(t);
    return rs;
}


void print_sunrise_sunset(date_t todayGreg)
{
    double gmt_offset;
    double h_rise, h_set, N_rise, N_set;
    int rise_hour, rise_minute, set_hour, set_minute;

    get_rise_set(todayGreg, &h_rise, &h_set, &gmt_offset);
    
    N_rise = h_rise + gmt_offset;
    N_set = h_set + gmt_offset;

    if (N_rise > 24 || N_rise < 0) {
	N_rise -= floor(N_rise / 24) * 24;
    }

    rise_hour = (int) N_rise;
    if (rise_hour > 12 && !twentyFourHour_sw) {
	rise_hour = rise_hour % 12;
    }
    rise_minute = (int) (60 * (N_rise - (int) N_rise));

    /* ugly: repeat for sunset */
    if (N_set > 24 || N_set < 0) {
	N_set -= floor(N_set / 24) * 24;
    }

    set_hour = (int) N_set;
    if (set_hour > 12 && !twentyFourHour_sw) {
	set_hour = set_hour % 12;
    }
    set_minute = (int) (60 * (N_set - (int) N_set));

    PrintGregDate (todayGreg);

    printf ("%s:%2d:%02d; %s:%2d:%02d\n",
	    "Sunrise",
	    rise_hour, rise_minute,
	    "Sunset",
	    set_hour, set_minute
	);
}

void print_candlelighting_times( int mask, int weekday, date_t todayGreg)
{
    double gmt_offset;
    double h_rise, h_set, N;
    const int calc_sunset = 1;
    double n_offset;
    int hour, minute, pm;
    int rs;
    int num_zmanim = sizeof (zemanim) / sizeof (struct _zman); 
    int i_zman;
    double var_hr_hours = 0.0, day_span;
    const char *zman_name;
 
    rs = get_rise_set(todayGreg, &h_rise, &h_set, &gmt_offset);
    if (rs != 0) {
        return;
    }
             
    day_span = h_set - h_rise;
    if (day_span < 0)
      day_span += 24;
    var_hr_hours = day_span / 12.0;
        
    for (i_zman = 0; i_zman < num_zmanim; i_zman ++)
    {
       double var_hour;
       char *name;
      
       if ( (zemanim[i_zman].flags & mask) == 0 )
          continue;
        
       var_hour = zemanim[i_zman].variable_hour;
       
       if (var_hour < 12.0)
       {
          N = h_rise + var_hour * var_hr_hours;
          n_offset = (zemanim[i_zman].min_offset) / 60.0;
       }
       else
       {
          N = h_set + (var_hour - 12) * var_hr_hours;
          n_offset = (zemanim[i_zman].min_offset) / 60.0;
       }

       N += gmt_offset;
       N += n_offset;
       if (N > 24 || N < 0)
       {
           N -= floor(N / 24) * 24;
       }
       
       hour = (int) N;
       pm = (hour > 11);
       if (hour > 12 && !twentyFourHour_sw) {
           hour = hour % 12;
       }
       minute = (int) (60 * (N - (int) N));
       
       PrintGregDate (todayGreg);
       zman_name = _(zemanim[i_zman].name_sfrd);
       if (zemanim[i_zman].flags == ZMAN_HAVDALAH) {
            printf(_("%s (%d min):%2d:%02d\n"),
                zman_name,
                havdalah_minutes,
                hour, minute);
       } else {
            printf ("%s: %2d:%02d\n", zman_name, hour, minute);
       }
    }
}

/*-------------------------------------------------------------------------*/

void reset_Omer( int hYear )
{
    date_t d;
    d.dd = 16;
    d.mm = NISAN;
    d.yy = hYear;

    beginOmer = hebrew2abs (d);
    d.dd = 5;
    d.mm = SIVAN;
    endOmer = hebrew2abs (d);
}

/*-------------------------------------------------------------------------*/

void main_calendar( long todayAbs, long endAbs) /* the range of the desired printout */
{
    date_t todayGreg, todayHeb;
    holstorep_t holi_start,holip;         /* a list of holidays for today */
    year_t theYear;
    char *omerStr ;
    int omer, day_of_week, first_weekday, returnedMask;
    int omer_today, sedra_today, candle_today, holidays_today, molad_today;
    molad_t moladNext;
    int monthNext;
    int today_zemanim, i_zman;
    int num_zmanim = sizeof (zemanim) / sizeof (struct _zman); 
    char buffer[80];
    
/* Used to decide whether a particular type of daily info should be
   included in the abbreviated view. In abbreviated mode things like
   sunrise, daf, omer are printed once a week. */
#define INCLUDE_TODAY(_sw) \
  ( (_sw) && ((!abbrev_sw) || (first_weekday == day_of_week)))
    
    todayHeb = abs2hebrew (todayAbs);
    todayGreg = abs2greg (todayAbs);
    
    theYear = yearData (todayHeb.yy);

    /* plug in light_offset before starting the loop */
    for (i_zman = 0; i_zman < num_zmanim; i_zman ++)
       if (zemanim[i_zman].flags == ZMAN_CANDLES_BEFORE)
          zemanim[i_zman].min_offset = light_offset;
       else if (zemanim[i_zman].flags == ZMAN_CANDLES_AFTER ||
                zemanim[i_zman].flags == ZMAN_HAVDALAH )
          zemanim[i_zman].min_offset = havdalah_minutes;
        

    /*============== Main Year Loop ==============*/
    
    
    reset_Omer (todayHeb.yy);
    if (sedraAllWeek_sw || sedrot_sw)
        reset_sedra (todayHeb.yy);
    first_weekday = day_of_week = (int) (todayAbs % 7L);
    while (todayAbs <= endAbs)
    {
        /* get the holidays for today */
      returnedMask = getHebHolidays (todayHeb, &holip);
      
      sedra_today = sedraAllWeek_sw || (sedrot_sw && (day_of_week == SAT));
      omer_today = printOmer_sw &&
          (todayAbs >= beginOmer) &&
          (todayAbs <= endOmer);
      holidays_today = holip &&
        (!noHolidays_sw || (returnedMask & USER_EVENT));
      molad_today = printMolad_sw &&
          (day_of_week == SAT) &&
          (todayHeb.dd >= 23 && todayHeb.dd <= 29) &&
          (todayHeb.mm != ELUL); /* no birkat hachodesh before rosh hashana */
      
      today_zemanim = 0;
      if (INCLUDE_TODAY(default_zemanim))
         today_zemanim |= default_zemanim;
      if (candleLighting_sw)
      {
         if (day_of_week == FRI)
            today_zemanim |= ZMAN_CANDLES_BEFORE;
         else
         {
            if (returnedMask & LIGHT_CANDLES)
               today_zemanim |= (day_of_week == SAT) ?
                  ZMAN_CANDLES_AFTER : ZMAN_CANDLES_BEFORE;
            else 
               if ((returnedMask & CHUL_ONLY) &&
                   ! (returnedMask & YOM_TOV_ENDS))
                  today_zemanim |= ZMAN_CANDLES_AFTER;
         }
         if (!(today_zemanim & (ZMAN_CANDLES_BEFORE | ZMAN_CANDLES_AFTER)) &&
             (day_of_week == SAT || returnedMask & YOM_TOV_ENDS))
            today_zemanim |= ZMAN_HAVDALAH;
         if (!(today_zemanim & (ZMAN_CANDLES_BEFORE)) &&
             (returnedMask & CHANUKAH_CANDLES))
           today_zemanim |= ZMAN_CANDLES_AFTER; /* even if havdalah */
      }
      if (INCLUDE_TODAY(printHebDates_sw) ||
          ((printSomeHebDates_sw || printHebDates_sw) && 
           (holidays_today || sedra_today || omer_today || 
            (today_zemanim & (ZMAN_CANDLES_BEFORE|ZMAN_CANDLES_AFTER|ZMAN_HAVDALAH)))))
      {
          PrintGregDate (todayGreg);
          printf ("%d%s of %s, %d\n",
                  todayHeb.dd,       /* print the hebrew date */
                  numSuffix( todayHeb.dd ),
                  _(hMonths[LEAP_YR_HEB( todayHeb.yy )][todayHeb.mm].name),
                  todayHeb.yy);
      }
      
      if (printSunriseSunset_sw)
      {
          print_sunrise_sunset(todayGreg);
      }
      
      /* print the sedra, if desired */
      if (sedra_today)
      {
          char sedraStr[40];
          int foundSedra = sedra( todayAbs, sedraStr, 40 );
          if (foundSedra)
          {
              PrintGregDate( todayGreg );
              printf( "%s %s\n",
                      _("Parashat"),
                      sedraStr );
          }
      }
      
      /* print today's holidays */
      holi_start=holip;         /* store the head of the list for freeing */
      for (; holip; holip = holip->next)
      {
          if (!noHolidays_sw || (holip->typeMask & USER_EVENT))
          {
              PrintGregDate( todayGreg ); 
              puts( holip->name ); 
          }
      }
      
      /* Print the Omer */
      if (INCLUDE_TODAY(omer_today))
      {
          initStr (&omerStr, NM_LEN);
          omer = (int) (todayAbs - beginOmer + 1L);
          if (!tabs_sw)
          {
              strncat (omerStr, hc_itoa (omer), NM_LEN);
              strncat (omerStr, numSuffix (omer), NM_LEN);
              strncat (omerStr, " day of the Omer", NM_LEN);
          }
          else
          {
              strncat (omerStr, "Omer: ", NM_LEN);
              strncat (omerStr, hc_itoa (omer), NM_LEN);
          }
          PrintGregDate (todayGreg);
          printf ("%s\n", omerStr);
          free( omerStr );
      }
      
      if (INCLUDE_TODAY(dafYomi_sw))
         hebcal_dafyomi(&todayGreg);
      
      /* Print CandleLighting times  */
      if (today_zemanim)
      {
          print_candlelighting_times (today_zemanim,
                                      day_of_week, todayGreg);
      }
      
      /* Print Molad */
      if (molad_today)
      {
          PrintGregDate (todayGreg);
          monthNext = (todayHeb.mm == MONTHS_IN_HEB(todayHeb.yy) ? 1 : todayHeb.mm + 1);
          moladNext = get_molad(todayHeb.yy, monthNext);
          printf ("Molad %s: %s, %d minutes and %d chalakim after %d %s\n",
              hMonths[LEAP_YR_HEB(todayHeb.yy)][monthNext].name,
              ShortDayNames[dayOfWeek(abs2greg(moladNext.day))],
              (int) moladNext.chalakim / 18,
              moladNext.chalakim % 18,
              (moladNext.hour > 12 ? moladNext.hour - 12 : moladNext.hour),
              (moladNext.hour > 12 ? "PM" : "AM")
          );
      }

      incHebGregDate (&todayHeb, &todayGreg, &todayAbs, &day_of_week, &theYear);
      
#     ifdef PLUG_LEAKS
      free_holidays(*holip);
#     endif
    }
#undef INCLUDE_TODAY
}




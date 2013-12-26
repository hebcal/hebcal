/*
   Hebcal - A Jewish Calendar Generator
   Copyright (C) 1994-2004  Danny Sadinoff
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

#include <stdio.h>
#include <stdlib.h>
#include "mystring.h"

#include "hebcal.h"
#include "common.h"
#include "danlib.h"
#include "myerror.h"
#include "rise.h"
#include "sedra.h"
#include "format.h"
#include "greg.h"

#define LANGUAGE(str) (ashkenazis_sw && (str)[1] ? ((str)[1]) : ((str)[0]))

#define NM_LEN 60

int
  ashkenazis_sw, candleLighting_sw, euroDates_sw, hebrewDates_sw, inputFile_sw,
  israel_sw, latlong_sw, printOmer_sw, printMolad_sw, printSunriseSunset_sw, sedraAllWeek_sw, sedrot_sw, noGreg_sw,
  shortGreg_sw,
  printHebDates_sw, printSomeHebDates_sw, noHolidays_sw, tabs_sw, weekday_sw,  suppress_rosh_chodesh_sw,
  sunsetAlways_sw, sunriseAlways_sw, dafYomi_sw,  
  abbrev_sw, first_weekday, this_weekday,
  yearDigits_sw, yahrtzeitFile_sw, DST_scheme, DST_value, default_zemanim;
int iso8859_8_sw;
long beginOmer, endOmer;
int printf_sw = 1;
extern hmonths_t hMonths;
FILE *inFile, *yFile;
char *formatString;
extern holstorep_t holidays[14][MAXDAYS], union_Adar[MAXDAYS];

#define SIMCHAT_DAY 23
int havdalah_minutes = 72;      /* default */
int light_offset = -18;         /* outside jerusalem */

extern const char *shortMonthNames[];


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
    
    if (shortGreg_sw)
    {
    if (weekday_sw)
        printf ("%s ", ShortDayNames[dayOfWeek (dt)]);
      printf("%d-%s: ", dt.dd, shortMonthNames[dt.mm]);
    }
    else if (weekday_sw)
      printf ("%s, ", ShortDayNames[dayOfWeek (dt)]);

    
}

/*-------------------------------------------------------------------------*/

void set_DST_bounds (long *beginDST, long *endDST, int gregYr)
{
    date_t tempDt;
    
    tempDt.yy = gregYr;
    tempDt.dd = 1;
    tempDt.mm = 4;
    
    switch (DST_scheme)
    {
    case DST_USOFA:
      /* FIX: this needs more work for historical DST, obviously */
      if( gregYr >= 1966 && gregYr < 1987 ) 
	{
        /*  the last sunday in April */
	  tempDt.dd = 30;
	  tempDt.mm = 4;
	  *beginDST = day_on_or_before (SUN, greg2abs (tempDt) );

        /*  the sunday before the first of november */
        tempDt.dd = 1;
        tempDt.mm = 11;
        *endDST = day_on_or_before (SUN, greg2abs (tempDt) - 1L);
        DST_value = 60;
        break;
	}
      else if( gregYr >= 1987 && gregYr < 2007 )
	{
        /*  the first sunday in April */
        *beginDST = day_on_or_before (SUN, greg2abs (tempDt) + 6L);


        /*  the sunday before the first of november */
        tempDt.dd = 1;
        tempDt.mm = 11;
        *endDST = day_on_or_before (SUN, greg2abs (tempDt) - 1L);

        DST_value = 60;
        break;
	}
      else if( gregYr >= 2007 )
	{
        /*  Second Sunday in March */
        tempDt.dd = 1;
        tempDt.mm = 3;
        *beginDST = day_on_or_before (SUN, greg2abs(tempDt) + 13L);


        /*  first sunday of november */
        tempDt.dd = 1;
        tempDt.mm = 11;
        *endDST = day_on_or_before (SUN, greg2abs(tempDt) +6L);

        DST_value = 60;
        break;
	}
    case DST_MX:
	/* the first sunday in April */
	*beginDST = day_on_or_before (SUN, greg2abs (tempDt) + 6L);

	/*  the sunday before the first of november */
	tempDt.dd = 1;
	tempDt.mm = 11;
	*endDST = day_on_or_before (SUN, greg2abs (tempDt) - 1L);

	DST_value = 60;
	break;
    case DST_EU:
        /* The EU version of Daylight Saving Time runs from the last
         * Sunday in March through the last Sunday in October.
       * it starts at 1am instead of 2am, but that's not relevant here.
       * http://webexhibits.org/daylightsaving/g.html
       */
        *beginDST = day_on_or_before (SUN, greg2abs (tempDt) - 1L);
        tempDt.dd = 1;
        tempDt.mm = 11;
        /*  the sunday before the first of november */
        *endDST = day_on_or_before (SUN, greg2abs (tempDt) - 1L);
        DST_value = 60;
        break;
    case DST_AUNZ:
        /* Australasia is the inverse of European Union */
        *endDST = day_on_or_before (SUN, greg2abs (tempDt) - 1L);
        tempDt.dd = 1;
        tempDt.mm = 11;
        *beginDST = day_on_or_before (SUN, greg2abs (tempDt) - 1L);
        DST_value = 60;
        break;
    case DST_ISRAEL:
	if (gregYr >= 2005) {
	    /* Starting in 2005, DST begins in Israel on the
	       last Friday before April 2nd, at 2:00 a.m. */
	    /* http://www.gov.il/FirstGov/NewsEng/SavingTime_news.htm */
	    tempDt.dd = 1;
	    tempDt.mm = 4;
	    *beginDST = day_on_or_before (FRI, greg2abs (tempDt));
	} else {
	    /* daylight savings time begins in Israel on the */
	    /* first Sunday after Passover ends on Nisan 21: */
	    tempDt.mm = NISAN;
	    tempDt.dd = 28;
	    tempDt.yy = gregYr + 3760;
	    *beginDST = day_on_or_before (SUN, hebrew2abs (tempDt));
	}
        
  if (gregYr >= 2013) {
    /*
      IDT will end on the last Sunday of October
      http://en.wikipedia.org/wiki/Israel_Summer_Time#2013.E2.80.93Present
    */
        tempDt.dd = 1;
        tempDt.mm = 11;
        /*  the sunday before the first of november */
        *endDST = day_on_or_before (SUN, greg2abs (tempDt) - 1L);
  }
  else {      
        /* daylight savings time ends in Israel on the Sunday Selichot */
        /* begins: */
        tempDt.mm = TISHREI;
        tempDt.dd = 1;
        tempDt.yy = gregYr + 3761;        /* next year */
        *endDST = day_on_or_before (SUN, hebrew2abs (tempDt) - 3L);
  } 
        DST_value = 60;
        break;
    case DST_NONE:
        *endDST = *beginDST = 0L;
        DST_value = 0;
        break;
    default:
        DST_value = 0;
        break;
    }
}

/*-------------------------------------------------------------------------*/

void 
set_DST( long beginDST, long endDST, long todayAbs, int *DST )
{
    if (beginDST > endDST)
    {
	/* Australia */
	if (todayAbs >= beginDST || todayAbs < endDST)
	{
	    *DST = DST_value;
	}
	else
	{
	    *DST = 0;
	}
    }
    else
    {
	if (beginDST <= todayAbs && todayAbs < endDST)
	    *DST = DST_value;
	else
	    *DST = 0;
    }
}

static struct _zman {
   int flags;
   char *name_sfrd;
   char *name_ashk;
   double variable_hour;
   int min_offset;
} zemanim[] = {
   { ZMAN_ALOT_HASHACHAR, "Alot HaShachar", "Alos HaShachar", 0, -72},
   { ZMAN_MISHEYAKIR,     "Misheyakir", "Misheyakir", 0, -45},
   { ZMAN_SUNRISE,        "Sunrise", "Sunrise", 0, 0, },
   { ZMAN_SZKS,           "Kriat Shema, sof zeman", "Krias Shema, sof zeman", 3, 0 },
   { ZMAN_TEFILAH,        "Tefilah, sof zeman", "Tefilah, sof zeman", 4, 0 }, 
   { ZMAN_CHATZOT,        "Chatzot hayom", "Chatzos hayom", 6, 0 },
   { ZMAN_MINCHA_GEDOLA,  "Mincha Gedolah", "Mincha Gedolah", 6.5, 0 },
   { ZMAN_MINCHA_KETANA,  "Mincha Ketanah", "Mincha Ketanah", 9.5, 0 },
   { ZMAN_PLAG_HAMINCHA,  "Plag HaMincha", "Plag HaMincha", 10.75, 0 },
   { ZMAN_SUNSET,         "Sunset", "Sunset", 12, 0 },
   { ZMAN_CANDLES_BEFORE,  "Light Candles Before", "Light Candles Before", 12, -18 },
   { ZMAN_CANDLES_AFTER,   "Light Candles After", "Light Candles After", 12, 42 },
   { ZMAN_TZAIT_42,        "Tzait HaKochavim", "Tzais HaKochavim", 12, 42 },
   { ZMAN_TZAIT_72,        "Tzait HaKochavim", "Tzais HaKochavim", 12, 72 },
   { ZMAN_HAVDALLAH,       "Havdallah after", "Havdallah after", 12, 42 }
};

/* mogen avraham: counts the 12 hrs from alos to 72 tzais. gra counts sunrise to sunset.
 * would be nice to add last time for chametz on erev pesach and auto-select it on the right day.
 * similarly for midnight on the seder nights.
 * add flags for candles. how to choose between ma and gra? how to handle configurable havdalah and
 * candlelighting?
 *
 * also need the iso8859 text back.
 */

/*-------------------------------------------------------------------------*/

void print_sunrise_sunset(date_t todayGreg, int DST)
{
    double xsunrise, xsunset;
    htime_t adj_sunrise, adj_sunset;
    int day_adj, status;
    
    PrintGregDate (todayGreg);
    
    status = suntime (&xsunrise, &xsunset, dayOfYear (todayGreg),
                    SUNRISE, SUNSET);
    if (status & NO_SUNSET)
        printf ("No sunset today.\n");
    else
    {
        timeadj (xsunrise, DST, &day_adj, &adj_sunrise);
        timeadj (xsunset, DST, &day_adj, &adj_sunset);
        printf ("%s: %2d:%02d; %s: %2d:%02d\n",
                "Sunrise",
                adj_sunrise.hours,
                adj_sunrise.minutes,
                "Sunset",
                adj_sunset.hours,
                adj_sunset.minutes
            );
    }
}

void print_candlelighting_times( int mask, int weekday, date_t todayGreg, int DST)
{
        double xsunrise, xsunset;
        int day_adj, status;
    int sunsetExists, sunriseExists;
    htime_t adj_time;
    char buffer[80];
    int num_zmanim = sizeof (zemanim) / sizeof (struct _zman); 
    int i_zman;
    double var_hr_hours = 0.0;
    char *zman_name;
        
        status = suntime (&xsunrise, &xsunset, dayOfYear (todayGreg),
                        SUNRISE, SUNSET);
    
    if (!(status & NO_SUNRISE))
        sunriseExists = TRUE;
        else
        {
       DeclareEvent( &todayGreg, NULL, "No sunrise today.", 0);
        }
    
    if (!(status & NO_SUNSET))
        sunsetExists = TRUE;
    else 
    {
       DeclareEvent( &todayGreg, NULL, "No sunset today.", 0);
    }
    
    if (sunsetExists && sunriseExists)
    {
       double day_span = xsunset - xsunrise;
       if (day_span < 0)
          day_span += 24;
       var_hr_hours = day_span / 12.0;
    }
        
    for (i_zman = 0; i_zman < num_zmanim; i_zman ++)
    {
       double var_hour;
       double offset_from;
       char *name;
      
       if ( (zemanim[i_zman].flags & mask) == 0 )
          continue;
        
       var_hour = zemanim[i_zman].variable_hour;
       
       if (var_hour < 12.0)
       {
          offset_from = xsunrise;
          if (!sunriseExists)
             continue;
       }
        else
        {
          offset_from = xsunset;
          var_hour -= 12.0;
          if (!sunsetExists)
             continue;
        }
          
       timeadj( offset_from + (var_hour * var_hr_hours),
                DST + zemanim[i_zman].min_offset,
                &day_adj, &adj_time );
       
       // a bit heavy-handed, but without this days after 21-Dec 
       // got the pm value wrong somehow.
       adj_time.pm = (offset_from == xsunset);
       
       zman_name = ashkenazis_sw ? zemanim[i_zman].name_ashk :
          zemanim[i_zman].name_sfrd;
       DeclareEvent( &todayGreg, &adj_time, zman_name, 0 );
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
    long beginDST, endDST;
    holstorep_t holi_start,holip;         /* a list of holidays for today */
    year_t theYear;
    char *omerStr ;
    int omer, day_of_week, first_weekday, returnedMask, DST;
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
    set_DST_bounds (&beginDST, &endDST, todayGreg.yy);
    set_DST (beginDST, endDST, todayAbs, &DST);

    /* plug in light_offset before starting the loop */
    for (i_zman = 0; i_zman < num_zmanim; i_zman ++)
      if (zemanim[i_zman].flags == ZMAN_CANDLES_BEFORE)
        {
          zemanim[i_zman].min_offset = light_offset;
          break;
        }

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
      if (INCLUDE_TODAY(sunsetAlways_sw))
         today_zemanim |= ZMAN_SUNSET;
      if (INCLUDE_TODAY(sunriseAlways_sw))
         today_zemanim |= ZMAN_SUNRISE;
      if (candleLighting_sw && (day_of_week == FRI || (returnedMask & LIGHT_CANDLES)))
         today_zemanim |= ZMAN_CANDLES_BEFORE;
      if (candleLighting_sw && (day_of_week == SAT || ((returnedMask & YOM_TOV_ENDS) && day_of_week != FRI)))
         today_zemanim |= ZMAN_HAVDALLAH;
      /* add ZMAN_CANDLES_AFTER for second day of yom tov */

      // Set DST if we are using time based functions
      if (print_sunrise_sunset || today_zemanim || molad_today)
      {
        set_DST (beginDST, endDST, todayAbs, &DST); 
      }
      if (INCLUDE_TODAY(printHebDates_sw) ||
          ((printSomeHebDates_sw || printHebDates_sw) && 
           (holidays_today || sedra_today || omer_today || 
            (today_zemanim & (ZMAN_CANDLES_BEFORE|ZMAN_CANDLES_AFTER|ZMAN_HAVDALLAH)))))
      {
          sprintf (buffer, "%d%s%s %s, %d",
                   todayHeb.dd,       /* print the hebrew date */
                  iso8859_8_sw ? "" : numSuffix( todayHeb.dd ),
                  iso8859_8_sw ? "" : " of",
                  LANGUAGE2(hMonths[LEAP_YR_HEB( todayHeb.yy )][todayHeb.mm].name),
                  todayHeb.yy);
          DeclareEvent (&todayGreg, NULL, buffer, 0);
      }
      
      if (printSunriseSunset_sw)
      {
          print_sunrise_sunset(todayGreg, DST);
      }
      
      /* print the sedra, if desired */
      if (sedra_today)
      {
          char sedraStr[40];
          int foundSedra = sedra( todayAbs, sedraStr, 40 );
          if (foundSedra)
          {
              sprintf(buffer, "%s %s",
                      iso8859_8_sw ? "\364\370\371\372" :
                      ashkenazis_sw ? "Parshas" : "Parashat",
                      sedraStr );
              DeclareEvent( &todayGreg, NULL, buffer, 0 );
          }
      }
      
      /* print today's holidays */
      holi_start=holip;         /* store the head of the list for freeing */
      for (; holip; holip = holip->next)
      {
          if (!noHolidays_sw || (holip->typeMask & USER_EVENT))
          {
              DeclareEvent( &todayGreg, NULL, holip->name, 0 );
          }
      }
      
      /* Print the Omer */
      if (INCLUDE_TODAY(omer_today))
      {
          char omerStr[NM_LEN];
          omerStr[0] = '\0';
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
          DeclareEvent( &todayGreg, NULL, omerStr, 0 );
      }
      
      if (INCLUDE_TODAY(dafYomi_sw))
         hebcal_dafyomi(&todayGreg);
      
      /* Print CandleLighting times  */
      if (today_zemanim)
      {
          print_candlelighting_times (today_zemanim,
                                      day_of_week, todayGreg, DST);
      }
      
      /* Print Molad */
      if (molad_today)
      {
          PrintGregDate (todayGreg);
          monthNext = (todayHeb.mm == MONTHS_IN_HEB(todayHeb.yy) ? 1 : todayHeb.mm + 1);
          moladNext = get_molad(todayHeb.yy, monthNext);
          printf ("Molad %s: %s, %d minutes and %d chalakim after %d %s\n",
              hMonths[LEAP_YR_HEB(todayHeb.yy)][monthNext].name[0],
              ShortDayNames[dayOfWeek(abs2greg(moladNext.day))],
              (int) moladNext.chalakim / 18,
              moladNext.chalakim % 18,
              (moladNext.hour > 12 ? moladNext.hour - 12 : moladNext.hour),
              (moladNext.hour > 12 ? "PM" : "AM")
          );
      }

      incHebGregDate (&todayHeb, &todayGreg, &todayAbs, &day_of_week, &theYear);
      
      if (1 == todayGreg.dd &&  /* happy new year! */
          JAN == todayGreg.mm)
      {
          set_DST_bounds (&beginDST, &endDST, todayGreg.yy);
      }
      
#     ifdef PLUG_LEAKS
      freeHolidays(&holi_start);
#     endif
    }
#undef INCLUDE_TODAY
}

#define MAX_EV 200
HebcalEvent events[MAX_EV];
int num_events = 0;

void clean_events()
{
  int i;
  for (i = 0; i < num_events; i ++)
    if (events[i].desc != NULL)
      {
        free(events[i].desc);
        events[i].desc = NULL;
      }
  num_events = 0;
}

void DeclareEvent(date_t *date, htime_t *time, char *description,
                  int daf_flag)
{

  if (printf_sw)
    {
      PrintGregDate(*date);
      printf("%s", description);
      if (time)
        {
          printf (": %2d:%02d %s", time->hours, time->minutes,
                      time->pm ? "pm": "am" );
        }
      printf("\n");
    }
  else
    {
      int i_ev = num_events;
      if (i_ev == MAX_EV)
        return;
      num_events ++;
      events[i_ev].mm = date->mm;
      events[i_ev].dd = date->dd;
      events[i_ev].yy = date->yy;
      if (time == NULL)
        {
          events[i_ev].time.hours = 
            events[i_ev].time.minutes = 
            events[i_ev].time.pm = 
            0;
        }
      else
        events[i_ev].time = *time;
      events[i_ev].daf_flag = daf_flag;
      makeStr(&events[i_ev].desc, description);
    }
}


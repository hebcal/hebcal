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
#include <string.h>

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
  printHebDates_sw, printSomeHebDates_sw, noHolidays_sw, tabs_sw, weekday_sw,  suppress_rosh_chodesh_sw,
  yearDigits_sw, yahrtzeitFile_sw, DST_scheme, DST_value;
int iso8859_8_sw;
long beginOmer, endOmer;
FILE *inFile, *yFile;
char *formatString;
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

/*-------------------------------------------------------------------------*/

void print_sunrise_sunset(date_t todayGreg, int DST)
{
    double xsunrise, xsunset;
    int day_adj, status;
    
    PrintGregDate (todayGreg);
    
    status = suntime (&xsunrise, &xsunset, dayOfYear (todayGreg),
                    SUNRISE, SUNSET);
    if (status & NO_SUNSET)
        printf ("No sunset today.\n");
    else
    {
        char * time_rise =timeadj ("", xsunrise, DST, &day_adj);
        char * time_set =timeadj ("", xsunset, DST, &day_adj);
        printf ("%s:%s; %s:%s\n",
                "Sunrise",
                time_rise,
                "Sunset",
                time_set
            );
        free( time_rise );
        free( time_set );
        /*              printf("%s %s %.1lf\n",timeadj ("",xsunrise,0,&day_adj),
                        timeadj ("",xsunset,0,&day_adj),
                        (xsunset-xsunrise) * 5.0 + 120.0);
        */
    }
}

void print_candlelighting_times( int mask, int weekday, date_t todayGreg, int DST)
{
    /* offset of sunset to candlelighting in minutes */
    
    if (weekday == FRI || (mask & LIGHT_CANDLES))
    {
        double xsunrise, xsunset;
        int day_adj, status;
        
        PrintGregDate (todayGreg);
        
        status = suntime (&xsunrise, &xsunset, dayOfYear (todayGreg),
                        SUNRISE, SUNSET);
        if (status & NO_SUNSET)
            printf ("No sunset today.\n");
        else
        {
            char * time =timeadj ("", xsunset, light_offset + DST, &day_adj);
            printf ("%s: %s\n",
                    iso8859_8_sw ? "\344\343\354\367\372 \360\370\345\372" :
                    "Candle lighting",
                    time
                );
            free( time );
            /*              printf("%s %s %.1lf\n",timeadj ("",xsunrise,0,&day_adj),
                            timeadj ("",xsunset,0,&day_adj),
                            (xsunset-xsunrise) * 5.0 + 120.0);
            */
        }
    }
    
    /* offset of sunset to havdallah in minutes */
    if (weekday == SAT ||
        ((mask & YOM_TOV_ENDS) &&
        weekday != FRI))
    {
        double xsunrise, xsunset;
        int day_adj, status;
        
      
        PrintGregDate (todayGreg);
        
        status = suntime (&xsunrise, &xsunset, dayOfYear (todayGreg),
                          SUNRISE, SUNSET);
        if (status & NO_SUNSET)
            printf ("No sunset today.\n");
        else
        {
            char * time = timeadj ("",
                                   xsunset,
                                   havdalah_minutes + DST,
                                   &day_adj);
            printf ("%s (%d %s):%s\n",
                    iso8859_8_sw ? "\344\341\343\354\344" : "Havdalah",
                    havdalah_minutes,
                    iso8859_8_sw ? "\343\367\345\372" : "min",
                    time
                    );
            free( time );
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
    long beginDST, endDST;
    holstorep_t holi_start,holip;         /* a list of holidays for today */
    year_t theYear;
    char *omerStr ;
    int omer, day_of_week, returnedMask, DST;
    int omer_today, sedra_today, candle_today, holidays_today, molad_today;
    molad_t moladNext;
    int monthNext;
    
    todayHeb = abs2hebrew (todayAbs);
    todayGreg = abs2greg (todayAbs);
    
    theYear = yearData (todayHeb.yy);
    set_DST_bounds (&beginDST, &endDST, todayGreg.yy);
    set_DST (beginDST, endDST, todayAbs, &DST);

    /*============== Main Year Loop ==============*/
    
    
    reset_Omer (todayHeb.yy);
    if (sedraAllWeek_sw || sedrot_sw)
        reset_sedra (todayHeb.yy);
    day_of_week = (int) (todayAbs % 7L);
    while (todayAbs <= endAbs)
    {
        /* get the holidays for today */
      returnedMask = getHebHolidays (todayHeb, &holip);
      
      sedra_today = sedraAllWeek_sw || (sedrot_sw && (day_of_week == SAT));
      omer_today = printOmer_sw &&
          (todayAbs >= beginOmer) &&
          (todayAbs <= endOmer);
      candle_today = candleLighting_sw &&
          (day_of_week >= FRI ||
           (returnedMask & LIGHT_CANDLES) ||
           (returnedMask & YOM_TOV_ENDS));
      holidays_today = holip &&
        (!noHolidays_sw || (returnedMask & USER_EVENT));
      molad_today = printMolad_sw &&
          (day_of_week == SAT) &&
          (todayHeb.dd >= 23 && todayHeb.dd <= 29) &&
          (todayHeb.mm != ELUL); /* no birkat hachodesh before rosh hashana */
      
      // Set DST if we are using time based functions
      if (print_sunrise_sunset || candle_today || molad_today)
      {
        set_DST (beginDST, endDST, todayAbs, &DST); 
      }
      if (printHebDates_sw ||
          (printSomeHebDates_sw && 
           (holidays_today || sedra_today || omer_today || candle_today)))
      {
          PrintGregDate (todayGreg);
          printf ("%d%s%s %s, %d\n", todayHeb.dd,       /* print the hebrew date */
                  iso8859_8_sw ? "" : numSuffix( todayHeb.dd ),
                  iso8859_8_sw ? "" : " of",
                  LANGUAGE2(hMonths[LEAP_YR_HEB( todayHeb.yy )][todayHeb.mm].name),
                  todayHeb.yy);
      }
      
      if (printSunriseSunset_sw)
      {
          print_sunrise_sunset(todayGreg, DST);
      }
      
      /* print the sedra, if desired */
      if (sedra_today)
      {
          const char *sedraStr = sedra( todayAbs );
          if (NULL != sedraStr)
          {
              PrintGregDate( todayGreg );
              printf( "%s %s\n",
                      iso8859_8_sw ? "\364\370\371\372" :
                      ashkenazis_sw ? "Parshas" : "Parashat",
                      sedraStr );
          }
      }
      
      /* print today's holidays */
      holi_start=holip;         /* store the head of the list for freeing */
      for (; holip; holip = holip->next)
      {
          if (!noHolidays_sw || (holip->typeMask & USER_EVENT))
          {
/*              char buf[200];
              puts(formatLine( todayGreg, todayHeb, holip->name, buf, 200));
*/
              PrintGregDate( todayGreg ); 
              puts( holip->name ); 
          }
      }
      
      /* Print the Omer */
      if (omer_today)
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
      
      /* Print CandleLighting times  */
      if (candle_today)
      {
          print_candlelighting_times (returnedMask,
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
      free_holidays(*holip);
#     endif
    }
}




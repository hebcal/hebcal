/*
   $Id$
   Hebcal - A Jewish Calendar Generator
   Copyright (C) 1994  Danny Sadinoff

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
   1 Cove La.
   Great Neck, NY
   11024

   sadinoff@pobox.com
 */


#include "mystdio.h"
#include <time.h>
#include <string.h>
#include "myerror.h"
#include "greg.h"

/* greg.c gregorian calendar module for hebrew calendar program
   By Danny Sadinoff
   (C) 1992

 */

char *eMonths[] =
{
  "UNUSED",
  "January", "February", "March", "April", "May", "June", "July",
  "August", "September", "October", "November", "December"
};

int MonthLengths[][13] =
{
  {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
  {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

char *ShortDayNames[] =
{
  "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};


/*
 *Return the day number within the year of the date DATE.
 *For example, dayOfYear({1,1,1987}) returns the value 1
 *while dayOfYear({12,31,1980}) returns 366.
 */


int 
dayOfYear (d)
     date_t d;
{
  int dOY = d.dd + 31 * (d.mm - 1);
  if (d.mm > FEB)
    {
      dOY -= (4 * d.mm + 23) / 10;
      if (LEAP (d.yy))
	dOY++;
    }
  return dOY;
}


/*
 * The number of days elapsed between the Gregorian date 12/31/1 BC and DATE.
 * The Gregorian date Sunday, December 31, 1 BC is imaginary.
 */
long int 
greg2abs (d)			/* "absolute date" */
     date_t d;
{
  return ((long) dayOfYear (d)	/* days this year */
	  + 365L * (long) (d.yy - 1)	/* + days in prior years */
	  + (long) ((d.yy - 1) / 4	/* + Julian Leap years */
		    - (d.yy - 1) / 100	/* - century years */
		    + (d.yy - 1) / 400));	/* + Gregorian leap years */
}

/*
 * See the footnote on page 384 of ``Calendrical Calculations, Part II:
 * Three Historical Calendars'' by E. M. Reingold,  N. Dershowitz, and S. M.
 * Clamen, Software--Practice and Experience, Volume 23, Number 4
 * (April, 1993), pages 383-404 for an explanation.
 */
date_t 
abs2greg (theDate)
     long theDate;
{
  int day, year, month, mlen;
  date_t d;
  long int d0, n400, d1, n100, d2, n4, d3, n1;

  d0 = theDate - 1L;
  n400 = d0 / 146097L;
  d1 = d0 % 146097L;
  n100 = d1 / 36524L;
  d2 = d1 % 36524L;
  n4 = d2 / 1461L;
  d3 = d2 % 1461L;
  n1 = d3 / 365L;

  day = (int) ((d3 % 365L) + 1L);
  year = (int) (400L * n400 + 100L * n100 + 4L * n4 + n1);

  if (4L == n100 || 4L == n1)
    {
      d.mm = 12;
      d.dd = 31;
      d.yy = year;
      return d;
    }
  else
    {
      year++;
      month = 1;
      while ((mlen = MonthLengths[LEAP (year)][month]) < day)
	{
	  day -= mlen;
	  month++;
	}
      d.yy = year;
      d.mm = month;
      d.dd = day;
      return d;
    }
}

void 
incDate (dt, n)			/* decrements dt by n days */
     date_t *dt;
     long int n;
{
  *dt = abs2greg (greg2abs (*dt) + n);
}


int 
dayOfWeek (d1)			/* sunday = 0 */
     date_t d1;
{
  return (int) (greg2abs (d1) % 7L);
}

void 
setDate (d)
     date_t *d;
{
/*
   asctime() converts a time value contained in a tm  structure
   to a 26-character string of the form:
   Sun Sep 16 01:03:52 1973\n\0
   Each field  has  a  constant  width.   asctime()  returns  a
   pointer to the string.
 */

  time_t time ();
  char *ctime PROTO ((const time_t *));

  time_t secs = time (NULL);
  char *timestr = ctime (&secs);

/* portability has driven me to truly shameful code.  
   please forgive this.
 */
  sscanf (timestr + 20, "%d", &d->yy);
  d->mm = lookup_string (timestr + 4, eMonths, 13, 3);
  sscanf (timestr + 8, "%d", &d->dd);
}


long 
day_on_or_before (day_of_week, date)
     int day_of_week;
     long date;
/* Returns the absolute date of the DAYNAME on or before absolute DATE.
 * DAYNAME=0 means Sunday, DAYNAME=1 means Monday, and so on.


 * Note: Applying this function to d+6 gives us the DAYNAME on or after an
 * absolute day d.  Similarly, applying it to d+3 gives the DAYNAME nearest to
 * absolute date d, applying it to d-1 gives the DAYNAME previous to absolute
 * date d, and applying it to d+7 gives the DAYNAME following absolute date d.

 */
{
  return date - ((date - (long) day_of_week) % 7L);
}



/* (defun calendar-nth-named-day (n dayname month year &optional day)
   "The date of Nth DAYNAME in MONTH, YEAR before/after optional DAY.
   A DAYNAME of 0 means Sunday, 1 means Monday, and so on.  If N<0,
   return the Nth DAYNAME before MONTH DAY, YEAR (inclusive).
   If N>0, return the Nth DAYNAME after MONTH DAY, YEAR (inclusive).

   If DAY is omitted, it defaults to 1 if N>0, and MONTH's last day otherwise.
   (calendar-gregorian-from-absolute
   (if (> n 0)
   (+ (* 7 (1- n))
   (calendar-dayname-on-or-before
   dayname
   (+ 6 (calendar-absolute-from-gregorian
   (list month (or day 1) year)))))
   (+ (* 7 (1+ n))
   (calendar-dayname-on-or-before
   dayname
   (calendar-absolute-from-gregorian
   (list month
   (or day (calendar-last-day-of-month month year))
   year)))))))
 */

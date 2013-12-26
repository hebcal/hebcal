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

#include <stdlib.h>
#include <ctype.h>
#include "common.h"
#include "greg.h"
#include "hebcal.h"

static long int hebrew_elapsed_days(int year);

hmonths_t hMonths =
{
    {
        {{"VOID",NULL,"VOID"}},
        {{"Nisan",NULL,"\360\351\361\357"}},
        {{"Iyyar",NULL,"\340\351\351\370"}},
        {{"Sivan",NULL,"\361\351\345\357"}},
        {{"Tamuz",NULL,"\372\356\345\346"}},
        {{"Av",NULL,"\340\341"}},
        {{"Elul",NULL,"\340\354\345\354"}},
        {{"Tishrei",NULL,"\372\371\370\351"}},
        {{"Cheshvan",NULL,"\347\371\345\357"}},
        {{"Kislev",NULL,"\353\361\354\345"}},
        {{"Tevet",NULL,"\350\341\372"}},
        {{"Sh'vat",NULL,"\371\341\350"}},
        {{"Adar",NULL,"\340\343\370"}},
        {{"Nisan",NULL,"\360\351\361\357"}}
    },
    {
        {{"VOID",NULL,"VOID"}},
        {{"Nisan",NULL,"\360\351\361\357"}},
        {{"Iyyar",NULL,"\340\351\351\370"}},
        {{"Sivan",NULL,"\361\351\345\357"}},
        {{"Tamuz",NULL,"\372\356\345\346"}},
        {{"Av",NULL,"\340\341"}},
        {{"Elul",NULL,"\340\354\345\354"}},
        {{"Tishrei",NULL,"\372\371\370\351"}},
        {{"Cheshvan",NULL,"\347\371\345\357"}},
        {{"Kislev",NULL,"\353\361\354\345"}},
        {{"Tevet",NULL,"\350\341\372"}},
        {{"Sh'vat",NULL,"\371\341\350"}},
        {{"Adar I",NULL,"\340\343\370 \340'"}},
        {{"Adar II",NULL,"\340\343\370 \341'"}},
        {{"Nisan",NULL,"\360\351\361\357"}}
    }
};

int max_days_in_heb_month (int month, int year)
{
    if ( month == IYYAR ||
         month == TAMUZ || 
         month == ELUL ||
         month == TEVET || 
         month == ADAR_II ||
         (month == ADAR_I && !LEAP_YR_HEB( year )) ||
         (month == CHESHVAN && !long_cheshvan( year )) ||
         (month == KISLEV && short_kislev( year )))
        return 29;
    else
        return 30;
}

int lookup_hebrew_month( const char *s )
{
  /* the hebrew months are unique to their second letter
     N         nisan  (november?)
     I         iyyar
     E        Elul
     C        Cheshvan
     K        Kislev
     1        1Adar
     2        2Adar   
     Si Sh     sivan, Shvat
     Ta Ti Te Tamuz, Tishrei, Tevet
     Av Ad    Av, Adar
   */
    switch ( tolower( *s++ ))
    {
    case 'n':
        return (tolower (*s) == 'o') ?	/* this catches "november" */
            0
            : NISAN;
    case 'i':
        return IYYAR;
    case 'e':
        return ELUL;
    case 'c':
        return CHESHVAN;
    case 'k':
        return KISLEV;
    case 's':
        switch (tolower (*s))
        {
        case 'i':
            return SIVAN;
        case 'h':
            return SHVAT;
        default:
            return 0;
        }
    case 't':
        switch (tolower (*s))
        {
        case 'a':
            return TAMUZ;
        case 'i':
            return TISHREI;
        case 'e':
            return TEVET;
        default:
            return 0;
        }
    case 'a':
        switch (tolower (*s))
        {
        case 'v':
            return AV;
        case 'd':
            for (; *s; s++)
            {			/* scan for a 1. */
                if (*s == '1')
                    return ADAR_I;
            }
            return ADAR_II;	/* else assume sheni */
        default:
            return 0;
        }
    default:
        return 0;
    }
}




/* 
   returns day of week, hours and chalakim of specified molad.
 */
molad_t get_molad( int year, int month)
{
    molad_t retMolad;
    
    long yearl, m_elapsed, p_elapsed, h_elapsed, parts, m_adj;

    m_adj = (long) month;
    m_adj -= 7;
    if (m_adj < 0) m_adj += MONTHS_IN_HEB(year);
    
    yearl = (long) year;
    m_elapsed = m_adj +
        235L * ((yearl - 1L) / 19L) +
    12L * ((yearl - 1L) % 19) +
        ((((yearl - 1L) % 19L) * 7) + 1L) / 19L;

    p_elapsed = 204L + (793L * (m_elapsed % 1080));

    h_elapsed = 5L + (12L * m_elapsed) +
        793L * (m_elapsed / 1080L) +
        p_elapsed / 1080L -
        6L;
    
    parts = (p_elapsed % 1080) + 1080 * (h_elapsed % 24);

    retMolad.day = 1L + 29L * m_elapsed + h_elapsed / 24L;
    retMolad.hour = (int) (h_elapsed % 24L);
    retMolad.chalakim = (int) (parts % 1080L);
    
    return retMolad;

}


date_t abs2hebrew( long d )
{
    static int mmap[] =
        {
            KISLEV, TEVET, SHVAT, ADAR_I, NISAN, 
            IYYAR, SIVAN, TAMUZ, 
            TISHREI, TISHREI, TISHREI, CHESHVAN
        };
    date_t hebdate, gregdate;
    int day, month, year;

    if( d >= 10555144L )
    {
       char buf[40];
       sprintf(buf, "%ld", d);
       die("parameter to abs2hebrew %s out of range", buf);
    }
    
    gregdate = abs2greg (d);
    hebdate.dd = 1;
    hebdate.mm = 7;
    year = 3760 + gregdate.yy;
    
    while (hebdate.yy = year + 1,
           d >= hebrew2abs (hebdate))
        year++;

    if( year >= 4635 && year < 10666  )
    {
        /* optimize search */
        month = mmap[gregdate.mm - 1];
    }
    else
    {
        /* we're outside the usual range, so assume nothing about hebrew/gregorian calendar drift... */
        month = TISHREI;
    }

    while (hebdate.mm = month,
           hebdate.dd = max_days_in_heb_month (month, year),
           hebdate.yy = year,
           d > hebrew2abs (hebdate))
        month = (month % MONTHS_IN_HEB (year)) + 1;

    hebdate.dd = 1;
    
    day = (int) (d - hebrew2abs (hebdate) + 1L);
    if( day < 0)
    {
       char buf[100];
       sprintf(buf, "assertion failure d < hebrew2abs(m,d,y) => %ld < %ld!\n", 
                d, hebrew2abs(hebdate));
       die(buf, NULL);
    }

    hebdate.dd = day;
    
    return hebdate;
}


/* Days from sunday prior to start of hebrew calendar to mean
   conjunction of tishrei in hebrew YEAR 
 */
static long int hebrew_elapsed_days (int year)
{
    long int yearl, m_elapsed, p_elapsed, h_elapsed, parts, day, alt_day;
    
    yearl = (long) year;
    m_elapsed = 235L * ((yearl - 1L) / 19L) +
        12L * ((yearl - 1L) % 19L) +
        ((((yearl - 1L) % 19L) * 7L) + 1L) / 19L;
    
    p_elapsed = 204L + (793L * (m_elapsed % 1080L));
  
    h_elapsed = 5L + (12L * m_elapsed) +
        793L * (m_elapsed / 1080L) +
        p_elapsed / 1080L;
    
    parts = (p_elapsed % 1080L) + 1080L * (h_elapsed % 24L);
    
    day = 1L + 29L * m_elapsed + h_elapsed / 24L;
    
    if ((parts >= 19440L) ||
        ((2L == (day % 7L)) && (parts >= 9924L) && !(LEAP_YR_HEB (year))) ||
        ((1L == (day % 7L)) && (parts >= 16789L) && LEAP_YR_HEB (year - 1)))
        alt_day = day + 1L;
    else
        alt_day = day;
    
    if ((alt_day % 7L) == 0L ||
        (alt_day % 7L) == 3L ||
        (alt_day % 7L) == 5L)
        return alt_day + 1L;
    else
        return alt_day;
}



/* convert hebrew date to absolute date */
/*Absolute date of Hebrew DATE.
   The absolute date is the number of days elapsed since the (imaginary)
   Gregorian date Sunday, December 31, 1 BC. */
long hebrew2abs (date_t d)
{
    int m;
    long tempabs = (long) d.dd;
    long ret;
    
    if (d.mm < TISHREI)
    {
        for (m = TISHREI; m <= MONTHS_IN_HEB (d.yy); m++)
            tempabs += (long) max_days_in_heb_month (m, d.yy);

        for (m = NISAN; m < d.mm; m++)
            tempabs += (long) max_days_in_heb_month (m, d.yy);
    }
    else
    {
        for (m = TISHREI; m < d.mm; m++)
            tempabs += (long) max_days_in_heb_month (m, d.yy);
    }
    
    
    ret = hebrew_elapsed_days (d.yy) - 1373429L + tempabs;
    return ret;
}

/* Number of days in the hebrew YEAR */
int days_in_heb_year( int year )
{
    return (int) (hebrew_elapsed_days (year + 1) - hebrew_elapsed_days (year));
}

/* true if Cheshvan is long in hebrew YEAR */
int long_cheshvan( int year ) 
{
    return ((days_in_heb_year (year) % 10) == 5);
}

/* true if Cheshvan is long in hebrew YEAR */
int short_kislev( int year ) 
{
    return ((days_in_heb_year (year) % 10) == 3);
}

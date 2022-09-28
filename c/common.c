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
#include <string.h>
#include "common.h"
#include "greg.h"
#include "hebcal.h"

hmonths_t hMonths =
{
    {
        {"VOID"},
        {"Nisan"},
        {"Iyyar"},
        {"Sivan"},
        {"Tamuz"},
        {"Av"},
        {"Elul"},
        {"Tishrei"},
        {"Cheshvan"},
        {"Kislev"},
        {"Tevet"},
        {"Sh'vat"},
        {"Adar"},
        {"Nisan"}
    },
    {
        {"VOID"},
        {"Nisan"},
        {"Iyyar"},
        {"Sivan"},
        {"Tamuz"},
        {"Av"},
        {"Elul"},
        {"Tishrei"},
        {"Cheshvan"},
        {"Kislev"},
        {"Tevet"},
        {"Sh'vat"},
        {"Adar I"},
        {"Adar II"},
        {"Nisan"}
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
        fprintf(stderr, "parameter to abs2hebrew  %ld out of range\n",
                d );
        exit(1);
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
        fprintf(stderr, "assertion failure d < hebrew2abs(m,d,y) => %ld < %ld!\n",
                d, hebrew2abs(hebdate));
        exit(1);
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

/* true if Kislev is short in hebrew YEAR */
int short_kislev( int year )
{
    return ((days_in_heb_year (year) % 10) == 3);
}

const char *num2heb(int num) {
    switch(num) {
    case 1: return "א";
    case 2: return "ב";
    case 3: return "ג";
    case 4: return "ד";
    case 5: return "ה";
    case 6: return "ו";
    case 7: return "ז";
    case 8: return "ח";
    case 9: return "ט";
    case 10: return "י";
    case 20: return "כ";
    case 30: return "ל";
    case 40: return "מ";
    case 50: return "נ";
    case 60: return "ס";
    case 70: return "ע";
    case 80: return "פ";
    case 90: return "צ";
    case 100: return "ק";
    case 200: return "ר";
    case 300: return "ש";
    case 400: return "ת";
    default: return "*INVALID*";
    }
}

#define GERESH "׳"
#define GERSHAYIM "״"

char *hebnum_to_string(char *buffer, int num) {
    int nums[10];
    int digits = 0;
    num = num % 1000;
    while (num > 0) {
        int i, incr = 100;
        if (num == 15 || num == 16) {
            nums[digits++] = 9;
            nums[digits++] = num - 9;
            break;
        }
        for (i = 400; i > num; i -= incr) {
            if (i == incr) {
                incr = incr / 10;
            }
        }
        nums[digits++] = i;
        num -= i;
    }
    buffer[0] = '\0';
    if (digits == 1) {
        const char *s = num2heb(nums[0]);
        strcat(buffer, s);
        strcat(buffer, GERESH);
        return buffer;
    }
    for (int i = 0; i < digits; i++) {
        const char *s = num2heb(nums[i]);
        if (i + 1 == digits) {
            strcat(buffer, GERSHAYIM);
        }
        strcat(buffer, s);
    }
    return buffer;
}
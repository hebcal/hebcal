/* Copyright(c) 1994  Danny Sadinoff
  See section COPYING for conditions for redistribution
*/

#include <stdbool.h>

#pragma once

#ifndef __GREG__
#define __GREG__

#define JAN 1
#define FEB 2
#define MAR 3
#define APR 4
#define MAY 5
#define JUN 6
#define JUL 7
#define AUG 8
#define SEP 9
#define OCT 10
#define NOV 11
#define DEC 12

#define SUN 0
#define MON 1
#define TUE 2
#define WED 3
#define THU 4
#define FRI 5
#define SAT 6

#define GREG_ADJUSTMENT_YEAR  1752              // we are setting the year of the gregorian adjustment to 1752
#define GREG_ADJUSTMENT_DAY_OF_YEAR  246        // September 2, 1752 was the 246th day of that year
#define GREG_ADJUSTMENT_ABSOLUTE_DATE   639796  // Sep 2, 1752 was day 639796th day when counting the number of days since the imaginary gregorian
                                                // date 12/31/1 (that day itself being 0) or the actual Julian date 1/2/1 (which are equivalent)

extern const char * eMonths[];
extern int MonthLengths[][13] ;
extern const char * DayNames[] ;
extern const char * ShortDayNames[];

typedef struct dateStruct {
    int mm;	/* months: jan=1, DEC = 12 */
    int dd;	/* day of month 1,31 */
    int yy;	/* years since year 1 BCE i.e. -1 = 2 BCE */
} date_t;

bool LEAP(int x);
int DAYS_IN(int x);

int getMonthLength(int year, int month);
int dayOfYear( date_t );
long greg2abs( date_t );
date_t abs2greg(long);
void decDate(date_t *, long);
void incDate(date_t *, long);
int dayOfWeek(date_t);
void setDate(date_t *);
long day_on_or_before(int,long);
#endif

/* Copyright(c) 1994  Danny Sadinoff
  See section COPYING for conditions for redistribution
*/
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

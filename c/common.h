/* Copyright(c) 1994  Danny Sadinoff
   Portions Copyright (c) 2002 Michael J. Radwin. All Rights Reserved.

   http://sourceforge.net/projects/hebcal

  See section COPYING for conditions for redistribution
*/
#ifndef __COMMON__
#define __COMMON__

#include "greg.h"

#define NISAN 1
#define IYYAR 2
#define SIVAN 3
#define TAMUZ 4
#define AV 5
#define ELUL 6
#define TISHREI 7
#define CHESHVAN 8
#define KISLEV 9
#define TEVET 10
#define SHVAT 11
#define ADAR_I 12
#define ADAR_II 13

typedef struct hmonth 
{
    const char *(name[3]);
} hmonth_t;

typedef hmonth_t hmonths_t [][15];

extern hmonths_t hMonths;

typedef struct molad {
    long int day;
    int hour, chalakim;
} molad_t;

molad_t molad(int, int);
date_t abs2hebrew(long);
long hebrew2abs(date_t);
int days_in_heb_year(int);
int long_cheshvan(int);
int short_kislev(int);
int max_days_in_heb_month(int,int);
int lookup_hebrew_month(const char *);
#endif

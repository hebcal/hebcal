/* Copyright(c) 1994  Danny Sadinoff
  See section COPYING for conditions for redistribution
*/
#ifndef __COMMON__
#define __COMMON__

#include "greg.h"
#include "proto.h"

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

typedef char * hmonths_t [][15];

extern hmonths_t hMonths;

typedef struct {
   long int day;
   int hour, chalakim;
   } molad_t;

molad_t molad PROTO((int, int));
date_t abs2hebrew PROTO((long));
long hebrew2abs PROTO((date_t));
int days_in_heb_year PROTO((int));
int long_cheshvan PROTO((int));
int short_kislev PROTO((int));
int max_days_in_heb_month PROTO((int,int));
int lookup_hebrew_month PROTO((char *));
#endif

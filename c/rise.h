/* Copyright(c) 1994  Danny Sadinoff
  See section COPYING for conditions for redistribution
*/

#ifndef __RISE__
#define __RISE__

#include "proto.h"

#define NORMAL     0
#define NO_SUNSET  1
#define NO_SUNRISE 2

#define SUNSET 0
#define CIVIL_TWILIGHT 1
#define NAUTICAL_TWILIGHT 2
#define ASTRONOMICAL_TWILIGHT 3
#define SUNRISE 4
#define DAWN 5
#define TAL_TEF_1 6
#define TAL_TEF_2 7

#define TWELVE 1
#define TWENTYFOUR 0

int suntime PROTO((double *, double *, int, int, int));
int one_time PROTO((int, double *, double, int, double, double, double));
char *timeadj PROTO((char *, double, int, int *));

#endif

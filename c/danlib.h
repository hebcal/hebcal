/* Copyright(c) 1994  Danny Sadinoff
  See section COPYING for conditions for redistribution
*/
#ifndef __DANLIB__
#define __DANLIB

#include "proto.h"
#include <stdio.h>

#define CHAR2NUM(x) ((x) - '0')
#define LAST_INDEX(x) (sizeof x / sizeof x[0])
#define strEQ(X,Y) (0 == strcmp((X),(Y)))

void initStr PROTO((char **, size_t));
int isAllNums PROTO((const char *));
const char *numSuffix PROTO((int));
char *hc_itoa PROTO((int));
int istrncasecmp PROTO((size_t,const char *,const char *));
int lookup_string PROTO((const char *str, const char *arr[], int size, int len));

#endif

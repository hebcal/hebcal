/*
  sanity checker.  determines whether any bugs  */

#include <stdio.h>
#include  <stdlib.h>
#include "common.h"
char * progname = "test-bug.cc";

/* returns 1 */
int reportIt(long d , date_t hebrew)
{
  date_t gregdate = abs2greg (d);

  printf ("%ld : [hm=%d hd=%d hy=%d], [gm=%d gd=%d gy=%d]\n",
	  d,
	  hebrew.mm,hebrew.dd,hebrew.yy,
	  gregdate.mm,gregdate.dd,gregdate.yy);
  return 1;
}


int main(int argc, char*argv[])
{
  long interval = 100000;
  long intervalCount = 0;
  int retVal =0;
  long d;

  setlinebuf(stdout);
  for (d =0 ; d < 1095363L; d++)
  {
    if( 0 == intervalCount-- )
      {
	printf("   %ld\n",d);
	intervalCount = interval-1;
      }
    date_t hebrew = abs2hebrew(d);
    if( hebrew.dd < 0 
	|| hebrew.mm < 0 
	|| hebrew.yy < 0  )
      retVal += reportIt(d,hebrew);
  }
  printf("found %d failure cases\n",retVal);
  exit( retVal );
    
}

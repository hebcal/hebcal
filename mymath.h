/* Copyright(c) 1994  Danny Sadinoff
  See section COPYING for conditions for redistribution
*/
/*LINTLIBRARY*/
#if defined(CRAY) || defined(vms)
  /* math.h is not wrapped on these machines */
#   ifndef __MATH__
#      define __MATH__
#      include <math.h>
#   endif
#else
#   include <math.h>
#endif


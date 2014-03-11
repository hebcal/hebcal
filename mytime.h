/* Copyright(c) 1994  Danny Sadinoff
  See section COPYING for conditions for redistribution
*/
#ifdef macII
# include <time.h>   /* on a Mac II we need this one as well */
#endif

#ifdef SYSV
# include <time.h>
#elif defined(vms)
# include <time.h>
#elif defined(CRAY)
# ifndef __TYPES__   /* it is not protected under CRAY */
#   define __TYPES__
#   include <sys/types.h>
# endif  /* ifndef _types_ */
# include <sys/time.h>
#else
# include <time.h>
#endif

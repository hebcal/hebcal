/* Copyright(c) 1994  Danny Sadinoff
  See section COPYING for conditions for redistribution
*/
#ifndef MYGETOPT_H
#define MYGETOPT_H
#include "proto.h"				/* define PROTO */

int Getopt PROTO((int, char **, char *,int));
extern char *Optarg;				/* current argv string */
extern int Optind;				/* current argv index */
extern int Optopt;				/* option character */
extern int Opterr;				/* getopt prints errors if 1 */
#endif /* MYGETOPT_H */

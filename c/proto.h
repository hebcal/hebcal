/* Copyright(c) 1994  Danny Sadinoff
  See section COPYING for conditions for redistribution
*/
#ifndef PROTO_H
#define PROTO_H
#if __STDC__==1 || PROTOTYPES_EXIST
#define PROTO(A) A
#else
#define PROTO(A) ()
#endif /*has prototypes */
#endif /*PROTO_H*/

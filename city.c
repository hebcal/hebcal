/* vi: set sw=2 ai sm et:
   Hebcal - A Jewish Calendar Generator
   Copyright (C) 1994-2006  Danny Sadinoff
   Portions Copyright (c) 2010 Michael J. Radwin. All Rights Reserved.

   https://github.com/hebcal/hebcal

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Danny Sadinoff can be reached at
   danny@sadinoff.com
 */

#include <stdlib.h>
#include "city.h"

/* This function compares two city names, treating all non-alphas as blanks */

/*********************************************************************/
/* Compares two city names and returns -1, 0, or 1 like stricmp.     */
/* The comparison is case insensitive and treats all non-alpha       */
/* characters as equivalent to blanks (i.e., "less than" alphas).    */
/* The function currently assumes both its input to be ASCII         */
/* (i.e., it does not yet support accented or non-Latin city names). */
/* If either string is NULL the function tries to return something   */
/* reasonable but the results are undefined.                         */
/*********************************************************************/
int compare_city(const char *a, const char *b)
{
  int result = 0;
  const char *p, *q;
  int current_p_is_alpha, current_q_is_alpha;
  int current_p_normalized, current_q_normalized;

  if (a != NULL && b != NULL)
  {
    for (p = a, q = b; result == 0 && *p && *q;)
    {
      current_p_is_alpha = isalpha((int) *p);
      current_q_is_alpha = isalpha((int) *q);
      if (current_p_is_alpha && current_q_is_alpha)
      {
        current_p_normalized = toupper((int) *p);
        current_q_normalized = toupper((int) *q);
        if (current_p_normalized < current_q_normalized)
          result = -1;
        else if (current_p_normalized > current_q_normalized)
          result = 1;
        p += 1;
        q += 1;
      }
      else
      {
        if (!current_p_is_alpha && current_q_is_alpha)
          result = -1;                    /* space < alpha */
        else if (current_p_is_alpha && !current_q_is_alpha)
          result = 1;                     /* alpha > space */
        /* Skip to either the next alpha or the end of string */
          while (*p && !isalpha((int) *p))
            p += 1;
          while (*q && !isalpha((int) *q))
            q += 1;
      }
    }
    /* 0 here means the start of the strings are identical, but the end can still differ */
    if (result)
      ;
    else if (!*p && *q)
      result = -1;                      /* nothing < something */
    else if (*p && !*q)
      result = 1;                       /* something > nothing */
  }
  else if (a == NULL && b != NULL && *b)
    result = -1;                        /* nothing < something */
  else if (a != NULL && *a && b == NULL)
    result = 1;                         /* something > nothing */
  return result;
}


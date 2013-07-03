/*
   Hebcal - A Jewish Calendar Generator
   Copyright (C) 1994-2004  Danny Sadinoff

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

/*
  a la perl
*/

#include "myerror.h"
#include <errno.h>
#include <stdlib.h>

extern char *progname;

/* print error message and die */
void die( const char *s1, const char *s2)			
{
    if (errno)
        perror (progname);
    else
        fprintf (stderr, "%s: ", progname);
    fprintf (stderr, s1, s2);
    fprintf (stderr, "\n");
    exit (1);
}

/* print error message but don't die */
void warn( const char *s1, const char *s2)			
{
    if (errno)
        perror (progname);
    else
        fprintf (stderr, "%s: ", progname);
    fprintf (stderr, s1, s2);
    fprintf (stderr, "\n");
    errno = 0;
}

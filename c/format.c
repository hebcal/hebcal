/*
   Hebcal - A Jewish Calendar Generator
   Copyright (C) 1994-2004  Danny Sadinoff
   Portions Copyright (c) 2002 Michael J. Radwin. All Rights Reserved.

   https://github.com/mjradwin/hebcal

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

/*-------------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>

#include "hebcal.h"
#include "danlib.h"
#include "format.h"


static void appendTo( char **buf, 
                      char **bufEnd, 
                      char **bufIter, 
                      const char *appendMe)
{
    size_t len = strlen(appendMe);
    size_t remainingBuf = *bufEnd - *bufIter;
    if( remainingBuf  <= len+1 )
    {                          
        size_t bufSize  = *bufEnd - *buf;
        size_t needed = bufSize + len + 5; /* fuzz */
        char * buf2 = calloc(needed,sizeof(char));
        size_t bufLen  =  *bufIter - *buf;

        if( ! buf2 )
            die( "couldn't reallocate enough memory for format",  "");
        
        memcpy( buf2, *buf, bufLen );
        
        *bufIter = buf2 + (bufIter -buf);
        *bufEnd  = buf2 + (bufEnd - buf);
        *buf     = buf2;
    }
    strncpy(*bufIter, appendMe, len );
    *bufIter += len;
}

/*-------------------------------------------------------------------------*/

char *formatLine( const date_t gregDate, 
                 const date_t hebDate, 
                 const char *text,
                 char *output,
                 size_t maxOutputLen
    )
{
    size_t formatLen = strlen(formatString);
    int allocLength = 200 + formatLen * 2;
    char autoBuf[allocLength];
    char *buf = autoBuf;
    char * bufEnd = buf + allocLength;
    char * bufIter = buf;
    char * formatEnd = formatString+ formatLen;
    const char *formatIter;

    if( ! buf )
        die( "unable to allocate %s bytes to format line", hc_itoa(strlen(formatString)*2));

    *buf = '\0';
    if( bufEnd > buf )
        *(bufEnd - 1) = '\0';

    for( formatIter = formatString; 
         formatIter < formatEnd && *formatIter; 
         formatIter++)
    {
        switch( *formatIter )
        {
        case '\\':
            formatIter++;
            if( formatIter >=formatEnd || ! *formatIter)
                break;
            switch( *formatIter )
            {
            case 'n':
                appendTo(&buf, &bufEnd, &bufIter, "\n");
                break;
            case 'b':
                appendTo(&buf, &bufEnd, &bufIter, "\b");
                break;
            case 'e':
                appendTo(&buf, &bufEnd, &bufIter, "\033");
                break;
            case 'f':
                appendTo(&buf, &bufEnd, &bufIter, "\f");
                break;
            case 'r':
                appendTo(&buf, &bufEnd, &bufIter, "\r");
                break;
            case 't':
                appendTo(&buf, &bufEnd, &bufIter, "\t");
                break;
            case '\\':
                appendTo(&buf, &bufEnd, &bufIter, "\\");
                break;
            }
            break;
        case '%':
            formatIter++;
            if( formatIter >=formatEnd || ! *formatIter)
                break;
            switch( *formatIter )
            {
            case '%':
                appendTo(&buf, &bufEnd, &bufIter, "%");
                break;
            case 'a':           /* abbreviated weekday name */
                appendTo(&buf, &bufEnd, &bufIter, ShortDayNames[dayOfWeek(gregDate)]);
                break;
            case 'Y':           /* year */
                appendTo(&buf, &bufEnd, &bufIter, hc_itoa(gregDate.yy));
                break;
            case 'm':           /* month */
                appendTo(&buf, &bufEnd, &bufIter, hc_itoa(gregDate.mm));
                break;
            case 'd':           /* day of month */
                appendTo(&buf, &bufEnd, &bufIter, hc_itoa(gregDate.dd));
                break;

            case 'Q': /* hebrew year */
                appendTo(&buf, &bufEnd, &bufIter, hc_itoa(hebDate.yy));
                break;
                
            case 'q': /* hebrew month */
                appendTo(&buf, &bufEnd, &bufIter, hc_itoa(hebDate.mm));
                break;
            case 'k': /* hebrew day */
                appendTo(&buf, &bufEnd, &bufIter, hc_itoa(hebDate.dd));
                break;
            case 'J': /* julian day number */
            {
                char numBuf[20];
                snprintf(numBuf, 20, "%ld",greg2abs(gregDate));
                numBuf[19] = '\0';
                appendTo(&buf, &bufEnd, &bufIter, numBuf);
                break;
            }
                
            case 't': /* holiday text */
                appendTo(&buf, &bufEnd, &bufIter, text);
                break;

/*             default: */
/*                 die ("unknown %% escape: %s",formatIter); */
            }
            break;
        default:
            *bufIter++ = *formatIter;
            break;
        }
    }
    *bufIter++ = '\0';

    strncpy( output, buf, maxOutputLen);

    output[maxOutputLen-1] ='\0';
    return output;
}

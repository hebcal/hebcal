/*
   Hebcal - A Jewish Calendar Generator
   Copyright (C) 1994  Danny Sadinoff

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



/** NOTE: need to recheck this */


/*
 *  These sunup/sundown routines were unceremoniously lifted from a program
 *  called SUN.C by Michael Schwartz.  I made a few changes to accommodate
 *  it to hebcal, but the calculation engine is completely unchanged.
 *
 *  NOTE:  I have found the numbers from this program to deviate from
 *   an accepted value by as much as three minutes.
 *
 *  The following comment is from his original code:
 */

/* Michael Schwarz writes:

 * Accuracy:  The USNO claims accuracy withing 2 minutes except at extreme
 *  northern or southern latitudes.  Comparison to local NWS charts for
 *  sunrise and sunset (which are cheap and easy to come by) shows that with
 *  the double precision calculations, the charts produced by this program 
 *  are no more than 1 minute removed from those charts in lattitudes lower
 *  than 41 degrees.  Candle lighting times agree with those on popular 
 *  calendars also to the 1 minute accuracy.
 *
 * Permission is granted to distribute and use this program as desired.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "mytime.h"
#include "mystring.h"
#include "mymath.h"
#include "rise.h"
#include "danlib.h"
#include "myerror.h"

#define TRUE  1
#define FALSE 0

int latdeg, latmin, longdeg, longmin, TZ;

int myclock = TWELVE;           /* Chooses a 12 hour clock */

/*
 * lambda = longitude
 * phi = latitude
 *
 * For 1978, M = .985600t -3.251 -- M is the Sun's mean anomaly
 * L = M + 1.916 SIN(M) + .020SIN(2M) + 282.565 -- L is Sun's TRUE long.
 * TANa = .91746 TAN L  -- a is Sun's right ascension
 * SIN DELTA = .39782 SIN L     -- Sun's declination
 * COS h = (COSz - SIN DELTA*SIN PHI)/(COS DELTA*COS PHI)
 * T = h + a - 0.065710t -6.620  -- h is local hour angle, a now in hours
 * UT = T + LAMBDA --UT is GMT.  LAMBDA is now in hours (deg / 15)
 *
 * Where Z is the zenith distance at phenomena in question. Samples:
 * 90,50'  regular sunrise/set  (atmospheric refraction + disk diameter)
 * 96      civil twilight
 * 102     nautical twilight
 * 106     astronomical twilight
 */


/* like I really care where PI is defined.  -djs*/
#ifdef PI               
#undef PI
#endif
#define PI 3.14159265358979323846

#define TODEC(a, b) ((double)a + (double)b/60.0)
#define DEGRAD (PI/180.0)
#define RADDEG (180.0/PI)
#define D 0.91746
#define E 0.39782
#define M(x) (0.985600*x - 3.251)
#define L(x) (x + 1.916*sin(DEGRAD*x) + 0.020*sin(2*DEGRAD*x) + 282.565)
#define ADJ(x) (-0.065710*x - 6.620)

/*
 *         --- This is where the REAL work is done ---
 * This routine will seem FORTRANy.  An attempt has been made to 
 * mirror the procedure outlined in the USGS publication; very little
 * optimization has been done, so that the connection between the
 * publication and the software will be extremely clear.
 * Thus, all angles are preserved in degrees (degrees are converted to
 * hours by the observation that 360 degrees = 24 hours -- so 15.0 
 * degrees are 1 hour).
 */

int suntime (double *sunrise, double *sunset,
             int day, int rise_opt, int set_opt)
{
    double lo_hr, lambda, phi;
    double cos_phi, sin_phi, cos_z;
    int retval = NORMAL;
    
    switch (set_opt)
    {
    case SUNSET:
        cos_z = cos (DEGRAD * TODEC (90, 50));
        break;
    case CIVIL_TWILIGHT:
        cos_z = cos (DEGRAD * TODEC (96, 0));
        break;
    case NAUTICAL_TWILIGHT:
        cos_z = cos (DEGRAD * TODEC (102, 0));
        break;
    case ASTRONOMICAL_TWILIGHT:
        cos_z = cos (DEGRAD * TODEC (106, 0));
        break;
    default:
        die("bad set_opt argument to suntime: %d", hc_itoa(set_opt));
        /* dead code to silence uninit warning in gcc */
        cos_z = NAN;
    }
    

    lambda = TODEC (longdeg, longmin);
    lo_hr = lambda / 15.0;
    phi = TODEC (latdeg, latmin);
    cos_phi = cos (DEGRAD * phi);
    sin_phi = sin (DEGRAD * phi);
    
    if (!one_time (day, sunset, lo_hr, FALSE, cos_z, cos_phi, sin_phi))
        retval |= NO_SUNSET;
    
    
    switch (rise_opt)
    {
    case SUNRISE:
        cos_z = cos (DEGRAD * TODEC (90, 50));
        break;
    case DAWN:
        cos_z = cos (DEGRAD * TODEC (106, 6));
        break;
    case TAL_TEF_1:
        cos_z = cos (DEGRAD * TODEC (101, 0));
        break;
    case TAL_TEF_2:
        cos_z = cos (DEGRAD * TODEC (100, 12));
        break;
    default:
        die("bad rise_opt to suntime: %d",hc_itoa(rise_opt));
    }
    
    if (!one_time (day, sunrise, lo_hr, TRUE, cos_z, cos_phi, sin_phi))
        retval |= NO_SUNRISE;
    
    return retval;
}



int one_time( int day, 
              double *tval, 
              double lo_hr, 
              int is_rise,
              double cos_z, 
              double cos_phi, 
              double sin_phi)
{
    double t, xm, xl, a, a_hr, sin_del, cos_del, h, h_hr;
    
    t = (double) day;
    if (is_rise)
        t += (18.0 + lo_hr) / 24.0;
    else
        t += (6.0 + lo_hr) / 24.0;
    
    xm = M (t);
    xl = L (xm);
    a = RADDEG * atan (D * tan (DEGRAD * xl));
    
    if (fabs (a + 360.0 - xl) > 90.0)
        a += 180.0;
    if (a > 360.0)
        a -= 360.0;
    
    a_hr = a / 15.0;
    sin_del = E * sin (DEGRAD * xl);
    cos_del = sqrt (1.0 - sin_del * sin_del);     /* cos delta must ALWAYS be >0 */
    h = (cos_z - sin_del * sin_phi) / (cos_del * cos_phi);
    
    if (fabs (h) > 1.0)
        return FALSE;
    
    h = RADDEG * acos (h);
    
    if (is_rise)
        h = 360.0 - h;              /* Puts sunrise in correct quadrant */
    h_hr = h / 15.0;
    *tval = h_hr + a_hr + ADJ (t) + lo_hr + TZ;
    
    return TRUE;
}


/* returns the string prefixStr followed by the time indicated by
   tval + minadj.  tval in hours and minadj in minutes. 

   The caller is responsible to free the string

*/
char * timeadj( char *prefixStr, double tval, int minadj, int *dayadj)
{
    char *str;
    int hour, min;
    size_t num = strlen (prefixStr) + 9;
    
    *dayadj = 0;
    tval += (double) minadj / 60.0;
    if (tval < 0.0)
    {
        tval += 24.0;
        *dayadj -= 1;
    }
    hour = (int) tval;            /* Type conversion causes truncation */
    min = (int) ((tval - (double) hour) * 60.0 + 0.5);
    if (min >= 60)
    {
        hour += 1;
        min -= 60;
    }
    if (hour > 24)
    {
        hour -= 24;
        *dayadj += 1;
    }
    if (myclock == TWELVE)        /* Check for 12 hour clock */
        if (hour > 12)
            hour -= 12;
    
    str = (char *) calloc ((unsigned) num, sizeof (char));
    sprintf (str, "%s%2d:%02d", prefixStr, hour, min);
    return str;
}


# #
#    Hebcal - A Jewish Calendar Generator
#    Copyright (C) 1994  Danny Sadinoff

#    This program is free software; you can redistribute it and/or
#    modify it under the terms of the GNU General Public License
#    as published by the Free Software Foundation; either version 2
#    of the License, or (at your option) any later version.

#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.

#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#    Danny Sadinoff can be reached at 
#    1 Cove La.
#    Great Neck, NY
#    11024

#    sadinoff@pobox.com 
#  */

# /*
#  *  These sunup/sundown routines were unceremoniously lifted from a program
#  *  called SUN.C by Michael Schwartz.  I made a few changes to accommodate
#  *  it to hebcal, but the calculation engine is completely unchanged.
#  *
#  *  NOTE:  I have found the numbers from this program to deviate from
#  *   an accepted value by as much as three minutes.
#  *
#  *  The following comment is from his original code:
#  */

# /* Michael Schwarz writes:

#  * Accuracy:  The USNO claims accuracy withing 2 minutes except at extreme
#  *  northern or southern latitudes.  Comparison to local NWS charts for
#  *  sunrise and sunset (which are cheap and easy to come by) shows that with
#  *  the double precision calculations, the charts produced by this program 
#  *  are no more than 1 minute removed from those charts in lattitudes lower
#  *  than 41 degrees.  Candle lighting times agree with those on popular 
#  *  calendars also to the 1 minute accuracy.
#  *
#  * Permission is granted to distribute and use this program as desired.
#  *
#  */

package Hebcal;
use strict;
use vars qw( $NORMAL $NO_SUNSET $NO_SUNRISE $SUNSET $CIVIL_TWILIGHT
	    $NAUTICAL_TWILIGHT $ASTRONOMICAL_TWILIGHT $SUNRISE $DAWN $TAL_TEF_1
	    $TAL_TEF_2 $TWELVE $TWENTYFOUR );

$NORMAL     = 0;
$NO_SUNSET  = 1;
$NO_SUNRISE = 2;

$SUNSET = 0;
$CIVIL_TWILIGHT = 1;
$NAUTICAL_TWILIGHT = 2;
$ASTRONOMICAL_TWILIGHT = 3;
$SUNRISE = 4;
$DAWN = 5;
$TAL_TEF_1 = 6;
$TAL_TEF_2 = 7;

$TWELVE = 1;
$TWENTYFOUR = 0;



my $myclock = $TWELVE;		# Chooses a 12 hour clock */

# /*
#  * lambda = longitude
#  * phi = latitude
#  *
#  * For 1978, M = .985600t -3.251 -- M is the Sun's mean anomaly
#  * L = M + 1.916 SIN(M) + .020SIN(2M) + 282.565 -- L is Sun's TRUE long.
#  * TANa = .91746 TAN L  -- a is Sun's right ascension
#  * SIN DELTA = .39782 SIN L     -- Sun's declination
#  * COS h = (COSz - SIN DELTA*SIN PHI)/(COS DELTA*COS PHI)
#  * T = h + a - 0.065710t -6.620  -- h is local hour angle, a now in hours
#  * UT = T + LAMBDA --UT is GMT.  LAMBDA is now in hours (deg / 15)
#  *
#  * Where Z is the zenith distance at phenomena in question. Samples:
#  * 90,50'  regular sunrise/set  (atmospheric refraction + disk diameter)
#  * 96      civil twilight
#  * 102     nautical twilight
#  * 106     astronomical twilight
#  */


# /* like I really care where PI is defined.  -djs*/
# #ifdef PI		
# #undef PI
# #endif

use Math::Complex qw(:trig);
use Math::Trig qw(acos pi);
sub TODEC($$){ $_[0]+  $_[1]/60.0 } 
my  $DEGRAD =  Math::Trig::pi/180.0;
my  $RADDEG =  180.0/Math::Trig::pi;
my  $D =  0.91746;
my  $E =  0.39782;
sub M($ )  {  0.985600*$_[0] - 3.251 }
sub L($ )  { $_[0] + 1.916*sin($DEGRAD*$_[0]) + 0.020*sin(2*$DEGRAD*$_[0]) + 282.565 }
sub ADJ($ ){-0.065710*$_[0] - 6.620}

# /*
#  *         --- This is where the REAL work is done ---
#  * This routine will seem FORTRANy.  An attempt has been made to 
#  * mirror the procedure outlined in the USGS publication; very little
#  * optimization has been done, so that the connection between the
#  * publication and the software will be extremely clear.
#  * Thus, all angles are preserved in degrees (degrees are converted to
#  * hours by the observation that 360 degrees = 24 hours -- so 15.0 
#  * degrees are 1 hour).
#  */
#returns (sunrise, sunset)
sub suntime($$$$){

    my ($location,$day, $rise_opt, $set_opt) = @_;
    my ($latdeg, $latmin, $longdeg, $longmin, $TZ) = @$location;

    my $cos_z;
    if ( $set_opt  ==  $SUNSET){
	$cos_z = cos($DEGRAD * TODEC (90, 50));
    }
    elsif($set_opt == $CIVIL_TWILIGHT ){
	$cos_z = cos($DEGRAD * TODEC (96, 0));
    }
    elsif($set_opt == $NAUTICAL_TWILIGHT){
	$cos_z = cos($DEGRAD * TODEC (102, 0));
    }
    elsif( $set_opt == $ASTRONOMICAL_TWILIGHT){
	$cos_z = cos($DEGRAD * TODEC (106, 0));
    }


    my $lambda = TODEC ($longdeg, $longmin);
    my $lo_hr = $lambda / 15.0;
    my $phi = TODEC ($latdeg, $latmin);
    my $cos_phi = cos ($DEGRAD * $phi);
    my $sin_phi = sin ($DEGRAD * $phi);
    
    my $sunset = one_time ($day,$TZ,$lo_hr, 0, $cos_z, $cos_phi, $sin_phi);
  SWTICH:{
       $rise_opt == $SUNRISE and do {
	  $cos_z = cos ($DEGRAD * TODEC (90, 50));
	  last };
       $rise_opt == $DAWN and do { 
	  $cos_z = cos ($DEGRAD * TODEC (106, 6));
	  last };
       $rise_opt == $TAL_TEF_1 and do{
	  $cos_z = cos ($DEGRAD * TODEC (101, 0));
	  last };
       $rise_opt == $TAL_TEF_2 and do{
	  $cos_z = cos ($DEGRAD * TODEC (100, 12));
	  last };
    }
    
    my $sunrise = one_time ($day,$TZ, $lo_hr, 1, $cos_z, $cos_phi, $sin_phi);
    return ( $sunrise, $sunset);
 }



sub one_time ($$$$$$$)
{
   my ($day,$TZ, $lo_hr, $is_rise, $cos_z, $cos_phi, $sin_phi) = @_;


   my $t = $day;
   if ($is_rise){
      $t += (18.0 + $lo_hr) / 24.0;
   }
   else{
      $t += (6.0 + $lo_hr) / 24.0;
   }
   
  my $xm = M ($t);
  my $xl = L ($xm);
  my $a = $RADDEG * atan ($D * tan ($DEGRAD * $xl));

   if (abs ($a + 360.0 - $xl) > 90.0){
      $a += 180.0;
   }
   if ($a > 360.0){
      $a -= 360.0;
   }

   my $a_hr = $a / 15.0;
   my $sin_del = $E * sin ($DEGRAD * $xl);
   my $cos_del = sqrt (1.0 - $sin_del * $sin_del); #cos delta must ALWAYS be >0 */
   my $h = ($cos_z - $sin_del * $sin_phi) / ($cos_del * $cos_phi);
   
   if (abs ($h) > 1.0){
      return undef;
   }
   
   $h = $RADDEG * acos ($h);
   $h = 360.0 - $h		# Puts sunrise in correct quadrant */
      if ($is_rise);
   my $h_hr = $h / 15.0;
   return $h_hr + $a_hr + ADJ($t) + $lo_hr + $TZ;
}

# returns the string prefixStr followed by the time indicated by
#    tval + minadj.  tval in hours and minadj in minutes. */
sub timeadj ($$$$)
#      char *prefixStr;
#      double tval;
#      int minadj, *dayadj;
{
   my ($prefixStr, $tval, $minadj, $dayadj)= @_;


   $dayadj = 0;
   $tval += $minadj / 60.0;
   if ($tval < 0.0) {
      $tval += 24.0;
      $dayadj -= 1;
   }
   my $hour = int($tval);		# Type conversion causes truncation */
   my $min = int( ($tval - $hour) * 60.0 + 0.5);
   if ($min >= 60) {
      $hour += 1;
      $min -= 60;
   }
   if($hour > 24) {
      $hour -= 24;
      $dayadj += 1;
   }
   if ($myclock == $TWELVE){	# Check for 12 hour clock */
     $hour -= 12
       if $hour > 12;
     $hour = 12 
       if $hour == 0;
   }
   
   $_[3] = $dayadj; 
   
   return sprintf ( "%s%2d:%02d", $prefixStr, $hour, $min);

}

1;


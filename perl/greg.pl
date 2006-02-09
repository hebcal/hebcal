#     $Header$
#     Hebcal - A Jewish Calendar Generator
#     Copyright (C) 1994-1998  Danny Sadinoff

#     This program is free software; you can redistribute it and/or
#     modify it under the terms of the GNU General Public License
#     as published by the Free Software Foundation; either version 2
#     of the License, or (at your option) any later version.

#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.

#     You should have received a copy of the GNU General Public License
#     along with this program; if not, write to the Free Software
#     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#     Danny Sadinoff can be reached at 
#     1 Cove La.
#     Great Neck, NY
#     11024

#     sadinoff@pobox.com
#     http://pobox.com/~sadinoff

#routines for handling gregorian dates
use Date;

package Hebcal;
require 'common.ph';
require 'greg.ph';
my @eMonths  = qw( UNUSED January February March April May June July
		   August September October November December
		 );

my $MonthLengths= [
		   [0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31],
		   [0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
		  ];


sub LEAP( $ ){
#    my $yr = shift;
   return (( 0 == $_[0] % 4) 
	   && ( (0 != $_[0] % 100) 
		|| (0 == $_[0] % 400)));
}
sub DAYS_IN( $ ){
   LEAP( $_[0] ) ? 366 : 365;
}

sub DAYS_IN_MONTH( $$ ){
   my ($month, $year) = @_;
   return $MonthLengths->[LEAP ($year)+0][$month]   
}


# /*
#  *Return the day number within the year of the date DATE.
#  *For example, dayOfYear({1,1,1987}) returns the value 1
#  *while dayOfYear({12,31,1980}) returns 366.
#  
#int 
sub dayOfYear( $ ) {
   my $d = shift;
   my $dOY = $d->{dd} + 31 * ($d->{mm} - 1);
   if ($d->{mm} > $Hebcal::FEB) {
      $dOY -= int((4 * $d->{mm} + 23) / 10);
      if (LEAP( $d->{yy} )){
	 $dOY++;
      }
   }
   return $dOY;
}


# /*
#  * The number of days elapsed between the Gregorian date 12/31/1 BC and DATE.
#  * The Gregorian date Sunday, December 31, 1 BC is imaginary.
#  
sub greg2abs( $ )#			/* "absolute date" 
{
 my $d = shift;
 return ( dayOfYear($d)	#/* days this year 
	  + 365 *  ($d->{yy} - 1)#	/* + days in prior years 
	  +  int(($d->{yy} - 1) / 4)#	/* + Julian Leap years 
	  -  int(($d->{yy} - 1) / 100)#	/* - century years 
	  +  int(($d->{yy} - 1) / 400));#	/* + Gregorian leap years 
}
# /*
#  * See the footnote on page 384 of ``Calendrical Calculations, Part II:
#  * Three Historical Calendars'' by E. M. Reingold,  N. Dershowitz, and S. M.
#  * Clamen, Software--Practice and Experience, Volume 23, Number 4
#  * (April, 1993), pages 383-404 for an explanation.
#  

sub abs2greg ($)
{
   my $theDate = shift;
#     day, year, month, mlen;
#    date_t d;
#    long int d0, n400, d1, n100, d2, n4, d3, n1;
   
   my $d0 = $theDate - 1;
   my $n400 = int($d0 / 146097);
   my $d1 = $d0 % 146097;
   my $n100 = int($d1 / 36524);
   my $d2 = $d1 % 36524;
   my $n4 = int ($d2 / 1461);
   my $d3 = $d2 % 1461;
   my $n1 = int ($d3 / 365);

   my $day =  ($d3 % 365) + 1;
   my $year =  (400 * $n400 + 100 * $n100 + 4 * $n4 + $n1);
   
   my $d = new Date();
   if (4 == $n100 || 4 == $n1) {
      $d->{mm} = 12;
      $d->{dd} = 31;
      $d->{yy} = $year;
      return $d;
   }
   else {
      $year++;
      my $month = 1;
      my $mlen;

      while (($mlen = $MonthLengths->[LEAP ($year)+0][$month]) < $day) {
	 $day -= $mlen;
	 $month++;
      }
      $d->{yy} = $year;
      $d->{mm} = $month;
      $d->{dd} = $day;
      return $d;
   }
}


# sub incGregDate($$)
# #      date_t *dt;
# #      long int n;
# {
#    $_[0] = abs2greg (greg2abs ($_[0]) + $_[1]);
# }


# int 
sub dayOfWeek ($ ){#			/* sunday = 0 
   #      date_t d1;
   return (greg2abs(shift) % 7);
}
	       
sub setDate ($ ){
#    date_t *d;
   my $d = shift;
   my($day, $mon, $yr) = (localtime)[3,4,5];
   $yr+=1900;
   $mon++;
   $d->{dd} = $day;
   $d->{mm} = $mon;
   $d->{yy} = $yr;
}


sub day_on_or_before ($$){
   my ($day_of_week, $date) = @_;
# /* Returns the absolute date of the DAYNAME on or before absolute DATE.
#  * DAYNAME=0 means Sunday, DAYNAME=1 means Monday, and so on.

#  * Note: Applying this function to d+6 gives us the DAYNAME on or after an
#  * absolute day d.  Similarly, applying it to d+3 gives the DAYNAME nearest to
#  * absolute date d, applying it to d-1 gives the DAYNAME previous to absolute
#  * date d, and applying it to d+7 gives the DAYNAME following absolute date d.
#  
   return $date - (($date -  $day_of_week) % 7);
}


# /* (defun calendar-nth-named-day (n dayname month year &optional day)
#    "The date of Nth DAYNAME in MONTH, YEAR before/after optional DAY.
#    A DAYNAME of 0 means Sunday, 1 means Monday, and so on.  If N<0,
#    return the Nth DAYNAME before MONTH DAY, YEAR (inclusive).
#    If N>0, return the Nth DAYNAME after MONTH DAY, YEAR (inclusive).

#    If DAY is omitted, it defaults to 1 if N>0, and MONTH's last day otherwise.
#    (calendar-gregorian-from-absolute
#    (if (> n 0)
#    (+ (* 7 (1- n))
#    (calendar-dayname-on-or-before
#    dayname
#    (+ 6 (calendar-absolute-from-gregorian
#    (list month (or day 1) year)))))
#    (+ (* 7 (1+ n))
#    (calendar-dayname-on-or-before
#    dayname
#    (calendar-absolute-from-gregorian
#    (list month
#    (or day (calendar-last-day-of-month month year))
#    year)))))))
#  



################ from $hebcal->{c} 
# /* end-of-loop increment routine 
# sub incHebGregDate($){
#    my $iter = shift;
#    $iter->{todayHeb} = nextHebDate ($iter->{todayHeb});
#    $iter->{todayAbs}++;
#    $iter->{todayGreg} = abs2greg($iter->{todayAbs});
#    $iter->{day_of_week} = ($iter->{day_of_week} + 1) % 7;
   
#    if ($Hebcal::TISHREI == $iter->{todayHeb}->{mm} &&   #	/* if Rosh Hashana then reset YearData 
#        1 == $iter->{todayHeb}->{dd}) {
#        $iter->{theYear} = yearData ($iter->{todayHeb}->{yy});
#        if ($switches->{'daily-sedrot'} || $switches->{'saturday-sedrot'}){
# 	   reset_sedra ($iter->{todayHeb}->{yy});
#        }
#        if ($switches->{printOmer}){
# 	   ($iter->{beginOmer}, $iter->{endOmer}) = getOmerBounds ($iter->{todayHeb}->{yy});
#        }
#        init_holidays($iter->{todayHeb}->{yy});
#    }
# }
sub numSuffix($ ){
   my $num = shift;
   if ( $num >9 && $num <20 ){
      return 'th';
   }
   my $rem = $num %10;
   $rem == 1 and return 'st';
   $rem == 2 and return 'nd';
   $rem == 3 and return 'rd';
   return 'th';
}

1;

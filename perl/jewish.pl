#    $Header$
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


#jewish calendar routines
package Hebcal;
# use YearData;
use Date;
sub GREGYR_2_HEBYR($) {
#    my $gregyr = shift;
   return $_[0]+ 3760
}
sub MONTHS_IN_HEB($){
#    my $hyear = shift;
   LEAP_YR_HEB($_[0]) ? 13 :12;
}

sub LEAP_YR_HEB($){
#    my $hebyear = shift;
   ((1 + $_[0]* 7) % 19 < 7 ?
    1 
    : 
    0);
} 

sub max_days_in_heb_month ($$)
{
   my ($month, $year) = @_;
   if ($month == $Hebcal::IYYAR || $month == $Hebcal::TAMUZ || $month == $Hebcal::ELUL ||
       $month == $Hebcal::TEVET || $month == $Hebcal::ADAR_II ||
       ($month == $Hebcal::ADAR_I && !LEAP_YR_HEB ($year)) ||
       ($month == $Hebcal::CHESHVAN && !long_cheshvan ($year)) ||
       ($month == $Hebcal::KISLEV && short_kislev ($year))){
     return 29;
  }
   else{
     return 30;
     }
}

sub lookup_hebrew_month ($)
{
   my $s = shift;
#   /* the hebrew months are unique to their second letter
#      N         nisan  (november?)
#      I         iyyar
#      E        Elul
#      C        Cheshvan
#      K        Kislev
#      1        1Adar
#      2        2Adar   
#      Si Sh     sivan, Shvat
#      Ta Ti Te Tamuz, Tishrei, Tevet
#      Av Ad    Av, Adar
#    
   
   for  ( lc substr ($s, 0,1 )) {
      /n/ and do {
	 if ( lc substr($s,1,1) eq 'o' ){ #november
	    return 0;
	 }
	 else{
	    return $NISAN;
	 }
      };
      /i/ and do { return $IYYAR } ;
      /e/ and do { return $ELUL };
      /c/ and do { return $CHESHVAN };
      /k/ and do { return $KISLEV };
      /s/ and do { 
	 for ( lc substr($s,1,1)){
	    /i/ and do {return $Hebcal::SIVAN};
	    /h/ and do {return $Hebcal::SHVAT};
	    return 0;
	 }
      };
      /t/ and do {
	 for( lc substr($s,1,1)){
	    /a/ and do { return $Hebcal::TAMUZ};
	    /i/ and do { return $Hebcal::TISHREI } ;
	    /e/ and do { return $Hebcal::TEVET } ;
	    return 0;
	 }};
      /a/ and do {
	 for( lc substr($s,1,1)){
	    /v/ and do { return $Hebcal::AV};
	    /d/ and do {
	       if( /1/ ){
		  return $Hebcal::ADAR_I;
	       }
	       else{
		  return $Hebcal::ADAR_II;
	       }
	    };
	    return 0;
	 }};
   }
   return 0;
}




# #if 0

# /* 
#    returns day of week, hours and chalakim of specified molad.
#  
# molad_t 
# molad (year, m)
#      int year, m;
# {
#   molad_t retMolad;

#   long yearl, m_elapsed, p_elapsed, h_elapsed, parts;

#   yearl = (long) year;
#   m_elapsed = (long) m +
#     235L * ((yearl - 1L) / 19L) +
#     12L * ((yearl - 1L) % 19) +
#     ((((yearl - 1L) % 19L) * 7) + 1L) / 19L;

#   p_elapsed = 204L + (793L * (m_elapsed % 1080));

#   h_elapsed = 5L + (12L * m_elapsed) +
#     793L * (m_elapsed / 1080L) +
#     p_elapsed / 1080L;

#   parts = (p_elapsed % 1080) + 1080 * (h_elapsed % 24);

#   retMolad.day = 1L + 29L * m_elapsed + h_elapsed / 24L;
#   retMolad.hour = (int) (h_elapsed % 24L);
#   retMolad.chalakim = (int) (parts % 1080L);

#   return retMolad;

# }
# #endif


sub abs2hebrew( $ ){
   my $d = shift;
   my @mmap = (9, 10, 11, 12, 1, 2, 3, 4, 7, 7, 7, 8);
   my $hebdate = new Date();

   my $gregdate = abs2greg($d);
   $hebdate->{dd} = 1;
   $hebdate->{mm} = 7;
   my $month = $mmap[$gregdate->{mm} - 1];
   my $year = 3760 + $gregdate->{yy};

   $hebdate->{yy} = $year + 1;
   while ( $d >= hebrew2abs($hebdate)){
     $year++;
     $hebdate->{yy} = $year + 1,
  }

  while ($hebdate->{mm} = $month,
	 $hebdate->{dd} = max_days_in_heb_month ($month,$year),
	 $hebdate->{yy} = $year,
	 $d > hebrew2abs ($hebdate)){
     $month = ($month % MONTHS_IN_HEB ($year)) + 1;
  }

  $hebdate->{dd} = 1;

  my $day = int($d - hebrew2abs($hebdate) + 1);
  $hebdate->{dd} = $day;

  return $hebdate;
}


# /* Days from sunday prior to start of hebrew calendar to mean
#    conjunction of tishrei in hebrew YEAR 
#  
sub hebrew_elapsed_days( $ ){
   my $year = shift;

   my $yearl = $year;
   my $m_elapsed = (235 * int (($yearl - 1) / 19) +
		    12 * (($yearl - 1) % 19) +
		    int((((($yearl - 1) % 19) * 7) + 1) / 19));
   
   my $p_elapsed = 204 + (793 * ($m_elapsed % 1080));
   
   my $h_elapsed = (5 + (12 * $m_elapsed) +
		    793 * int ($m_elapsed / 1080) +
		    int($p_elapsed / 1080));
   
   my $parts = ($p_elapsed % 1080) + 1080 * ($h_elapsed % 24);
   
   my $day = 1 + 29 * $m_elapsed + int($h_elapsed / 24);
   my $alt_day;

   if (($parts >= 19440) ||
       ((2 == ($day % 7)) && ($parts >= 9924) && !(LEAP_YR_HEB ($year))) ||
       ((1 == ($day % 7)) && ($parts >= 16789) && LEAP_YR_HEB ($year - 1))){
    $alt_day = $day + 1;}
   else{
     $alt_day = $day;}
   
   if (($alt_day % 7) == 0 ||
       ($alt_day % 7) == 3 ||
       ($alt_day % 7) == 5){
      return $alt_day + 1;
   }
   else{
     return $alt_day;
  }
}



# /* convert hebrew date to absolute date 
# /*Absolute date of Hebrew DATE.
#    The absolute date is the number of days elapsed since the (imaginary)
#    Gregorian date Sunday, December 31, 1 BC. 
sub hebrew2abs( $ ) {
   my $d = shift;
   my $m;
   my $tempabs = $d->{dd};


   # FIX: These loops want to be optimized with table-lookup
   if ($d->{mm} < $Hebcal::TISHREI) {
      for ($m = $Hebcal::TISHREI; $m <= MONTHS_IN_HEB($d->{yy}); $m++){
	 $tempabs +=  max_days_in_heb_month ($m, $d->{yy});
      }
      
      for ($m = $Hebcal::NISAN; $m < $d->{mm}; $m++){
	 $tempabs +=  max_days_in_heb_month ($m, $d->{yy});
      }
   }
   else {
      for ($m = $Hebcal::TISHREI; $m < $d->{mm}; $m++){
	 $tempabs +=  max_days_in_heb_month ($m, $d->{yy});
      }
   }
   my $days = hebrew_elapsed_days ($d->{yy}) - 1373429 + $tempabs;
#    croak Dumper($d)if $days < 0;
   return $days;
}

# /* Number of days in the hebrew YEAR 
sub days_in_heb_year( $ ){
   my $year = shift;
   return hebrew_elapsed_days ($year + 1) - hebrew_elapsed_days ($year);
}

# /* true if Cheshvan is long in hebrew YEAR 
sub long_cheshvan( $ ){
#    my $year = shift;
   return ((days_in_heb_year ($_[0]) % 10) == 5);
}

# /* true if Cheshvan is long in hebrew YEAR 
sub short_kislev( $ ){
#    my $year = shift;
   return ((days_in_heb_year ($_[0]) % 10) == 3);
}

# sub yearData( $ ){
#    my $hebyr = shift;
#    my $tempDate = new Date;
#    my YearData $retYear = new YearData;
   
#   $tempDate->{yy} = $hebyr;
#   $tempDate->{mm} = $Hebcal::TISHREI;
#   $tempDate->{dd} = 1;

#   $retYear->{first_day_of_week} = hebrew2abs($tempDate) % 7;
#   $retYear->{leap_p} = LEAP_YR_HEB ($hebyr);
#   return $retYear;
# }


sub nextHebDate( $ ){
   my $dth = new Date(shift);
   $dth->{dd}++;
   if ($dth->{dd} > max_days_in_heb_month ($dth->{mm}, $dth->{yy}))
     {
	if ($dth->{mm} == MONTHS_IN_HEB ($dth->{yy}))
	  {
	     $dth->{mm} = $Hebcal::NISAN;
	     $dth->{dd} = 1;
	  }
	else
	  {
	     $dth->{mm}++;
	     $dth->{dd} = 1;
	     if ($dth->{mm} == $Hebcal::TISHREI)
	       {
		  $dth->{yy}++;
	       }
	  }
     }
   return $dth;
}

1;

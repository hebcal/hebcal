#!/usr/local/bin/perl5 -w
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

use File::Basename;
use lib dirname($0);

package Hebcal;
use strict;

use Getopt::Long;
use Carp;

require 'common.ph';
require 'greg.ph';
use Date;
use Holiday;
use Sedrot;

use vars qw( $SUNRISE $SUNSET  );
use vars qw( $JAN $FEB $MAR $APR $MAY $JUN $JUL $AUG $SEP $OCT $NOV $DEC);
use vars qw( $SUN $MON $TUE $WED $THU $FRI $SAT );
use vars qw( $SIVAN $SHVAT $AV $IYYAR $ELUL $CHESHVAN $KISLEV $NISAN );
my $switches = {};
my $self = {};


my @ShortDayNames = qw(Sun Mon Tue Wed Thu Fri Sat);

use vars qw( @hMonths );
@hMonths =
(
  [
    "VOID", "Nisan", "Iyyar", "Sivan", "Tamuz", "Av", "Elul", "Tishrei",
    "Cheshvan", "Kislev", "Tevet", "Sh'vat", "Adar", "Nisan"
  ],
  [
    "VOID", "Nisan", "Iyyar", "Sivan", "Tamuz", "Av", "Elul", "Tishrei",
    "Cheshvan", "Kislev", "Tevet", "Sh'vat", "Adar I", "Adar II",
    "Nisan"
  ]
);
my %locations =
(
   "Atlanta" => [ 33, 45, 84, 23, -5],
   "Austin" => [ 30, 16, 97, 45, -6],
   "Berlin" => [ 52, 31, -13, -24, 1],
   "Baltimore" => [ 39, 17, 76, 36, -5],
   "Bogota" => [ 4, 36, 74, 5, -5],
   "Boston" => [ 42, 20, 71, 4, -5],
   "Buenos Aires" => [ -34, -37, 58, 24, -3],
   "Buffalo" => [ 42, 53, 78, 52, -5],
   "Chicago" => [ 41, 50, 87, 45, -6],
   "Cincinnati" => [ 39, 6, 84, 31, -5],
   "Cleveland" => [ 41, 30, 81, 41, -5],
   "Dallas" => [ 32, 47, 96, 48, -6],
   "Denver" => [ 39, 44, 104, 59, -7],
   "Detroit" => [ 42, 20, 83, 2, -5],
   "Gibraltar" => [ 36, 0, 5, 0, -10],
   "Hawaii" => [ 19, 30, 155, 30, -10],
   "Houston" => [ 29, 46, 95, 22, -6],
   "Jerusalem" => [ 31, 47, -35, -14, 2],
   "Johannesburg" => [ -28, -3, -26, -12, 1],
   "London" => [ 51, 30, 0, 10, 0],
   "Los Angeles" => [ 34, 4, 118, 15, -8],
   "Miami" => [ 25, 46, 80, 12, -5],
   "Mexico City" => [ 19, 24, 99, 9, -6],
   "New York" => [ 40, 43, 74, 1, -5],
   "Omaha" => [ 41, 16, 95, 56, -7],
   "Philadelphia" => [ 39, 57, 75, 10, -5],
   "Phoenix" => [ 33, 27, 112, 4, -7],
   "Pittsburgh" => [ 40, 26, 80, 0, -5],
   "Saint Louis" => [ 38, 38, 90, 12, -6],
   "San Francisco" => [ 37, 47, 122, 25, -8],
   "Seattle" => [ 47, 36, 122, 20, -8],
   "Toronto" => [ 43, 38, 79, 24, -5],
   "Vancouver" => [ 49, 16, 123, 7, -8],
   "Washington DC" => [ 38, 55, 77, 0, -5],
);

require "greg.pl";
require "rise.pl";
require "holidays.pl";
require "jewish.pl";

main();
exit 0;

sub main(){

   if ($ENV{HEBCAL_OPTS}){
      process_arguments($switches, split ' ', $ENV{HEBCAL_OPTS})
   }
   my ($rangetype, $theDate) = process_arguments($switches,@ARGV);

   $switches->{city} or $switches->{city} = "New York";

   my ($startAbs, $endAbs);
 SW1:{
      $rangetype eq 'year' and do{
	 if( $switches->{'hebrew-date-range'}){
	    die 'not supported';
	 }
	 else{
	    $theDate->{dd} = 1;
	    $theDate->{mm} = $JAN;
	    $startAbs = greg2abs($theDate);
	    $theDate->{yy}++;
	    $endAbs = greg2abs($theDate);
	 }
	 last;
      };

      $rangetype eq 'month' and do{
	 if( $switches->{'hebrew-date-range'}){
	    die 'not supported';
	 }
	 else{
	    $theDate->{dd} = 1;
	    $startAbs = greg2abs($theDate);
	    $endAbs = $startAbs+ DAYS_IN_MONTH($theDate->{mm},$theDate->{yy})
	 }
	 last;
      };

      $rangetype eq 'day' and do{
	 if( $switches->{'hebrew-date-range'}){
	    die 'not supported';
	 }
	 else{
	    $startAbs = greg2abs($theDate);
	    $endAbs = $startAbs+1;
	 }
	 last;
      };

   }

#    my @a = ();
#    tie *OUT, 'IO::ScalarArray', \@a;
#    select OUT;

#    print "startabs is $startAbs, endAbs is $endAbs\n";
   $theDate = abs2hebrew($startAbs);
   init_holidays($theDate->{yy});
   main_calendar($startAbs, $endAbs);
}

############### from greg.c



sub PrintGregDate( $ ) {
   my $dt = shift;

   my $separator = $switches->{'machine-format'} ? "\t" : ' ';

   if (!$switches->{noGreg}) {
      my $year = $dt->{yy};
      if ($switches->{yearDigits}){
	 $year %= 100;
      }

      if ($switches->{'iso-date-format'}){
	 printf ("%d-%d-%d$separator", $year, $dt->{mm}, $dt->{dd});	# yyyy-mm-dd
      }
      else{
	 printf ("%d/%d/$year$separator", $dt->{mm}, $dt->{dd});	# mm/dd/yyyy
      }
   }

   if ($switches->{weekday}){
      print sprintf ("%s%s", $ShortDayNames[dayOfWeek ($dt)],
		     $switches->{'machine-format'} ? "\t" : ', '
		    );
   }
}

# this is very wrong.
sub get_DST_offset( $ ){
   return 0;
}

sub print_candlelighting_times ($$$$) {

   my ($mask, $weekday, $config);
   my $todayGreg;
   ($config, $mask, $weekday, $todayGreg) = @_;
# offset of sunset to candlelighting in minutes

   if ($weekday == $FRI || ($mask & $Holiday::LIGHT_CANDLES))
   {
#       double xsunrise, xsunset;
#       int day_adj, status;

      PrintGregDate ($todayGreg);
      print "$Holiday::CANDLELIGHTING_TIME\t"
	if $switches->{'machine-format'};
      my ($sunrise, $sunset) =suntime ($locations{$config->{switches}{city}}, dayOfYear ($todayGreg),
				       $Hebcal::SUNRISE, $Hebcal::SUNSET);
      if (!defined $sunset ) {
	 print ("No sunset today.\n");
      }
      else {
	 my $day_adj =0;
	 printf "%s\n",
		timeadj ("Candle lighting: ",
			 $sunset,
			 $config->{candlelighting_offset} + get_DST_offset($config),
			 $day_adj);
      }
   }

   # offset of sunset to havdallah in minutes
   if ($weekday == $SAT ||
       (($mask & $Holiday::YOM_TOV_ENDS) &&
	$weekday != $FRI))
   {
#       double xsunrise, xsunset;
#       int day_adj, status;


      PrintGregDate ($todayGreg);
      print "$Holiday::HAVDALAH_TIME\t"
	if $switches->{'machine-format'};

      my ($sunrise, $sunset) = suntime ($locations{'New York'},dayOfYear ($todayGreg),
					$Hebcal::SUNRISE, $Hebcal::SUNSET);
      if (!defined $sunset ){
	 print ("No sunset today.\n");
      }
      else {
	 my $day_adj = 0;
	 printf "Havdalah [%d min]:%s\n",
		 $config->{havdalah_offset},
  		 timeadj ("",$sunset,
  			  $config->{havdalah_offset} + get_DST_offset($config),
  			  $day_adj);
      }
   }
}


sub getOmerBounds($)

{
   my ($hYear) = @_;
   my $d = new Date;

   $d->{dd} = 16;
   $d->{mm} = $NISAN;
   $d->{yy} = $hYear;

   my $beginOmer = hebrew2abs($d);
   $d->{dd} = 5;
   $d->{mm} = $SIVAN;
   my $endOmer = hebrew2abs($d);
   return ($beginOmer, $endOmer)
}


sub process_arguments( $@ ){
   my ($opthash, @arglist) = @_;
   @ARGV = @arglist;
   Getopt::Long::Configure( qw( no_ignore_case  bundling ))
      if( $Getopt::Long::VERSION >= 2.17 );
   GetOptions($opthash, qw(ashkenazis|a
			   candlelighting-times|c
			   city|C=s
			   all-hebrew-dates|d
			   hebrew-dates|D
			   iso-date-format|e
			   machine-format|f
			   no-default-holidays|h
			   hebrew-date-range|H
			   israeli-sedrot|i
 			   input-file|I=s
			   lattitude|l=s
			   longitude|L=s
			   havdalah-minutes|m=i
			   omer|o
			   saturday-sedrot|s
			   daily-sedrot|S
			   today|t
			   no-gregorian|T
			   weekday|w
			   no-rosh-chodesh|x
			   no-century|y
 			   yahrtzeit|yartzeit|Y=s
			  )) or die getUsage();

   my $theDate = new Date();
   my ( $gregd,  $gregm, $gregy ) = (localtime)[3,4,5];
   $gregm++;
   $gregy+=1900;
   my $gregDate = new Date( dd=>$gregd, mm=>$gregm, yy=>$gregy);

   local $_ = "@ARGV";
   /help/ and do {print getUsage(); exit };
   /info/ and do {print getInfo(); exit };
   /cities/ and do {print getCities(); exit };
   /warranty/ and do {print getWarranty(); exit };

   my $rangetype;
   if( @ARGV == 0 ){		# print this year
      $rangetype = 'year';
      if( $opthash->{'hebrew-date-range'} ){
	 $theDate->{yy} = abs2hebrew(greg2abs($gregDate))->{yy}
      }
      else{
 	 $theDate->{yy} = $gregDate->{yy};
      }
   }elsif( @ARGV == 1){
      $rangetype = 'year';
      $theDate->{yy} = $ARGV[0];
   }elsif( @ARGV == 2){
      $rangetype = 'month';
      ($theDate->{mm}, $theDate->{yy}) = @ARGV;

   }elsif( @ARGV == 3){
      $rangetype = 'day';
      ($theDate->{mm},$theDate->{dd}, $theDate->{yy}) = @ARGV;

   }

   $opthash->{today} and do {
      $rangetype = 'day';
      $theDate->{mm} = $gregm;
      $theDate->{dd} = $gregd;
      $opthash->{omer} = 1;
   };

   return( $rangetype,$theDate );

}

sub getUsage(){
   (my $progname = $0) =~ s(.([^/]+)$)($1);
<<EOF;
   $progname [options] [year]
   $progname [options] month year
   $progname [options] month day year
   $progname help
   $progname info
   $progname cities
Options:
  -a, --ashkenazis          Use ashkenazis hebrew
  -c, --candlelighting-times Print candlelighting times
  -c CITY, --city=CITY	    Set latitude, longitude, timezone and daylight
                		savings scheme according to specified city.
			    	This option implies the -c option.
  -d, --all-hebrew-dates
  -D, --hebrew-dates
  -e, --iso-date-format
  -f, --machine-format
  -h, --no-default-holidays
  -H, --hebrew-date-range
  --help    		    print this message
  -i, --israeli-sedrot
  -I FILE, --input-file=FILE
  -l DEG,MIN, --lattitude=DEG,MIN
  -L DEG,MIN, --longitude=DEG,MIN
  -m MIN, --havdalah-minutes=MIN
  -o, --omer
  -s, --saturday-sedrot
  -S, --daily-sedrot
  -t, --today
  -T, --no-gregorian
  -w, --weekday
  -x, --no-rosh-chodesh
  -y, --no-century
  -Y FILE, --yahrtzeit=FILE, -yartzeit=FILE

EOF
}

sub main_calendar ($$)
{
   my ($todayAbs, $endAbs) = @_;
   my $todayHeb = abs2hebrew ($todayAbs);
   my $todayGreg = abs2greg ($todayAbs);

   my $day_of_week =  ($todayAbs % 7);
   my ($beginOmer, $endOmer) = getOmerBounds ($todayHeb->{yy});
   my $sedrot;
   if ($switches->{'daily-sedrot'} || $switches->{'saturday-sedrot'}){
      $sedrot = new Sedrot($todayHeb->{yy}, $switches);
#       $sedraHandle = reset_sedra ($todayHeb->{yy});
   }

#   set_DST_bounds (&beginDST, &endDST, $todayGreg->{yy});
#   set_DST (beginDST, endDST, todayAbs, &DST);


   my $iterator = {
		   todayAbs	 => $todayAbs,
		   todayHeb	 => $todayHeb,
		   todayGreg	 => $todayGreg,
# 		   theYear	 => $theYear,
		   day_of_week	 => $day_of_week,
		   beginOmer	 => $beginOmer,
		   endOmer	 => $endOmer,
		   sedrot        => $sedrot,
		   switches      => $switches,
		   candlelighting_offset => -18,
		   havdalah_offset => 72,
		  };

   while ($todayAbs < $endAbs) {
      # get the holidays for today
      my ($returnedMask, $holidays) = getHebHolidays ($todayHeb);

      my $sedra_today = ($switches->{'daily-sedrot'}
			 || ($switches->{'saturday-sedrot'}
			     && ($day_of_week == $SAT)));
      my $omer_today = ($switches->{omer}
			&& ($todayAbs >= $beginOmer)
			&& ($todayAbs <= $endOmer));
      my $candle_today = ($switches->{'candlelighting-times'}
			  && ($day_of_week >= $FRI
			      || ($returnedMask & $Holiday::LIGHT_CANDLES)
			      || ($returnedMask & $Holiday::YOM_TOV_ENDS)));
      my $holidays_today = @$holidays &&
	(! $switches->{'no-default-holidays'} || ($returnedMask & $Holiday::USER_EVENT));

      if ($switches->{'all-hebrew-dates'}||
	  ($switches->{'hebrew-dates'} &&
	   ($holidays_today || $sedra_today || $omer_today || $candle_today))) {
	 PrintGregDate ($todayGreg);
	 print "$Holiday::HEBREW_DATE\t"
	   if $switches->{'machine-format'};
	 printf "%d%s of %s, %d\n",
	 $todayHeb->{dd},	# print the hebrew date
	 numSuffix ($todayHeb->{dd}),
	 $hMonths[LEAP_YR_HEB ($todayHeb->{yy})][$todayHeb->{mm}],
	 $todayHeb->{yy};
      }


      # print the sedra, if desired
      if ($sedra_today) {
	 my $sedra = $sedrot->getSedra($todayAbs);
	 if ($sedra) {
	    PrintGregDate ($todayGreg);
	    print "$Holiday::SEDRA\t"
	      if $switches->{'machine-format'};
	    printf "%s %s\n", $switches->{'ashkenazis'} ? "Parshas" : "Parashat",
	    $sedra;
	 }
      }

      # print today's holidays
#       holi_start=holip;		# store the head of the list for freeing
#       for (; holip; holip = holip->next)
      foreach my $holiday (@$holidays){
	 if (!$switches->{'no-default-holidays'} || ($holiday->typeMask & $Holiday::USER_EVENT)) {
	    PrintGregDate ($todayGreg);
	    print scalar($holiday->{mask}+0)."\t"
	      if $switches->{'machine-format'};
	    print $holiday->{name}."\n"
	 }
      }

      # Print the Omer
      if ($omer_today) {
	 PrintGregDate ($todayGreg);
	 my $omer =  ($todayAbs - $beginOmer + 1);
	 my $omerstr;
	 if ($switches->{'machine-format'}){
	    $omerstr = "Omer: $omer";
	 }
	 else {
	    $omerstr = $omer.numSuffix($omer)." day of the Omer";
	 }
	 print "$Holiday::OMER\t"
	   if $switches->{'machine-format'};
	 print "$omerstr\n"
      }

      # Print CandleLighting times
      if ($candle_today) {
	 # 	 set_DST (beginDST, endDST, $todayAbs, &DST);
	 print_candlelighting_times ($iterator,$returnedMask,
				     $day_of_week, $todayGreg)
      }

      $todayHeb = nextHebDate ($todayHeb);
      $todayAbs++;
      $todayGreg = abs2greg($todayAbs);
      $day_of_week = ($day_of_week + 1) % 7;

      if ($Hebcal::TISHREI == $todayHeb->{mm}   #	/* if Rosh Hashana then reset year-related data
	  && 1 == $todayHeb->{dd}) {
# 	 $theYear = yearData ($todayHeb->{yy});
# 	 if ($switches->{'daily-sedrot'} || $switches->{'saturday-sedrot'}){
# 	    $sedraHandle = reset_sedra ($todayHeb->{yy});
# 	 }
	 if ($switches->{printOmer}){
	    ($beginOmer, $endOmer) = getOmerBounds ($todayHeb->{yy});
	 }
	 init_holidays($todayHeb->{yy});
      }

#       if (1 == $todayGreg->{dd} &&	# happy new year!
# 	  $JAN == $todayGreg->{mm}) {
# # 	  set_DST_bounds (&beginDST, &endDST, $todayGreg->{yy});
#       }
    }
}


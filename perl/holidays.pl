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

use Holiday;
use strict;
my $const_holidays = [];
my $var_holidays = [];

use vars qw( $SUNRISE $SUNSET  );
use vars qw( $JAN $FEB $MAR $APR $MAY $JUN $JUL $AUG $SEP $OCT $NOV $DEC);
use vars qw( $SUN $MON $TUE $WED $THU $FRI $SAT );
use vars qw( $SIVAN $SHVAT $AV $IYYAR $ELUL $CHESHVAN $KISLEV $NISAN  $TEVET $ADAR_I $ADAR_II $TISHREI);
use vars qw( $switches @hMonths);


my $chan = 'Chanukah';		# keep this spelling consistent
my @input_holidays = ( {
			mm => $TISHREI, 
			dd=>2,
			name => "Rosh Hashana II",
			mask =>  $Holiday::YOM_TOV | $Holiday::YOM_TOV_ENDS 
		       },
		       {
			mm => $TISHREI,
			dd=> 9,
			name=> "Erev Yom Kippur",
			mask=> $Holiday::LIGHT_CANDLES},
		       {
			mm=>$TISHREI,dd=> 10,
			name=>"Yom Kippur", mask=>$Holiday::YOM_TOV_ENDS| $Holiday::YOM_TOV},
		       {
			mm=>$TISHREI, dd=>14,
			name=>["Erev Sukkot", "Erev Sukkos"], 
			mask=>$Holiday::LIGHT_CANDLES},
		       {
			mm=>$TISHREI, dd=>15,
			name=>["Sukkot I", "Sukkos I"],
			mask=>  $Holiday::YOM_TOV
		       },
		       {
			mm => $TISHREI, dd=> 16,
			name=>["Sukkot II", "Sukkos II"], 
			mask => $Holiday::YOM_TOV_ENDS| $Holiday::YOM_TOV,
		       },
		       {
			mm => $TISHREI, dd=> 17,
			name => ["Sukkot III (CH''M)", "Sukkos III (CH''M)"],
			mask => $Holiday::CHOL_HAMOED,
		       },
		       {
			mm => $TISHREI, dd => 18,
			name => ["Sukkot IV (CH''M)", "Sukkos IV (CH''M)"],
			mask => $Holiday::CHOL_HAMOED,
		       },
		       {
			mm => $TISHREI, dd => 19,
			name=>["Sukkot V (CH''M)", "Sukkos V (CH''M)"],
			mask => $Holiday::CHOL_HAMOED,
		       },
		       {
			mm => $TISHREI, dd => 20,
			name=>["Sukkot VI (CH''M)", "Sukkos VI (CH''M)"],
			mask => $Holiday::CHOL_HAMOED,
		       },
		       {
			mm => $TISHREI, dd => 21,
			name =>["Sukkot VII (Hoshana Raba)", "Sukkos VII (Hoshana Raba)"],
			mask=> $Holiday::LIGHT_CANDLES | $Holiday::YOM_TOV, 
		       },
		       {
			mm => $TISHREI, dd => 22,
			name => ["Shmini Atzeret", "Shmini Atzeres"],
			mask => $Holiday::YOM_TOV
		       },
		       {
			mm => $TISHREI, dd => 23,
			name =>["Simchat Torah", "Simchas Torah"],
			mask => $Holiday::YOM_TOV_ENDS | $Holiday::YOM_TOV},
		       
		       {
			mm => $KISLEV, dd => 24,
			name =>"$chan: 1 Candle",
			mask => $Holiday::CHANUKAH
		       },
		       {
			mm => $KISLEV, dd => 25,
			name => "$chan: 2 Candles",
			mask => $Holiday::CHANUKAH
},
		       {
			mm => $KISLEV, dd => 26,
			name =>"$chan: 3 Candles",
			mask => $Holiday::CHANUKAH
},
		       {
			mm => $KISLEV, dd => 27,
			name => "$chan: 4 Candles",
			mask => $Holiday::CHANUKAH
},
		       {
			mm => $KISLEV, dd => 28,
			name => "$chan: 5 Candles",
			mask => $Holiday::CHANUKAH
		       },
		       {
			mm => $KISLEV, dd => 29,
			name => "$chan: 6 Candles",
			mask => $Holiday::CHANUKAH
		       },
		       {
			mm => $SHVAT, dd => 15,
			name => "Tu B'Shvat",
			mask => $Holiday::HOLIDAY_NON_YOM_TOV,
		       },
		       {
			mm => $NISAN, dd => 14,
			name => "Erev Pesach",
			mask => $Holiday::LIGHT_CANDLES
		       },
		       {
			mm => $NISAN, dd => 15,
			name => "Pesach I",
		        mask => $Holiday::YOM_TOV,
		       },
		       {
			mm => $NISAN, dd => 16,
			name => "Pesach II",
			mask => $Holiday::YOM_TOV_ENDS| $Holiday::YOM_TOV,
		       },
		       {
			mm => $NISAN, dd => 17,
			name => "Pesach III (CH''M)",
			mask => $Holiday::CHOL_HAMOED,
		       },
		       {
			mm => $NISAN, dd => 18,
			name => "Pesach IV (CH''M)",
			mask => $Holiday::CHOL_HAMOED,
		       },
		       {
			mm => $NISAN, dd => 19,
			name => "Pesach V (CH''M)",
			mask => $Holiday::CHOL_HAMOED,
		       },
		       {
			mm => $NISAN, dd => 20,
			name => "Pesach VI (CH''M)",
			mask =>$Holiday::LIGHT_CANDLES | $Holiday::CHOL_HAMOED},
		       {
			mm   => $NISAN, dd => 21,
			name => "Pesach VII",
			mask =>  $Holiday::YOM_TOV
		       },
		       {
			mm => $NISAN, dd => 22,
			name => "Pesach VIII", 
			mask =>$Holiday::YOM_TOV_ENDS | $Holiday::YOM_TOV 
		       },
		       {
			mm => $IYYAR, dd => 18,
			name => "Lag B'Omer",
			mask => $Holiday::HOLIDAY_NON_YOM_TOV,
		       },
		       {
			mm => $SIVAN, dd => 5,
			name => ["Erev Shavuot", "Erev Shavuos"],
			mask =>$Holiday::LIGHT_CANDLES},
		       {
			mm => $SIVAN, dd => 6,
			name => ["Shavuot I", "Shavuos I"],
			mask =>  $Holiday::YOM_TOV,
		       },
		       {
			mm => $SIVAN, dd => 7,
			name => ["Shavuot II", "Shavuos II"], 
			mask =>$Holiday::YOM_TOV_ENDS| $Holiday::YOM_TOV
		       },
		       {
			mm => $ELUL, dd => 29,
			name => "Erev Rosh Hashana",
			mask => $Holiday::LIGHT_CANDLES
		       }
		     );


#-------------------------------------------------------------------------
# pushes a copy of a holiday on to a holiday     
# list, returning the typemask of the pushed element 
sub PushHoliday( $$$$ )
{
   my ($holiday, $holidaymap, $mm, $dd) = @_;
   push @{$holidaymap->[$mm][$dd]}, $holiday;
   return $holiday->{mask};
}


#-------------------------------------------------------------------------

sub get_variable_holidays( $ ){
   my $var_holidays = [];
   my $hYear = shift;
   my $tempDt = new Date();


   $tempDt->{yy} = $hYear;
   $tempDt->{mm} = $TISHREI;
   $tempDt->{dd} = 1;
   my $roshHashana = hebrew2abs ($tempDt); 

   
   $tempDt->{yy} = $hYear;
   $tempDt->{mm} = $TEVET;
   $tempDt->{dd} = 10;
   my $tevet10 = hebrew2abs ($tempDt);

   $tempDt->{mm} = $NISAN;
   $tempDt->{dd} = 15;
   my $passover = hebrew2abs ($tempDt);

   $tempDt->{mm} = $AV;
   $tempDt->{dd} = 9;
   my $tishaBav = hebrew2abs ($tempDt);

   my $hol = new Holiday( 
				 name => "Tzom Gedaliah",
				 mask => $Holiday::FAST_DAY,
				);
   PushHoliday ($hol, $var_holidays,$TISHREI,
		$roshHashana % 7 == $THU ? 4 : 3);

   $hol = new Holiday(name => $switches->{ashkenazis} ? "Shabbas Shuvah" : "Shabbat Shuva",
		      mask => $Holiday::SPECIAL_SHABBAT);
   $tempDt = abs2hebrew( day_on_or_before( $SAT, 7 + $roshHashana));
   PushHoliday( $hol, $var_holidays,$TISHREI,$tempDt->{dd});


   if( short_kislev( $hYear )) {
      PushHoliday( new Holiday( name => "$chan: 7 Candles",
				mask => $Holiday::CHANUKAH
			      ), $var_holidays,$TEVET,1);
      PushHoliday( new Holiday( name => "$chan: 8 Candles",
				mask => $Holiday::CHANUKAH
			      ), $var_holidays,$TEVET,2);
      PushHoliday( new Holiday( name => "$chan: 8th Day" ,
				mask => $Holiday::CHANUKAH
			      ), $var_holidays,$TEVET,3);
   }
   else {
      PushHoliday( new Holiday( name => "$chan: 7 Candles",
				mask => $Holiday::CHANUKAH
			      ), $var_holidays,$KISLEV,30);
      PushHoliday( new Holiday( name => "$chan: 8 Candles",
				mask => $Holiday::CHANUKAH
			      ), $var_holidays,$TEVET,1);
      PushHoliday( new Holiday( name => "$chan: 8th Day" ,
				mask => $Holiday::CHANUKAH
			      ), $var_holidays,$TEVET,2);
   }

   $hol = new Holiday (name => $switches->{ashkenazis} ? "Shabbas Shekalim" : "Shabbat Shekalim",
		       mask => $Holiday::SPECIAL_SHABBAT);
   $tempDt = abs2hebrew (day_on_or_before ($SAT, $passover - 43));
   PushHoliday ($hol, $var_holidays,$tempDt->{mm},$tempDt->{dd});

   $hol = new Holiday (name => $switches->{ashkenazis} ? "Shabbas Zachor" : "Shabbat Zachor",
		       mask => $Holiday::SPECIAL_SHABBAT);
   $tempDt = abs2hebrew (day_on_or_before ($SAT, $passover - 30));
   PushHoliday ($hol, $var_holidays,$tempDt->{mm},$tempDt->{dd});

   $hol = new Holiday ( name=> $switches->{ashkenazis} ? "Ta'anis Esther" : "Ta'anit Esther",
			mask => $Holiday::FAST_DAY);
   $tempDt = abs2hebrew ($passover - ($passover % 7 == $TUE ? 33 : 31));
   PushHoliday ($hol, $var_holidays,$tempDt->{mm},$tempDt->{dd});

   if (LEAP_YR_HEB ($hYear))
   {
      $hol = new Holiday (name => $switches->{ashkenazis} ? "Purim Koson" : "Purim Katan",
			  mask => $Holiday::HOLIDAY_NON_YOM_TOV);
      PushHoliday( $hol, $var_holidays,$ADAR_I,14);
      PushHoliday( new Holiday( name =>"Purim",
				mask => $Holiday::HOLIDAY_NON_YOM_TOV),
		   $var_holidays,$ADAR_II,14);
   }
   else
   {
      $hol = new Holiday (name => "Purim",
			  mask => $Holiday::HOLIDAY_NON_YOM_TOV,
			 );
      PushHoliday ($hol, $var_holidays,$ADAR_I,14);
   }

   $hol = new Holiday ( name => "Shushan Purim",
			mask => $Holiday::HOLIDAY_NON_YOM_TOV,
		      );
   $tempDt = abs2hebrew ($passover - ($passover % 7 == $SUN ? 28 : 29));
   PushHoliday ($hol, $var_holidays,$tempDt->{mm},$tempDt->{dd});

   $hol = new Holiday (name => $switches->{ashkenazis} ? "Shabbas Parah" : "Shabbat Parah",
		       mask => $Holiday::SPECIAL_SHABBAT);
   $tempDt = abs2hebrew (day_on_or_before ($SAT, $passover - 14) - 7);
   PushHoliday ($hol, $var_holidays,$tempDt->{mm},$tempDt->{dd});

   $hol = new Holiday ( name => $switches->{ashkenazis} ? "Shabbas HaChodesh" : "Shabbat HaChodesh",
		       mask => $Holiday::SPECIAL_SHABBAT);
   $tempDt = abs2hebrew (day_on_or_before ($SAT, $passover - 14));
   PushHoliday ($hol, $var_holidays,$tempDt->{mm},$tempDt->{dd});

   $hol = new Holiday ( name => $switches->{ashkenazis} ? "Ta'anis Bechoros" : "Ta'anit Bechorot",
			mask => $Holiday::FAST_DAY );
   if (($passover - 1) % 7 >= 5)
   {				# $SATurday or sunday 
      $tempDt = abs2hebrew (day_on_or_before ($THU, $passover));
      PushHoliday ($hol, $var_holidays,$tempDt->{mm},$tempDt->{dd});
   }
   else{
      PushHoliday ($hol, $var_holidays,$NISAN,14);
   }

 
   $hol = new Holiday (name => $switches->{ashkenazis} ? "Shabbas HaGadol" : "Shabbat HaGadol",
		       mask => $Holiday::SPECIAL_SHABBAT);
   $tempDt = abs2hebrew (day_on_or_before ($SAT, $passover - 1));
   PushHoliday ($hol, $var_holidays,$tempDt->{mm},$tempDt->{dd});

   if ($hYear > 5698)
   {				# these only really make sense after 1938 
      $hol = new Holiday (name =>"Yom HaShoah",
			  mask => $Holiday::HOLIDAY_NON_YOM_TOV);
      PushHoliday ($hol, $var_holidays,$NISAN,27);

      PushHoliday( new Holiday( name => "Yom Yerushalayim",
				mask => $Holiday::HOLIDAY_NON_YOM_TOV),
		   $var_holidays,$IYYAR,28);

      $hol = new Holiday (name => "Yom HaAtzma'ut",
			  mask => $Holiday::HOLIDAY_NON_YOM_TOV);
      if ($passover % 7 == $SUN){
	 $tempDt->{dd} = 3;
      }
      elsif ($passover % 7 == $SAT){
	 $tempDt->{dd} = 4;
      }
      else{
	 $tempDt->{dd} = 5;
      }
      PushHoliday ($hol, $var_holidays,$IYYAR,$tempDt->{dd});
      $hol = new Holiday ( name => "Yom HaZikaron",
			   mask => $Holiday::HOLIDAY_NON_YOM_TOV);
      PushHoliday ($hol, $var_holidays,$IYYAR,$tempDt->{dd} - 1);
   }

   $hol = new Holiday ( name => "Tzom Tammuz",
			mask => $Holiday::FAST_DAY);
   if ($tishaBav % 7 == $SAT){
      $tempDt = abs2hebrew ($tishaBav - 20);
   }
   else{
      $tempDt = abs2hebrew ($tishaBav - 21);
   }
   PushHoliday ($hol, $var_holidays,$tempDt->{mm},$tempDt->{dd});

   $hol = new Holiday (name => $switches->{ashkenazis} ? "Shabbas Hazon" : "Shabbat Hazon",
		       mask => $Holiday::SPECIAL_SHABBAT);
   $tempDt = abs2hebrew (day_on_or_before ($SAT, $tishaBav));
   PushHoliday ($hol, $var_holidays,$tempDt->{mm},$tempDt->{dd});

   $hol = new Holiday ( name => "Tish'a B'Av",
			mask => $Holiday::FAST_DAY);
   PushHoliday ($hol, $var_holidays,$AV,
		$tishaBav % 7 == $SAT ? 10 : 9);

   $hol = new Holiday (name => $switches->{ashkenazis} ? "Shabbas Nachamu" : "Shabbat Nachamu",
		       mask => $Holiday::SPECIAL_SHABBAT);
   $tempDt = abs2hebrew (day_on_or_before ($SAT, $tishaBav + 7));
   PushHoliday ($hol, $var_holidays,$tempDt->{mm},$tempDt->{dd});

   $hol = new Holiday (name => "Asara B'Tevet",
		       mask => $Holiday::FAST_DAY);
   if ($tevet10 % 7 >= 5)
   {				# $SATurday or sunday 
      $tempDt = abs2hebrew (day_on_or_before ($THU, $tevet10));
      PushHoliday ($hol, $var_holidays,$tempDt->{mm},$tempDt->{dd});
   }
   else{
      PushHoliday ($hol, $var_holidays,$TEVET,10);
   }

   return $var_holidays;
}


#-------------------------------------------------------------------------
my $first =1;
sub init_holidays( $ ){
   my $hYear  = shift;
# NB that this is dependant on the $hYear. 
# It must therefore be called at the beginning of every hebrew year. 

   if ($first)
   {
      foreach my $href (@input_holidays){
	 my $holiday = new Holiday(%$href);
	 $holiday->{name} = $holiday->{name}[exists $switches->{ashkenazis}] 
	   if ref $holiday->{name} ;
	 PushHoliday ($holiday, $const_holidays, $holiday->{mm}, $holiday->{dd});
      }
      $first = 0;
   }
   $var_holidays = get_variable_holidays( $hYear );
}



#-------------------------------------------------------------------------

#return a bitmask and a list of today's holidays
sub getHebHolidays ($)
{
   my $dth = shift;	# today's hebrew date
   my $tmpMask =0;
   my @holidays = ();

   my $holiday;
   foreach $holiday (  @{$const_holidays->[$dth->{mm}][$dth->{dd}]},
		       @{$var_holidays->[$dth->{mm}][$dth->{dd}]}){
      $tmpMask |= $holiday->{mask} if $holiday->{mask};
      push @holidays, $holiday;
   }

   if ($dth->{dd} == 1)
   {
      if($dth->{mm} == $TISHREI)	# special processing for rosh hashana 
      {
	 $holiday = new Holiday( name =>"Rosh Hashana $dth->{yy}",
				 mask =>$Holiday::YOM_TOV
			       );
	 push @holidays, $holiday;
      }
      else {
	 if( ! $switches->{'no-rosh-chodesh'} )      # rosh Chodesh Processing... 
	 {
# 	    print Dumper($dth);
	    $holiday = new Holiday( name => "Rosh Chodesh ". $hMonths[LEAP_YR_HEB( $dth->{yy})][$dth->{mm}],
				    mask => $Holiday::ROSH_CHODESH
				  );
# 	    print Dumper(@hMonths);
# 	    print "index= ".LEAP_YR_HEB($dth->{yy})." month is $dth->{mm}\n";
	    push @holidays, $holiday;
	 }
      }
   }
   

   if( $dth->{dd} == 30 && ! $switches->{'no-rosh-chodesh'})
   {
      $holiday = new Holiday( name => "Rosh Chodesh ". $hMonths[LEAP_YR_HEB ($dth->{yy})][$dth->{mm}+1] ,
				    mask => $Holiday::ROSH_CHODESH
			    );
      push @holidays, $holiday;
   }
  

   return ($tmpMask, \@holidays);
}
__END__
#------------------------------------------------------------
#-------------------------------------------------------------------------
# this function stores the user "holidays" drawn from inFile in the 
   var_holidays array.  The intent is that it will be loaded every new 
   Jewish year.  Gross, I know, but an efficient version will have to wait 
   until %ID% + .1
   

void 
init_user_holidays ($hYear)
int $hYear;
{
   holstorep_t tmpholp;

   char *s, *monthStr, *eventStr, nextChar;
   int index, inMonth, inDay, lineNum = 1;

   initStr (&s, MAX_LINE_LEN);
   initStr (&monthStr, MAX_LINE_LEN);
   rewind (inFile);
   nextChar = (char) getc (inFile);	# priming getc 
   for (; !feof (inFile);
	lineNum++, nextChar = (char) getc (inFile))
   {				# force an EOF 
      ungetc (nextChar, inFile);
      if (!fgets (s, MAX_LINE_LEN, inFile))
      {
	 warn ("input file read error. Skipping line %s", hc_itoa (lineNum));
	 continue;
      }
      if (s[0] == '\n')		# blank line 
	 continue;
      if (!sscanf (s, "%s %d%n", monthStr, &inDay, &index))
      {
	 warn ("Error in input file.  Skipping line %s", hc_itoa (lineNum));
	 continue;
      }
      if (isAllNums (monthStr))
      {
	 warn ("Numeric hebrew month in input file.  Skipping line %s",
	       hc_itoa (lineNum));
	 continue;
      }
      if (!(inMonth = lookup_hebrew_month (monthStr)))
      {
	 warn ("Unrecognized hebrew month in input file.  Skipping line %s",
	       hc_itoa (lineNum));
	 continue;
      }
      if (inDay < 1 || inDay > 30)
      {
	 warn ("Date out of range in input file. Skipping line %s",
	       hc_itoa (lineNum));
	 continue;
      }

      if (inMonth == $ADAR_II && !LEAP_YR_HEB ($hYear))
	 inMonth = $ADAR_I;

      eventStr = s + index + 1;	# get the name of the event 
      if (eventStr[strlen (eventStr) - 1] == '\n')
	 eventStr[strlen (eventStr) - 1] = '\0';		# chop off the \n 

      # store the holiday in the LUT 
      tmpholp = getHolstorep ();
      initStr (&tmpholp->name, MAX_LINE_LEN);
      strcpy (tmpholp->name, eventStr);		# load the user holiday into it. 
      tmpholp->typeMask = $Holiday::USER_EVENT;
      PushHoliday (tmpholp, $var_holidays[inMonth][inDay]);

   }
}


#-------------------------------------------------------------------------
# this function stores yahrtzeits found in yFile.
   each record is stored in mm dd yyyy str format using GREGORIAN dates 
   

void 
init_yahrtzeits ($hYear)
int $hYear;
{
   holstorep_t tmpholp;

   char *s, *monthStr, *eventStr, nextChar;
   int index, inMonth, inDay, inYear, lineNum = 1;
   date_t hDeath, gDeath;

   initStr (&s, MAX_LINE_LEN);
   initStr (&monthStr, MAX_LINE_LEN);
   rewind (yFile);
   nextChar = (char) getc (yFile);	# priming getc 
   for (; !feof (yFile);
	lineNum++, nextChar = (char) getc (yFile))
   {				# force an EOF 
      ungetc (nextChar, yFile);
      if (!fgets (s, MAX_LINE_LEN, yFile))
      {
	 warn ("yahrtzeit file read error. Skipping line %s", hc_itoa (lineNum));
	 continue;
      }
      if (s[0] == '\n')		# blank line 
	 continue;
      if (sscanf (s, "%s %d %d%n", monthStr, &inDay, &inYear, &index) < 3)
      {
	 warn ("Error in yahrtzeit file.  Skipping line %s", hc_itoa (lineNum));
	 continue;
      }
      if (!isAllNums (monthStr))
      {
	 warn ("Non-numeric month in yahrtzeit file. Skipping line %s",
	       hc_itoa (lineNum));
	 continue;
      }
      sscanf (monthStr, "%d", &inMonth);

      if (inMonth > 12 || inMonth < 1 ||
	  inDay < 1 || inDay > MonthLengths[LEAP (inYear)][inMonth])
      {

	 warn ("Date out of range in yahrtzeit file. Skipping line %s",
	       hc_itoa (lineNum));
	 continue;
      }

      gDeath.dd = inDay;
      gDeath.mm = inMonth;
      gDeath.yy = inYear;

      hDeath = abs2hebrew (greg2abs (gDeath));

# If it's Heshvan 30 it depends on the first anniversary; if
#    that was not Heshvan 30, use the day before $KISLEV 1. 

      if (hDeath.mm == CHESHVAN && hDeath.dd == 30 &&
	  !long_cheshvan (hDeath.yy + 1))
      {
	 hDeath.dd = 1;
	 hDeath.mm = $KISLEV;
	 hDeath.yy = $hYear;
	 hDeath = abs2hebrew (hebrew2abs (hDeath) - 1);
      }
# If it's $KISLEV 30 it depends on the first anniversary; if
#    that was not $KISLEV 30, use the day before $TEVETh 1. 
      elsif (hDeath.mm == $KISLEV && hDeath.dd == 30 &&
	       !short_$KISLEV (hDeath.yy + 1))
      {
	 hDeath.dd = 1;
	 hDeath.mm = $TEVET;
	 hDeath.yy = $hYear;
	 hDeath = abs2hebrew (hebrew2abs (hDeath) - 1);
      }
# If it's Adar II, use the same day in last month of
#    year (Adar or Adar II). 
      elsif (hDeath.mm == $ADAR_II)
      {
	 hDeath.mm = MONTHS_IN_HEB ($hYear);
      }
# If it's the 30th in Adar I and year is not a leap year
#    (so Adar has only 29 days), use the last day in Shevat. 
      elsif (hDeath.mm == $ADAR_I && hDeath.dd == 30 &&
	       !LEAP_YR_HEB ($hYear))
      {
	 hDeath.dd = 30;
	 hDeath.mm = SHVAT;
      }
# In all other cases, use the normal anniversary of the date of death. 

      eventStr = s + index + 1;	# get the name of the event 
#    if (eventStr[strlen(eventStr)-1] == '\n') 
      eventStr[strlen (eventStr) - 1] = '\0';	# chop off the \n 

      # store the holiday in the LUT 
      tmpholp = getHolstorep ();
      initStr (&tmpholp->name, MAX_LINE_LEN);
      strcpy (tmpholp->name, eventStr);		# load the user holiday into it. 
      tmpholp->typeMask = USER_EVENT;
      PushHoliday (tmpholp, $var_holidays[hDeath.mm][hDeath.dd]);

   }
}



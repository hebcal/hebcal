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


package Holiday;


$USER_EVENT	 = 1;
$LIGHT_CANDLES	 = 1<<1;
$YOM_TOV_ENDS	 = 1<<2;
   
   # event designators
$YOM_TOV	 = 1<<3;
$FAST_DAY	 = 1<<4;
$ROSH_CHODESH	 = 1<<5;
$OMER		 = 1<<6;
$SEDRA		 = 1<<7;
$CANDLELIGHTING_TIME	 = 1<<8;
$HAVDALAH_TIME	 = 1<<9;
$HEBREW_DATE	 = 1<<10;
$CHOL_HAMOED	 = 1<<11;
$CHANUKAH	 = 1<<12;
$SPECIAL_SHABBAT = 1<<13;
$HOLIDAY_NON_YOM_TOV	 = 1<<14;
$YAHRTZEIT	 = 1<<15;


package Hebcal;
$NISAN =1;
$IYYAR = 2;
$SIVAN = 3;
$TAMUZ = 4;
$AV = 5;
$ELUL = 6;
$TISHREI = 7;
$CHESHVAN = 8;
$KISLEV = 9;
$TEVET = 10;
$SHVAT = 11;
$ADAR_I = 12;
$ADAR_II = 13;

package Hebcal;
$JAN = 1;
$FEB = 2;
$MAR = 3;
$APR = 4;
$MAY = 5;
$JUN = 6;
$JUL = 7;
$AUG = 8;
$SEP = 9;
$OCT = 10;
$NOV = 11;
$DEC = 12;

$SUN = 0;
$MON = 1;
$TUE = 2;
$WED = 3;
$THU = 4;
$FRI = 5;
$SAT = 6;

package Date;
use Carp;
my @FIELDNAMES = qw( yy mm dd );
# use fields qw( yy mm dd  ); #neat-o compile-time field-name checking
#months are 1-based    
sub new (@){ 
   my $class = shift; 
   my $self;
   if( ref $_[0] eq 'Date' ){
      $self = bless { map {$_,($_[0]->{$_})} @FIELDNAMES };
   }
   else{
      scalar(@_) %2 == 0 or croak "bad arglist";
      my %args = @_;
      $self = bless { map {$_,($args{$_})} @FIELDNAMES };
   }
   return $self;
}
package Holiday;
use Carp;

sub import{
   require 'Holiday.ph';
}

my @FIELDNAMES = qw( mm dd name mask);
#months are 1-based    
sub new (@){ 
   my $class = shift; 
   my $self;
   if( ref $_[0] eq 'Holiday' ){
      $self = bless { map{($_, $_[0]->{$_})} @FIELDNAMES };
   }
   else{
      my %args = (mask => 0, @_);
      $self = bless  { map{($_, $args{$_})} @FIELDNAMES };
   }
   return $self;
}


#for issues dealing with hebrew text, first read http://www.cis.ohio-state.edu/htbin/rfc/rfc1555.html


# FIX: think about hebrew output 
#                        PC    IBM     PC     ISO
#            Hebrew                           8859-8
#            letter     8-bit         7-bit   8-bit
#                       Ascii  EBCDIC Ascii   Ascii
#            ---------- -----  ------ -----   ------
#            alef        128     41    96     224
#            bet         129     42    97     225
#            gimel       130     43    98     226
#            dalet       131     44    99     227
#            he          132     45   100     228
#            vav         133     46   101     229
#            zayin       134     47   102     230
#            het         135     48   103     231
#            tet         136     49   104     232
#            yod         137     51   105     233
#            kaf sofit   138     52   106     234
#            kaf         139     53   107     235
#            lamed       140     54   108     236
#            mem sofit   141     55   109     237
#            mem         142     56   110     238
#            nun sofit   143     57   111     239
#            nun         144     58   112     240
#            samekh      145     59   113     241
#            ayin        146     62   114     242
#            pe sofit    147     63   115     243
#            pe          148     64   116     244
#            tsadi sofit 149     65   117     245
#            tsadi       150     66   118     246
#            qof         151     67   119     247
#            resh        152     68   120     248
#            shin        153     69   121     249
#            tav         154     71   122     250

package Sedrot;
use strict; 
my @sedrot = ( ["Bereshit", "Bereshis"],
	    ["Noach", "Noach"],
	    ["Lech-Lecha", "Lech-Lecha"],
	    ["Vayera", "Vayera"],
	    ["Chayei Sara", "Chayei Sara"],
	    ["Toldot", "Toldos"],
	    ["Vayetzei", "Vayetzei"],
	    ["Vayishlach", "Vayishlach"],
	    ["Vayeshev", "Vayeshev"],
	    ["Miketz", "Miketz"],
	    ["Vayigash", "Vayigash"],
	    ["Vayechi", "Vayechi"],
	    ["Shemot", "Shemos"],
	    ["Vaera", "Vaera"],
	    ["Bo", "Bo"],
	    ["Beshalach", "Beshalach"],
	    ["Yitro", "Yisro"],
	    ["Mishpatim", "Mishpatim"],
	    ["Terumah", "Terumah"],
	    ["Tetzaveh", "Tetzaveh"],
	    ["Ki Tisa", "Ki Sisa"],
	    ["Vayakhel", "Vayakhel"],
	    ["Pekudei", "Pekudei"],
	    ["Vayikra", "Vayikra"],
	    ["Tzav", "Tzav"],
	    ["Shmini", "Shmini"],
	    ["Tazria", "Sazria"],
	    ["Metzora", "Metzora"],
	    ["Achrei Mot", "Achrei Mos"],
	    ["Kedoshim", "Kedoshim"],
	    ["Emor", "Emor"],
	    ["Behar", "Behar"],
	    ["Bechukotai", "Bechukosai"],
	    ["Bamidbar", "Bamidbar"],
	    ["Nasso", "Nasso"],
	    ["Beha'alotcha", "Beha'aloscha"],
	    ["Sh'lach", "Sh'lach"],
	    ["Korach", "Korach"],
	    ["Chukat", "Chukas"],
	    ["Balak", "Balak"],
	    ["Pinchas", "Pinchas"],
	    ["Matot", "Matos"],
	    ["Masei", "Masei"],
	    ["Devarim", "Devarim"],
	    ["Vaetchanan", "Vaeschanan"],
	    ["Eikev", "Eikev"],
	    ["Re'eh", "Re'eh"],
	    ["Shoftim", "Shoftim"],
	    ["Ki Teitzei", "Ki Seitzei"],
	    ["Ki Tavo", "Ki Savo"],
	    ["Nitzavim", "Nitzavim"],
	    ["Vayeilech", "Vayeilech"],
	    ["Ha'Azinu", "Ha'Azinu"],
	  );



my $INCOMPLETE = 0;
my $REGULAR = 1;
my $COMPLETE = 2;

# /* parsha doubler */
sub D($){ return - $_[0]; }
# /* parsha undoubler */
sub U($){ return - $_[0]; }


# /* The ordinary year types (keviot) */

# /* Hebrew year that starts on Monday, is `incomplete' (Heshvan and
#    Kislev each have 29 days), and has Passover start on Tuesday. */
my $nonleap_monday_incomplete =
[51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, D (21), 23, 24, -1, 25, D (26), D (28), 30, D (31), 33, 34,
 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49, D (50)];

# /* Hebrew year that starts on Monday, is `complete' (Heshvan and
#    Kislev each have 30 days), and has Passover start on Thursday. */
my $nonleap_monday_complete_diaspora =
[51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, D (21), 23, 24, -1, 25, D (26), D (28), 30, D (31), 33,
 -1, 34, 35, 36, 37, D (38), 40, D (41), 43, 44, 45, 46, 47, 48, 49,
 D (50)];

# /* Hebrew year that starts on Monday, is `complete' (Heshvan and
#    Kislev each have 30 days), and has Passover start on Thursday. */
my $nonleap_monday_complete_israel =  $nonleap_monday_incomplete;

# /* Hebrew year that starts on Tuesday, is `regular' (Heshvan has 29
#    days and Kislev has 30 days), and has Passover start on Thursday. */
my $nonleap_tuesday_regular_diaspora  =  $nonleap_monday_complete_diaspora;


# /* Hebrew year that starts on Tuesday, is `regular' (Heshvan has 29
#    days and Kislev has 30 days), and has Passover start on Thursday. */
my $nonleap_tuesday_regular_israel = $nonleap_monday_incomplete;

# /* Hebrew year that starts on Thursday, is `regular' (Heshvan has 29
#    days and Kislev has 30 days), and has Passover start on Saturday. */
my $nonleap_thursday_regular_diaspora =
[52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, D (21), 23, 24, -1, -1, 25, D (26), D (28), 30, D (31),
 33, 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49, 50];

# /* Hebrew year that starts on Thursday, is `regular' (Heshvan has 29
#    days and Kislev has 30 days), and has Passover start on Saturday. */
my $nonleap_thursday_regular_israel =
[52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, D (21), 23, 24, -1, 25, D (26), D (28), 30, 31, 32, 33,
 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49, 50];

# /* Hebrew year that starts on Thursday, is `complete' (Heshvan and
#    Kislev each have 30 days), and has Passover start on Sunday. */
my $nonleap_thursday_complete =
[52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, 21, 22, 23, 24, -1, 25, D (26), D (28), 30, D (31), 33,
 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49, 50];


# /* Hebrew year that starts on Saturday, is `incomplete' (Heshvan and Kislev
#    each have 29 days), and has Passover start on Sunday. */
my $nonleap_saturday_incomplete =
[-1, 52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
 15, 16, 17, 18, 19, 20, D (21), 23, 24, -1, 25, D (26), D (28), 30, D (31),
 33, 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49,
 50];


# /* Hebrew year that starts on Saturday, is `complete' (Heshvan and
#    Kislev each have 30 days), and has Passover start on Tuesday. */
my $nonleap_saturday_complete =
[-1, 52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
 16, 17, 18, 19, 20, D (21), 23, 24, -1, 25, D (26), D (28), 30,
 D (31), 33, 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47,
 48, 49, D (50)];

# /* --  The leap year types (keviot) -- */

# /* Hebrew year that starts on Monday, is `incomplete' (Heshvan and
#    Kislev each have 29 days), and has Passover start on Thursday. */
my $leap_monday_incomplete_diaspora =
[51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32, 33,
 -1, 34, 35, 36, 37, D (38), 40, D (41), 43, 44, 45, 46, 47, 48, 49,
 D (50)];

# /* Hebrew year that starts on Monday, is `incomplete' (Heshvan and
#    Kislev each have 29 days), and has Passover start on Thursday. */
my $leap_monday_incomplete_israel =
[51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32, 33,
 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49, D (50)];


# /* Hebrew year that starts on Monday, is `complete' (Heshvan and
#    Kislev each have 30 days), and has Passover start on Saturday. */
my $leap_monday_complete_diaspora =
[51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, -1, 28, 29, 30, 31, 32,
 33, 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49, 50];

# /* Hebrew year that starts on Monday, is `complete' (Heshvan and
#    Kislev each have 30 days), and has Passover start on Saturday. */
my $leap_monday_complete_israel =
[51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32, 33,
 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50];


# /* Hebrew year that starts on Tuesday, is `regular' (Heshvan has 29
#    days and Kislev has 30 days), and has Passover start on Saturday. */
my $leap_tuesday_regular_diaspora  = $leap_monday_complete_diaspora;

# /* Hebrew year that starts on Tuesday, is `regular' (Heshvan has 29
#    days and Kislev has 30 days), and has Passover start on Saturday. */
my $leap_tuesday_regular_israel  = $leap_monday_complete_israel;

# /* Hebrew year that starts on Thursday, is `incomplete' (Heshvan and
#    Kislev both have 29 days), and has Passover start on Sunday. */
my $leap_thursday_incomplete =
[52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, -1, 29, 30, 31, 32, 33,
 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50];

# /* Hebrew year that starts on Thursday, is `complete' (Heshvan and
#    Kislev both have 30 days), and has Passover start on Tuesday. */
my $leap_thursday_complete =
[52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, -1, 29, 30, 31, 32, 33,
 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, D (50)];

# /* Hebrew year that starts on Saturday, is `incomplete' (Heshvan and
#    Kislev each have 29 days), and has Passover start on Tuesday. */
my $leap_saturday_incomplete =
[-1, 52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32,
 33, 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49,
 D (50)];

# /* Hebrew year that starts on Saturday, is `complete' (Heshvan and
#    Kislev each have 30 days), and has Passover start on Thursday. */
my $leap_saturday_complete_diaspora =
[-1, 52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32,
 33, -1, 34, 35, 36, 37, D (38), 40, D (41), 43, 44, 45, 46, 47, 48, 49,
 D (50)];

# /* Hebrew year that starts on Saturday, is `complete' (Heshvan and
#    Kislev each have 30 days), and has Passover start on Thursday. */
my $leap_saturday_complete_israel = $leap_saturday_incomplete;

sub ROSH_DAY_INDEX( $ ) { ($_[0]==1) ? 0 : ($_[0]/2) }

# /* sedra_years_array[leap][rosh_day][type][israel/diaspora] */
my @sedra_years_array = 
(
  [				# nonleap years */

    [				# monday */
      [				# incomplete */
	$nonleap_monday_incomplete,
	$nonleap_monday_incomplete
      ],

      [				# regular */
 	0,0
      ],

      [				# complete */
	$nonleap_monday_complete_diaspora,
	$nonleap_monday_complete_israel
      ],

    ],

    [				# tuesday */
      [				# incomplete */
       0,0
      ],

      [				# regular */
	$nonleap_tuesday_regular_diaspora,
	$nonleap_tuesday_regular_israel

      ],

      [				# complete */
 	0, 0
      ],
    ],

    [				# thursday */
      [				# incomplete */
 	0, 0
      ],

      [				# regular */
	$nonleap_thursday_regular_diaspora,
	$nonleap_thursday_regular_israel
      ],

      [				# complete */
	$nonleap_thursday_complete,
	$nonleap_thursday_complete
      ],
    ],

    [				# saturday */
      [				# incomplete */
	$nonleap_saturday_incomplete,
	$nonleap_saturday_incomplete
      ],

      [				# regular */
  	0, 0
      ],

      [				# complete */
	$nonleap_saturday_complete, 
        $nonleap_saturday_complete
      ],
    ],
  ],


  [				# leap years */
    [				# monday */
      [				# incomplete */
	$leap_monday_incomplete_diaspora,
	$leap_monday_incomplete_israel
      ],

      [				# regular */
 	0, 0
      ],

      [				# complete */
	$leap_monday_complete_diaspora,
	$leap_monday_complete_israel
      ],
    ],

    [				# tuesday */
      [				# incomplete */
 	0, 0
      ],

      [				# regular */
	$leap_tuesday_regular_diaspora,
	$leap_tuesday_regular_israel,
      ],

      [				# complete */
 	0, 0
      ],
    ],

    [				# thursday */
      [				# incomplete */
	$leap_thursday_incomplete,
	$leap_thursday_incomplete
      ],

      [				# regular */
 	0, 0
      ],

      [				# complete */
	$leap_thursday_complete,
	$leap_thursday_complete
      ],
    ],

    [				# saturday */
      [				# incomplete */
	$leap_saturday_incomplete,
	$leap_saturday_incomplete
      ],

      [				# regular */
 	0, 0
      ],

      [				# complete */
	$leap_saturday_complete_diaspora,
	$leap_saturday_complete_israel
      ],
    ],
  ]
);

sub new (@){
   my $class = shift;
   my $self = bless {};
   if ( @_ ){
      $self->reset(@_);
   }
   return $self;
}

# sets static globals based on this year. */
sub reset($$$) {
   my $self = shift;
   my ($hebYr, $switches) = @_;
#   date_t tempDt;
#   int long_c, short_k, rosh_hashana_day, type;
#   long int rosh_hashana;

   my $long_c = Hebcal::long_cheshvan ($hebYr);
   my $short_k = Hebcal::short_kislev ($hebYr);
   my $type;

   my $theSedraArray;
   my $first_saturday;
   
   if ($long_c && !$short_k){
      $type = $COMPLETE;
   }
   elsif (!$long_c && $short_k){
      $type = $INCOMPLETE;
   }
   else{
      $type = $REGULAR;
   }
   
   my $tempDt = new Date();
   $tempDt->{dd} = 1;
   $tempDt->{mm} = $Hebcal::TISHREI;
   $tempDt->{yy} = $hebYr;
   my $rosh_hashana = Hebcal::hebrew2abs($tempDt);
   my $rosh_hashana_day =$rosh_hashana % 7;
   
   # find the first saturday on or after Rosh Hashana */
    $first_saturday = Hebcal::day_on_or_before (6, $rosh_hashana + 6);
   
   if (! ref ($theSedraArray = $sedra_years_array
	      [Hebcal::LEAP_YR_HEB ($hebYr)]
	      [ROSH_DAY_INDEX ($rosh_hashana_day)]
	      [$type]
	      [exists $switches->{'israeli-sedrot'}])){
      die ("improper sedra year type calculated.", "");
   }
   $self->{firstDOY} = $rosh_hashana;
   $self->{lastDOY}  = $rosh_hashana + Hebcal::days_in_heb_year($hebYr);
   $self->{theSedraArray} = $theSedraArray;
   $self->{first_saturday} = $first_saturday;
   $self->{switches} = $switches;
}



# returns a string "Parshat <parsha>" based on the current parsha number */
sub getSedra ($$){
    my ($self, $absDate) = @_;
    if ($absDate < $self->{firstDOY} 
	||  $absDate >= $self->{lastDOY}){
       $self->reset(Hebcal::abs2hebrew($absDate)->{yy});
    }
	
    # find the first saturday on or after today's date */
    $absDate = Hebcal::day_on_or_before( 6, $absDate + 6 );
    my $index = $self->{theSedraArray}[int(($absDate - $self->{first_saturday}) / 7)];
    if ($index >= 0){
       return $sedrot[$index][exists $self->{switches}{ashkenazis}];
    }
    elsif (-1 == $index){
	return undef;
    }
    else
    {
      my $index = U($index);	# undouble the parsha */
      return sprintf ("%s-%s",
	       $sedrot[$index][exists $self->{switches}{ashkenazis}],
	       $sedrot[$index + 1][exists $self->{switches}{ashkenazis}]);
    }
}

##################################################
#routines for handling gregorian dates

package Hebcal;
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


#  *Return the day number within the year of the date DATE.
#  *For example, dayOfYear({1,1,1987}) returns the value 1
#  *while dayOfYear({12,31,1980}) returns 366.
#  
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



#  * The number of days elapsed between the Gregorian date 12/31/1 BC and DATE.
#  * The Gregorian date Sunday, December 31, 1 BC is imaginary.
sub greg2abs( $ )#			/* "absolute date" 
{
 my $d = shift;
 return ( dayOfYear($d)	#/* days this year 
	  + 365 *  ($d->{yy} - 1)#	/* + days in prior years 
	  +  int(($d->{yy} - 1) / 4)#	/* + Julian Leap years 
	  -  int(($d->{yy} - 1) / 100)#	/* - century years 
	  +  int(($d->{yy} - 1) / 400));#	/* + Gregorian leap years 
}


#  * See the footnote on page 384 of ``Calendrical Calculations, Part II:
#  * Three Historical Calendars'' by E. M. Reingold,  N. Dershowitz, and S. M.
#  * Clamen, Software--Practice and Experience, Volume 23, Number 4
#  * (April, 1993), pages 383-404 for an explanation.
sub abs2greg ($)
{
   my $theDate = shift;
   
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


# sunday = 0 
sub dayOfWeek ($ ){
   return (greg2abs(shift) % 7);
}
	       
sub setDate ($ ){
   my $d = shift;
   my($day, $mon, $yr) = (localtime)[3,4,5];
   $yr+=1900;
   $mon++;
   $d->{dd} = $day;
   $d->{mm} = $mon;
   $d->{yy} = $yr;
}


# /* Returns the absolute date of the DAYNAME on or before absolute DATE.
#  * DAYNAME=0 means Sunday, DAYNAME=1 means Monday, and so on.

#  * Note: Applying this function to d+6 gives us the DAYNAME on or after an
#  * absolute day d.  Similarly, applying it to d+3 gives the DAYNAME nearest to
#  * absolute date d, applying it to d-1 gives the DAYNAME previous to absolute
#  * date d, and applying it to d+7 gives the DAYNAME following absolute date d.
#  
sub day_on_or_before ($$){
   my ($day_of_week, $date) = @_;
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



################ from hebcal.c
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

use File::Basename;
use lib dirname($0);

package Hebcal;
use strict; 

use Getopt::Long;
use Carp;

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

# require "greg.pl";
# require "rise.pl";
# require "holidays.pl";
# require "jewish.pl";

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
	 if ($switches->{printOmer}){
	    ($beginOmer, $endOmer) = getOmerBounds ($todayHeb->{yy});
	 }
	 init_holidays($todayHeb->{yy});
      }

    }
}

# use Holiday;
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


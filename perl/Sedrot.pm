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

1;

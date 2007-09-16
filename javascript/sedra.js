// $Header$

/*
   Hebcal Javascript-mode - A Jewish Calendar Generator
   Copyright (C) 2007  Danny Sadinoff

   Hebcal is distributed under the GNU Public License.  The program
   and its source code may be freely distributed.  For details, see
   the file COPYING in the distribution.

Hebcal is, at root, a port of the GNU Emacs Calendar lisp
code by Edward M. Reingold and Nachum Dershowitz.
http://emr.cs.iit.edu/~reingold/ 
http://www.math.tau.ac.il/~nachumd/


Michael Radwin has made significant contributions as a result of
maintaining hebcal.com.
*/   

Sedra.TYPE_NAMES = [
'INCOMPLETE',
'REGULAR ',
'COMPLETE',
];

Sedra.INCOMPLETE  = 0;
Sedra.REGULAR  =1;
Sedra.COMPLETE  = 2;

function Sedra ( hebYr, israel_sw ){ /* the hebrew year */
//     var tempDt;
//     int long_c, short_k, rosh_hashana_day, type;
//     long int rosh_hashana;
    
    var long_c = long_cheshvan (hebYr);
    var short_k = short_kislev (hebYr);
    var type;
    this.year = hebYr;
    if (long_c && !short_k)
        type = Sedra.COMPLETE;
    else if (!long_c && short_k)
        type = Sedra.INCOMPLETE;
    else
        type = Sedra.REGULAR;

    var tempDt = new HebcalHebrewDate( hebYr,HebcalHebrewDate.TISHREI,1);
    var rosh_hashana = hebrew2abs (tempDt);
    var rosh_hashana_day = rosh_hashana % 7;

    /* find the first saturday on or after Rosh Hashana */
    this.first_saturday = day_on_or_before (6, rosh_hashana + 6);
    var leap = +LEAP_YR_HEB(hebYr);
    this.type = type;
    this.rosh_hashana_day = rosh_hashana_day;
    this.leap = leap;
    this.israel_sw = israel_sw;

    this.theSedraArray = Sedra.sedra_years_array
        [leap]
        [Sedra.ROSH_DAY_INDEX(rosh_hashana_day)]
        [type]
        [+israel_sw];
    if ( null == this.theSedraArray )
        alert ("improper sedra year type calculated."
               +" leap = "+leap
               +" rdi = "+Sedra.ROSH_DAY_INDEX(rosh_hashana_day)
               +" type="+type
               +" israel_sw="+israel_sw, "");
}

Sedra.prototype.toString = function(){
    return "[ "
    +"year="+this.year
    +", leap=" + this.leap
    +", RH-day="+ Hebcal.WKDAY_NAMES[this.rosh_hashana_day]
    +", type=" + Sedra.TYPE_NAMES[this.type]
    +", israel_sw="+this.israel_sw
    +"]";
};



Sedra.parshiot = 
[
{ en : "Bereshit", he:"בראשית"}, //0
{ en:"Noach",       he:"נח"},
{ en:"Lech-Lecha",  he: "לך-לך"},
{ en:"Vayera",      he: "וירא"},
{ en:"Chayei Sara", he: "חי שרה"},
{ en:"Toldot",      he: "תולדות"},  //5
{ en:"Vayetzei",    he:"ויצא"},
{ en:"Vayishlach",  he:"וישלח"},
{ en:"Vayeshev",    he:"וישב"},
{ en:"Miketz",      he:"מקץ"},
{ en:"Vayigash",    he:"ויגש"},   //10
{ en:"Vayechi",     he:"ויחי"},
{ en:"Shemot",      he:"שמות"},
{ en:"Vaera",       he:"וארא"},
{ en:"Bo",  he:"בא"},
{ en:"Beshalach",   he:"בשלח"},  //15
{ en:"Yitro",       he:"יתרו"},
{ en:"Mishpatim",   he:"משפטים"},
{ en:"Terumah",     he:"תרומה"},
{ en:"Tetzaveh",    he:"תצוה"},
{ en:"Ki Tisa",     he:"כי תשא"}, //20
{ en:"Vayakhel",    he:"ויקהל"},
{ en:"Pekudei",     he:"פקודי"},  //22
{ en:"Vayikra",     he:"ויקרא"},  //23
{ en:"Tzav", he:"צו"}, //24
{ en:"Shmini",      he:"שמיני"}, //25
{ en:"Tazria",      he:"תזריע"}, //26
{ en:"Metzora",     he:"מצרע"}, //27
{ en:"Achrei Mot",  he:"אחרי מות"}, //28
{ en:"Kedoshim",    he:"קדשים"}, //29
{ en:"Emor", he:"אמור"},
{ en:"Behar",       he:"בהר"},
{ en:"Bechukotai",  he:"בחקתי"},
{ en:"Bamidbar",    he:"במדבר"},
{ en:"Nasso",       he:"נשא"},
{ en:"Beha'alotcha",he: "בהעלתך"},
{ en:"Sh'lach",     he: "שלח לך"},
{ en:"Korach",      he:"קורח"},
{ en:"Chukat",      he:"חקת"},
{ en:"Balak",       he:"בלק"},
{ en:"Pinchas",     he:"פינחס"},
{ en:"Matot",       he:"מטות"},
{ en:"Masei",       he:"מסעי"},
{ en:"Devarim",     he:"דברים"},
{ en:"Vaetchanan",  he:"ואתחנן"},
{ en:"Eikev",       he:"עקב"},
{ en:"Re'eh",       he:"ראה"},
{ en:"Shoftim",     he:"שופטים"},
{ en:"Ki Teitzei",  he:"כי תצא"},
{ en:"Ki Tavo",     he:"כי תבוא"},
{ en:"Nitzavim",    he:"נצבים"},
{ en:"Vayeilech",   he:"וילך"},
{ en:"Ha'Azinu",    he:"האזינו"},
];


/* parsha doubler */
function D(p) { return -p ;} 

/* parsha undoubler */
function U(p) { return -p; } 

var  RH = { en: "Rosh Hashana" , he: "ראש השנה"}; //0
var YK  =  { en: "Yom Kippur" , he: "יום כיפור"};  //1
var EOY  =  { en: "End-Of-Year: simchat-torah, sukkot" , he: "סופשנה: סוכות וסמחת תורה"};  //0

var SUKKOT  =  { en: "Sukkot" , he: "סוכות"};  //0
var CHMSUKOT  =  { en: "Chol hamoed Sukkot" , he: "חול המועד סוכות"};  //0
var SHMINI  =  { en: "Shmini Atzeret" , he: "שמיני עצרת"};  //0

var PESACH = { en: "Pesach" , he: "פסח"}; //25
var CHMPESACH  =  { en: "Chol hamoed Pesach" , he: "חול המועד פסח"};  //25
var PESACH7 = { en: "Second days of Pesach" , he: "שביעי של פסח"}; //25

var  SHAVUOT = { en: "Shavuot" , he: "שבועות"}; //33




/* The ordinary year types (keviot) */

/* Hebrew year that starts on Monday, is `incomplete' (Heshvan and
   Kislev each have 29 days), and has Passover start on Tuesday. */
//e.g. 5753
Sedra.nonleap_monday_incomplete =
[51, 52, EOY, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, D (21), 23, 24, PESACH, 25, D (26), D (28), 30, D (31), 33, 34,
 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49, D (50)];

/* Hebrew year that starts on Monday, is `complete' (Heshvan and
   Kislev each have 30 days), and has Passover start on Thursday. */
//e.g. 5756
Sedra.nonleap_monday_complete_diaspora =
[51, 52, EOY, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, D (21), 23, 24, PESACH, 25, D (26), D (28), 30, D (31), 33,
 SHAVUOT, 34, 35, 36, 37, D (38), 40, D (41), 43, 44, 45, 46, 47, 48, 49,
 D (50)];

/* Hebrew year that starts on Monday, is `complete' (Heshvan and
   Kislev each have 30 days), and has Passover start on Thursday. */
Sedra.nonleap_monday_complete_israel = Sedra.nonleap_monday_incomplete

/* Hebrew year that starts on Tuesday, is `regular' (Heshvan has 29
   days and Kislev has 30 days), and has Passover start on Thursday. */
//e.g. 5715
Sedra.nonleap_tuesday_regular_diaspora = Sedra.nonleap_monday_complete_diaspora

/* Hebrew year that starts on Tuesday, is `regular' (Heshvan has 29
   days and Kislev has 30 days), and has Passover start on Thursday. */
Sedra.nonleap_tuesday_regular_israel = Sedra.nonleap_monday_incomplete

/* Hebrew year that starts on Thursday, is `regular' (Heshvan has 29
   days and Kislev has 30 days), and has Passover start on Saturday. */
//e.g. 5701
Sedra.nonleap_thursday_regular_diaspora =
[52, YK, EOY, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, D (21), 23, 24, PESACH, PESACH, 25, D (26), D (28), 30, D (31),
 33, 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49, 50];

/* Hebrew year that starts on Thursday, is `regular' (Heshvan has 29
   days and Kislev has 30 days), and has Passover start on Saturday. */
// e.g. 5745
Sedra.nonleap_thursday_regular_israel =
[52, YK, EOY, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, D (21), 23, 24, PESACH, 25, D (26), D (28), 30, 31, 32, 33,
 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49, 50];

/* Hebrew year that starts on Thursday, is `complete' (Heshvan and
   Kislev each have 30 days), and has Passover start on Sunday. */
//e.g. 5754
Sedra.nonleap_thursday_complete =
[52, YK, CHMSUKOT, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, 21, 22, 23, 24, PESACH7, 25, D (26), D (28), 30, D (31), 33,
 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49, 50];

/* Hebrew year that starts on Saturday, is `incomplete' (Heshvan and Kislev
   each have 29 days), and has Passover start on Sunday. */
//e.g. 5761
Sedra.nonleap_saturday_incomplete =
[RH, 52, SUKKOT, SHMINI, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
 15, 16, 17, 18, 19, 20, D (21), 23, 24, PESACH7, 25, D (26), D (28), 30, D (31),
 33, 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49,
 50];


/* Hebrew year that starts on Saturday, is `complete' (Heshvan and
   Kislev each have 30 days), and has Passover start on Tuesday. */
//e.g. 5716
Sedra.nonleap_saturday_complete =
[RH, 52, SUKKOT, SHMINI, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
 16, 17, 18, 19, 20, D (21), 23, 24, CHMPESACH, 25, D (26), D (28), 30,
 D (31), 33, 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47,
 48, 49, D (50)];


/* --  The leap year types (keviot) -- */
/* Hebrew year that starts on Monday, is `incomplete' (Heshvan and
   Kislev each have 29 days), and has Passover start on Thursday. */
//e.g. 5746
Sedra.leap_monday_incomplete_diaspora =
[51, 52, null, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, null, 28, 29, 30, 31, 32, 33,
 null, 34, 35, 36, 37, D (38), 40, D (41), 43, 44, 45, 46, 47, 48, 49,
 D (50)];

/* Hebrew year that starts on Monday, is `incomplete' (Heshvan and
   Kislev each have 29 days), and has Passover start on Thursday. */
//e.g. 5746
Sedra.leap_monday_incomplete_israel =
[51, 52, CHMSUKOT, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, CHMPESACH, 28, 29, 30, 31, 32, 33,
 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49, D (50)];

/* Hebrew year that starts on Monday, is `complete' (Heshvan and
   Kislev each have 30 days), and has Passover start on Saturday. */
//e.g.5752
Sedra.leap_monday_complete_diaspora =
[51, 52, CHMSUKOT, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, PESACH, PESACH, 28, 29, 30, 31, 32,
 33, 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49, 50];

/* Hebrew year that starts on Monday, is `complete' (Heshvan and
   Kislev each have 30 days), and has Passover start on Saturday. */
//e.g.5752
Sedra.leap_monday_complete_israel =
[51, 52, CHMSUKOT, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, PESACH, 28, 29, 30, 31, 32, 33,
 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50];

/* Hebrew year that starts on Tuesday, is `regular' (Heshvan has 29
   days and Kislev has 30 days), and has Passover start on Saturday. */
// e.g. 5715
Sedra.leap_tuesday_regular_diaspora = Sedra.leap_monday_complete_diaspora

/* Hebrew year that starts on Tuesday, is `regular' (Heshvan has 29
   days and Kislev has 30 days), and has Passover start on Saturday. */
Sedra.leap_tuesday_regular_israel= Sedra.leap_monday_complete_israel;

/* Hebrew year that starts on Thursday, is `incomplete' (Heshvan and
   Kislev both have 29 days), and has Passover start on Sunday. */
//e.g. 5768
Sedra.leap_thursday_incomplete =
[52, YK, CHMSUKOT, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, PESACH7, 29, 30, 31, 32, 33,
 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50];

/* Hebrew year that starts on Thursday, is `complete' (Heshvan and
   Kislev both have 30 days), and has Passover start on Tuesday. */
//eg. 5771
Sedra.leap_thursday_complete =
[52, YK, CHMSUKOT, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, CHMPESACH, 29, 30, 31, 32, 33,
 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, D (50)];

/* Hebrew year that starts on Saturday, is `incomplete' (Heshvan and
   Kislev each have 29 days), and has Passover start on Tuesday. */
//e.g.5757
Sedra.leap_saturday_incomplete =
[RH, 52, SUKKOT, SHMINI, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, CHMPESACH, 28, 29, 30, 31, 32,
 33, 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49,
 D (50)];

/* Hebrew year that starts on Saturday, is `complete' (Heshvan and
   Kislev each have 30 days), and has Passover start on Thursday. */
Sedra.leap_saturday_complete_diaspora =
[RH, 52, null, null, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, null, 28, 29, 30, 31, 32,
 33, null, 34, 35, 36, 37, D (38), 40, D (41), 43, 44, 45, 46, 47, 48, 49,
 D (50)];

/* Hebrew year that starts on Saturday, is `complete' (Heshvan and
   Kislev each have 30 days), and has Passover start on Thursday. */
Sedra.leap_saturday_complete_israel = Sedra.leap_saturday_incomplete;



Sedra.ROSH_DAY_INDEX = function(x) {return  (x==1) ? 0 : (x/2) };

/* sedra_years_array[leap][rosh_day][type][israel/diaspora] */
Sedra.sedra_years_array =  /* [2][4][3][2] */ 
[
    [                             /* nonleap years */
        
        [                           /* monday */
            [                         /* incomplete */
             Sedra.nonleap_monday_incomplete,
             Sedra.nonleap_monday_incomplete
            ],
            
            [                         /* regular */
                null, null
            ],
            
            [                         /* complete */
                Sedra.nonleap_monday_complete_diaspora,
                Sedra.nonleap_monday_complete_israel
            ],
            
        ],
        
        [                           /* tuesday */
            [                         /* incomplete */
                null, null
            ],
            
            [                         /* regular */   //e.g. 5715
                Sedra.nonleap_tuesday_regular_diaspora,
                Sedra.nonleap_tuesday_regular_israel 
                
            ],
            
            [                         /* complete */
                null, null
            ],
        ],
        
        [                           /* thursday */
            [                         /* incomplete */
                null, null
            ],
            
            [                         /* regular */  //e.g. 5745
                Sedra.nonleap_thursday_regular_diaspora,
                Sedra.nonleap_thursday_regular_israel
            ],
            
            [                         /* complete */
                Sedra.nonleap_thursday_complete,
                Sedra.nonleap_thursday_complete
            ],
        ],
        
        [                           /* saturday */
            [                         /* incomplete */
                Sedra.nonleap_saturday_incomplete,
                Sedra.nonleap_saturday_incomplete
            ],
            
            [                         /* regular */
                null, null
            ],
            
            [                         /* complete */
                Sedra.nonleap_saturday_complete, Sedra.nonleap_saturday_complete  //e.g. 5716
            ],
        ],
    ],
    
    
    [                             /* leap years */
        [                           /* monday */
            [                         /* incomplete */ //e.g. 5746
                Sedra.leap_monday_incomplete_diaspora,
                Sedra.leap_monday_incomplete_israel
            ],
            
            [                         /* regular */
                null, null
            ],
            
            [                         /* complete */
                Sedra.leap_monday_complete_diaspora,
                Sedra.leap_monday_complete_israel
            ],
        ],
        
        [                           /* tuesday */
            [                         /* incomplete */
             null, null
            ],
            
            [                         /* regular */
                Sedra.leap_tuesday_regular_diaspora,
                Sedra.leap_tuesday_regular_israel,
            ],
            
            [                         /* complete */
                null, null
            ],
        ],
        
    [                           /* thursday */
        [                         /* incomplete */
            Sedra.leap_thursday_incomplete,
            Sedra.leap_thursday_incomplete
        ],
        
        [                         /* regular */
            null, null
        ],
        
        [                         /* complete */
            Sedra.leap_thursday_complete,
            Sedra.leap_thursday_complete
        ],
    ],
        
        [                           /* saturday */
            [                         /* incomplete */
                Sedra.leap_saturday_incomplete,
                Sedra.leap_saturday_incomplete
            ],
            
            [                         /* regular */
                null, null
            ],
            
            [                         /* complete */
                Sedra.leap_saturday_complete_diaspora,
                Sedra.leap_saturday_complete_israel
            ],
        ],
    ]
];

/* Note: Applying this function to d+6 gives us the DAYNAME on or after an
 * absolute day d.  Similarly, applying it to d+3 gives the DAYNAME nearest to
 * absolute date d, applying it to d-1 gives the DAYNAME previous to absolute
 * date d, and applying it to d+7 gives the DAYNAME following absolute date d.

**/
function day_on_or_before(  day_of_week, absdate)
{
  return absdate - ((absdate - day_of_week) % 7);
}

Sedra.prototype.getSedraFromHebcalDate = function( hDate ){
    assert("getsedrafromhebcaldate",hDate instanceof HebcalHebrewDate);
    return this.getSedraFromAbsDate(hebrew2abs(hDate));
}

/* returns a string describing the parsha on the first saturday on or after absdate*/
//FIX: ignores holidays on the birthday thru friday.
Sedra.prototype.getSedraFromAbsDate = function( absBirthdayDate)
{

    /* find the first saturday on or after today's date */
    var absDate = day_on_or_before (6, absBirthdayDate + 6);
    
    var weekNum = (absDate - this.first_saturday) / 7;
    var index = this.theSedraArray[weekNum];
    
    if (null == index)
        return null;
    if( typeof(index) == 'object'){
        // Shabbat has a chag.  return a description, plus the subsequent shabbat's chag.  
        // runs twice if necessary
        return index['he'] +", " +this.getSedraFromAbsDate(absDate +7);
    }
    if (index >= 0)
        return  Sedra.parshiot[index]['he'];
    
    var i = U (index);      /* undouble the parsha */
    var p1 = Sedra.parshiot[i];
    var p2 = Sedra.parshiot[i+1];
    return p1['he'] + "-" + p2['he'];
}



/*
	Hebcal - A Jewish Calendar Generator
	Copyright (C) 1994-2004  Danny Sadinoff
	Portions Copyright (c) 2002 Michael J. Radwin. All Rights Reserved.

	https://github.com/hebcal/hebcal

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Danny Sadinoff can be reached at 
	danny@sadinoff.com

	Michael Radwin has made significant contributions as a result of
	maintaining hebcal.com.

	The JavaScript code was completely rewritten in 2014 by Eyal Schachter
 */
/*
 * Many of the following algorithms were taken from hebrew calendar
 * routines by Maimonedes, from his Mishneh Torah, and implemented by
 *  Nachum Dershowitz                Department of Computer Science
 *  (217) 333-4219                   University of Illinois at Urbana-Champaign
 *  nachum@cs.uiuc.edu               1304 West Springfield Avenue
 *                                   Urbana, Illinois 61801
 *
 * The routines were included in the emacs 19 distribution.
 *
 */
var c = require('./common');

module.exports = function(HDate) {
	var TYPE_NAMES = [
		'INCOMPLETE',
		'REGULAR ',
		'COMPLETE'
	],

	INCOMPLETE = 0,
	REGULAR = 1,
	COMPLETE = 2;

	function Sedra (hebYr, israel_sw) { /* the hebrew year */
		israel_sw = !!israel_sw;
		var long_c = c.long_cheshvan (hebYr);
		var short_k = c.short_kislev (hebYr);
		var type;
		this.year = hebYr;
		if (long_c && !short_k) {
			type = COMPLETE;
		} else if (!long_c && short_k) {
			type = INCOMPLETE;
		} else {
			type = REGULAR;
		}

		var tempDt = new HDate(1,c.months.TISHREI,hebYr);
		var rosh_hashana = HDate.hebrew2abs(tempDt);
		var rosh_hashana_day = rosh_hashana % 7;

		/* find the first saturday on or after Rosh Hashana */
		this.first_saturday = c.day_on_or_before(6, rosh_hashana + 6);
		var leap = +c.LEAP_YR_HEB(hebYr);
		this.type = type;
		this.rosh_hashana_day = rosh_hashana_day;
		this.leap = leap;
		this.israel_sw = israel_sw;

		this.theSedraArray = Sedra.sedra_years_array
			[leap]
			[ROSH_DAY_INDEX(rosh_hashana_day)]
			[type]
			[+israel_sw];
		if (null === this.theSedraArray) {
			console.log(this);
			throw new TypeError("improper sedra year type calculated.");
		}
	}



	Sedra.parshiot = [
		[ 'Bereshit', null, 'בראשית' ],
		[ 'Noach', null, 'נח' ],
		[ 'Lech-Lecha', null, 'לך-לך' ],
		[ 'Vayera', null, 'וירא' ],
		[ 'Chayei Sara', null, 'חי שרה' ],
		[ 'Toldot', null, 'תולדות' ],
		[ 'Vayetzei', null, 'ויצא' ],
		[ 'Vayishlach', null, 'וישלח' ],
		[ 'Vayeshev', null, 'וישב' ],
		[ 'Miketz', null, 'מקץ' ],
		[ 'Vayigash', null, 'ויגש' ],
		[ 'Vayechi', null, 'ויחי' ],
		[ 'Shemot', null, 'שמות' ],
		[ 'Vaera', null, 'וארא' ],
		[ 'Bo', null, 'בא' ],
		[ 'Beshalach', null, 'בשלח' ],
		[ 'Yitro', null, 'יתרו' ],
		[ 'Mishpatim', null, 'משפטים' ],
		[ 'Terumah', null, 'תרומה' ],
		[ 'Tetzaveh', null, 'תצוה' ],
		[ 'Ki Tisa', null, 'כי תשא' ],
		[ 'Vayakhel', null, 'ויקהל' ],
		[ 'Pekudei', null, 'פקודי' ],
		[ 'Vayikra', null, 'ויקרא' ],
		[ 'Tzav', null, 'צו' ],
		[ 'Shmini', null, 'שמיני' ],
		[ 'Tazria', null, 'תזריע' ],
		[ 'Metzora', null, 'מצרע' ],
		[ 'Achrei Mot', null, 'אחרי מות' ],
		[ 'Kedoshim', null, 'קדשים' ],
		[ 'Emor', null, 'אמור' ],
		[ 'Behar', null, 'בהר' ],
		[ 'Bechukotai', null, 'בחקתי' ],
		[ 'Bamidbar', null, 'במדבר' ],
		[ 'Nasso', null, 'נשא' ],
		[ 'Beha\'alotcha', null, 'בהעלתך' ],
		[ 'Sh\'lach', null, 'שלח לך' ],
		[ 'Korach', null, 'קורח' ],
		[ 'Chukat', null, 'חקת' ],
		[ 'Balak', null, 'בלק' ],
		[ 'Pinchas', null, 'פינחס' ],
		[ 'Matot', null, 'מטות' ],
		[ 'Masei', null, 'מסעי' ],
		[ 'Devarim', null, 'דברים' ],
		[ 'Vaetchanan', null, 'ואתחנן' ],
		[ 'Eikev', null, 'עקב' ],
		[ 'Re\'eh', null, 'ראה' ],
		[ 'Shoftim', null, 'שופטים' ],
		[ 'Ki Teitzei', null, 'כי תצא' ],
		[ 'Ki Tavo', null, 'כי תבוא' ],
		[ 'Nitzavim', null, 'נצבים' ],
		[ 'Vayeilech', null, 'וילך' ],
		[ 'Ha\'Azinu', null, 'האזינו' ]
	];


	/* parsha doubler */
	function D(p) { return -p ;} 

	/* parsha undoubler */
	function U(p) { return -p; } 

	var RH = [ 'Rosh Hashana', null, 'ראש השנה' ]; //0
	var YK  =  [ 'Yom Kippur', null, 'יום כיפור' ];  //1
	var EOY  =  [ 'End-of-Year: Simchat-Torah, Sukkot', 'End-of-Year: Simchas-Torah, Succos', 'סופשנה: סוכות וסמחת תורה' ];  //0

	var SUKKOT  =  [ 'Sukkot', 'Succos', 'סוכות' ];  //0
	var CHMSUKOT  =  [ 'Chol hamoed Sukkot', 'Chol hamoed Succos', 'חול המועד סוכות' ];  //0
	var SHMINI  =  [ 'Shmini Atzeret', 'Shmini Atzeres', 'שמיני עצרת' ];  //0

	var PESACH = [ 'Pesach', null, 'פסח' ]; //25
	var CHMPESACH  =  [ 'Chol hamoed Pesach', null, 'חול המועד פסח' ];  //25
	var PESACH7 = [ 'Second days of Pesach', null, 'שביעי של פסח' ]; //25

	var SHAVUOT = [ 'Shavuot', 'Shavuos', 'שבועות' ]; //33




	// The ordinary year types (keviot)

	/* Hebrew year that starts on Monday, is `incomplete' (Heshvan and
	 * Kislev each have 29 days), and has Passover start on Tuesday. */
	//e.g. 5753
	Sedra.nonleap_monday_incomplete =
	[51, 52, EOY, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
	 17, 18, 19, 20, D (21), 23, 24, PESACH, 25, D (26), D (28), 30, D (31), 33, 34,
	 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49, D (50)];

	/* Hebrew year that starts on Monday, is `complete' (Heshvan and
	 * Kislev each have 30 days), and has Passover start on Thursday. */
	//e.g. 5756
	Sedra.nonleap_monday_complete_diaspora =
	[51, 52, EOY, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
	 17, 18, 19, 20, D (21), 23, 24, PESACH, 25, D (26), D (28), 30, D (31), 33,
	 SHAVUOT, 34, 35, 36, 37, D (38), 40, D (41), 43, 44, 45, 46, 47, 48, 49,
	 D (50)];

	/* Hebrew year that starts on Monday, is `complete' (Heshvan and
	 * Kislev each have 30 days), and has Passover start on Thursday. */
	Sedra.nonleap_monday_complete_israel = Sedra.nonleap_monday_incomplete

	/* Hebrew year that starts on Tuesday, is `regular' (Heshvan has 29
	 * days and Kislev has 30 days), and has Passover start on Thursday. */
	//e.g. 5715
	Sedra.nonleap_tuesday_regular_diaspora = Sedra.nonleap_monday_complete_diaspora

	/* Hebrew year that starts on Tuesday, is `regular' (Heshvan has 29
	 * days and Kislev has 30 days), and has Passover start on Thursday. */
	Sedra.nonleap_tuesday_regular_israel = Sedra.nonleap_monday_incomplete

	/* Hebrew year that starts on Thursday, is `regular' (Heshvan has 29
	 * days and Kislev has 30 days), and has Passover start on Saturday. */
	//e.g. 5701
	Sedra.nonleap_thursday_regular_diaspora =
	[52, YK, EOY, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
	 17, 18, 19, 20, D (21), 23, 24, PESACH, PESACH, 25, D (26), D (28), 30, D (31),
	 33, 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49, 50];

	/* Hebrew year that starts on Thursday, is `regular' (Heshvan has 29
	 * days and Kislev has 30 days), and has Passover start on Saturday. */
	// e.g. 5745
	Sedra.nonleap_thursday_regular_israel =
	[52, YK, EOY, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
	 17, 18, 19, 20, D (21), 23, 24, PESACH, 25, D (26), D (28), 30, 31, 32, 33,
	 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49, 50];

	/* Hebrew year that starts on Thursday, is `complete' (Heshvan and
	 * Kislev each have 30 days), and has Passover start on Sunday. */
	//e.g. 5754
	Sedra.nonleap_thursday_complete =
	[52, YK, CHMSUKOT, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
	 17, 18, 19, 20, 21, 22, 23, 24, PESACH7, 25, D (26), D (28), 30, D (31), 33,
	 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49, 50];

	/* Hebrew year that starts on Saturday, is `incomplete' (Heshvan and Kislev
	 * each have 29 days), and has Passover start on Sunday. */
	//e.g. 5761
	Sedra.nonleap_saturday_incomplete =
	[RH, 52, SUKKOT, SHMINI, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
	 15, 16, 17, 18, 19, 20, D (21), 23, 24, PESACH7, 25, D (26), D (28), 30, D (31),
	 33, 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49,
	 50];


	/* Hebrew year that starts on Saturday, is `complete' (Heshvan and
	 * Kislev each have 30 days), and has Passover start on Tuesday. */
	//e.g. 5716
	Sedra.nonleap_saturday_complete =
	[RH, 52, SUKKOT, SHMINI, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	 16, 17, 18, 19, 20, D (21), 23, 24, CHMPESACH, 25, D (26), D (28), 30,
	 D (31), 33, 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47,
	 48, 49, D (50)];


	/* --  The leap year types (keviot) -- */
	/* Hebrew year that starts on Monday, is `incomplete' (Heshvan and
	 * Kislev each have 29 days), and has Passover start on Thursday. */
	//e.g. 5746
	Sedra.leap_monday_incomplete_diaspora =
	[51, 52, null, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
	 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, null, 28, 29, 30, 31, 32, 33,
	 null, 34, 35, 36, 37, D (38), 40, D (41), 43, 44, 45, 46, 47, 48, 49,
	 D (50)];

	/* Hebrew year that starts on Monday, is `incomplete' (Heshvan and
	 * Kislev each have 29 days), and has Passover start on Thursday. */
	//e.g. 5746
	Sedra.leap_monday_incomplete_israel =
	[51, 52, CHMSUKOT, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
	 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, CHMPESACH, 28, 29, 30, 31, 32, 33,
	 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49, D (50)];

	/* Hebrew year that starts on Monday, is `complete' (Heshvan and
	 * Kislev each have 30 days), and has Passover start on Saturday. */
	//e.g.5752
	Sedra.leap_monday_complete_diaspora =
	[51, 52, CHMSUKOT, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
	 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, PESACH, PESACH, 28, 29, 30, 31, 32,
	 33, 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49, 50];

	/* Hebrew year that starts on Monday, is `complete' (Heshvan and
	 * Kislev each have 30 days), and has Passover start on Saturday. */
	//e.g.5752
	Sedra.leap_monday_complete_israel =
	[51, 52, CHMSUKOT, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
	 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, PESACH, 28, 29, 30, 31, 32, 33,
	 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50];

	/* Hebrew year that starts on Tuesday, is `regular' (Heshvan has 29
	 * days and Kislev has 30 days), and has Passover start on Saturday. */
	// e.g. 5715
	Sedra.leap_tuesday_regular_diaspora = Sedra.leap_monday_complete_diaspora

	/* Hebrew year that starts on Tuesday, is `regular' (Heshvan has 29
	 * days and Kislev has 30 days), and has Passover start on Saturday. */
	Sedra.leap_tuesday_regular_israel= Sedra.leap_monday_complete_israel;

	/* Hebrew year that starts on Thursday, is `incomplete' (Heshvan and
	 * Kislev both have 29 days), and has Passover start on Sunday. */
	//e.g. 5768
	Sedra.leap_thursday_incomplete =
	[52, YK, CHMSUKOT, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
	 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, PESACH7, 29, 30, 31, 32, 33,
	 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50];

	/* Hebrew year that starts on Thursday, is `complete' (Heshvan and
	 * Kislev both have 30 days), and has Passover start on Tuesday. */
	//eg. 5771
	Sedra.leap_thursday_complete =
	[52, YK, CHMSUKOT, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
	 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, CHMPESACH, 29, 30, 31, 32, 33,
	 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, D (50)];

	/* Hebrew year that starts on Saturday, is `incomplete' (Heshvan and
	 * Kislev each have 29 days), and has Passover start on Tuesday. */
	//e.g.5757
	Sedra.leap_saturday_incomplete =
	[RH, 52, SUKKOT, SHMINI, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, CHMPESACH, 28, 29, 30, 31, 32,
	 33, 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49,
	 D (50)];

	/* Hebrew year that starts on Saturday, is `complete' (Heshvan and
	 * Kislev each have 30 days), and has Passover start on Thursday. */
	Sedra.leap_saturday_complete_diaspora =
	[RH, 52, null, null, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, null, 28, 29, 30, 31, 32,
	 33, null, 34, 35, 36, 37, D (38), 40, D (41), 43, 44, 45, 46, 47, 48, 49,
	 D (50)];

	/* Hebrew year that starts on Saturday, is `complete' (Heshvan and
	 * Kislev each have 30 days), and has Passover start on Thursday. */
	Sedra.leap_saturday_complete_israel = Sedra.leap_saturday_incomplete;



	function ROSH_DAY_INDEX(x) {
		return  (x === 1) ? 0 : x / 2;
	}

	// sedra_years_array[leap][rosh_day][type][israel/diaspora]
	Sedra.sedra_years_array =  // [2][4][3][2]
	[
		[                             // nonleap years
			
			[                           // monday
				[                         // incomplete
				 Sedra.nonleap_monday_incomplete,
				 Sedra.nonleap_monday_incomplete
				],
				
				[                         // regular
					null, null
				],
				
				[                         // complete
					Sedra.nonleap_monday_complete_diaspora,
					Sedra.nonleap_monday_complete_israel
				]
				
			],
			
			[                           // tuesday
				[                         // incomplete
					null, null
				],
				
				[                         // regular   //e.g. 5715
					Sedra.nonleap_tuesday_regular_diaspora,
					Sedra.nonleap_tuesday_regular_israel 
					
				],
				
				[                         // complete
					null, null
				]
			],
			
			[                           // thursday
				[                         // incomplete
					null, null
				],
				
				[                         // regular  //e.g. 5745
					Sedra.nonleap_thursday_regular_diaspora,
					Sedra.nonleap_thursday_regular_israel
				],
				
				[                         // complete
					Sedra.nonleap_thursday_complete,
					Sedra.nonleap_thursday_complete
				]
			],
			
			[                           // saturday
				[                         // incomplete
					Sedra.nonleap_saturday_incomplete,
					Sedra.nonleap_saturday_incomplete
				],
				
				[                         // regular
					null, null
				],
				
				[                         // complete
					Sedra.nonleap_saturday_complete, Sedra.nonleap_saturday_complete  //e.g. 5716
				]
			]
		],
		
		
		[                             // leap years
			[                           // monday
				[                         // incomplete //e.g. 5746
					Sedra.leap_monday_incomplete_diaspora,
					Sedra.leap_monday_incomplete_israel
				],
				
				[                         // regular
					null, null
				],
				
				[                         // complete
					Sedra.leap_monday_complete_diaspora,
					Sedra.leap_monday_complete_israel
				]
			],
			
			[                           // tuesday
				[                         // incomplete
				 null, null
				],
				
				[                         // regular
					Sedra.leap_tuesday_regular_diaspora,
					Sedra.leap_tuesday_regular_israel
				],
				
				[                         // complete
					null, null
				]
			],
			
		[                           // thursday
			[                         // incomplete
				Sedra.leap_thursday_incomplete,
				Sedra.leap_thursday_incomplete
			],
			
			[                         // regular
				null, null
			],
			
			[                         // complete
				Sedra.leap_thursday_complete,
				Sedra.leap_thursday_complete
			]
		],
			
			[                           // saturday
				[                         // incomplete
					Sedra.leap_saturday_incomplete,
					Sedra.leap_saturday_incomplete
				],
				
				[                         // regular
					null, null
				],
				
				[                         // complete
					Sedra.leap_saturday_complete_diaspora,
					Sedra.leap_saturday_complete_israel
				]
			]
		]
	];

	Sedra.prototype.getSedraFromHebcalDate = function(hDate) {
		return this.getSedraFromAbsDate(HDate.hebrew2abs(hDate));
	};

	// returns an array describing the parsha on the first saturday on or after absdate
	//FIX: ignores holidays on the birthday thru friday.
	Sedra.prototype.getSedraFromAbsDate = function(absBirthdayDate) {

		// find the first saturday on or after today's date
		var absDate = c.day_on_or_before (6, absBirthdayDate + 6);
		
		var weekNum = (absDate - this.first_saturday) / 7;
		var index = this.theSedraArray[weekNum];
		
		if (null === index)
			return null;
		if( typeof(index) == 'object'){
			// Shabbat has a chag.  return a description
			return [index];
		}
		if (index >= 0)
			return [Sedra.parshiot[index]];
		
		var i = U (index);      // undouble the parsha
		return [Sedra.parshiot[i], Sedra.parshiot[i + 1]];
	};

	return Sedra;
};
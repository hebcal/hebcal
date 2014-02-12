var months = exports.months = {};
months.NISAN    = 1;
months.IYYAR    = 2;
months.SIVAN    = 3;
months.TAMUZ    = 4;
months.AV       = 5;
months.ELUL     = 6;
months.TISHREI  = 7;
months.CHESHVAN = 8;
months.KISLEV   = 9;
months.TEVET    = 10;
months.SHVAT    = 11;
months.ADAR_I   = 12;
months.ADAR_II  = 13;

var monthNames = exports.monthNames = [
	[
		["VOID",null,"VOID"],
		["Nisan",null,"ניסן"],
		["Iyyar",null,"אייר"],
		["Sivan",null,"סיון"],
		["Tamuz",null,"תמוז"],
		["Av",null,"אב"],
		["Elul",null,"אלול"],
		["Tishrei",null,"תשרי"],
		["Cheshvan",null,"חשון"],
		["Kislev",null,"כסלו"],
		["Tevet",null,"טבת"],
		["Sh'vat",null,"שבט"],
		["Adar",null,"אדר"],
		["Nisan",null,"ניסן"]
	], [
		["VOID",null,"VOID"],
		["Nisan",null,"ניסן"],
		["Iyyar",null,"אייר"],
		["Sivan",null,"סיון"],
		["Tamuz",null,"תמוז"],
		["Av",null,"אב"],
		["Elul",null,"אלול"],
		["Tishrei",null,"תשרי"],
		["Cheshvan",null,"חשון"],
		["Kislev",null,"כסלו"],
		["Tevet",null,"טבת"],
		["Sh'vat",null,"שבט"],
        ["Adar I",null,"אדר א'"],
        ["Adar II",null,"אדר ב'"],
		["Nisan",null,"ניסן"]
	]
];

var days = exports.days = {};
days.SUN = 0;
days.MON = 1;
days.TUE = 2;
days.WED = 3;
days.THU = 4;
days.FRI = 5;
days.SAT = 6;

function LANGUAGE(str, opts){
	return (opts === 'h' && str[2] ? str[2] : (opts === 'a' && str[1] ? str[1] : str[0]));
}
exports.LANGUAGE = LANGUAGE;

function LEAP_YR_HEB(x) {
	return (1 + x * 7) % 19 < 7 ? true : false;
}
exports.LEAP_YR_HEB = LEAP_YR_HEB;

function MONTHS_IN_HEB(x) {
	return 12 + LEAP_YR_HEB(x); // boolean is cast to 1 or 0
}
exports.MONTHS_IN_HEB = MONTHS_IN_HEB;

function max_days_in_heb_month(month, year) {
	if (month == months.IYYAR ||
		month == months.TAMUZ || 
		month == months.ELUL ||
		month == months.TEVET || 
		month == months.ADAR_II ||
		(month == months.ADAR_I && !LEAP_YR_HEB(year)) ||
		(month == months.CHESHVAN && !long_cheshvan(year)) ||
		(month == months.KISLEV && short_kislev(year))) {
		return 29;
	}
	else {
		return 30;
	}
}
exports.max_days_in_heb_month = max_days_in_heb_month;


/* Days from sunday prior to start of hebrew calendar to mean
   conjunction of tishrei in hebrew YEAR 
 */
function hebrew_elapsed_days(hYear){
	// borrowed from original JS
	var yearl = hYear;
	var m_elapsed = 235 * Math.floor((yearl - 1) / 19) 
		+ 12 * ((yearl - 1) % 19) 
		+ Math.floor(((((yearl - 1) % 19) * 7) + 1) / 19);
	
	var p_elapsed = 204 + (793 * (m_elapsed % 1080));
	
	var h_elapsed = 5 + (12 * m_elapsed) +
		793 * Math.floor(m_elapsed / 1080) +
		Math.floor(p_elapsed / 1080);
	
	var parts = (p_elapsed % 1080) + 1080 * (h_elapsed % 24);
	
	var day = 1 + 29 * m_elapsed + Math.floor(h_elapsed / 24);
	var alt_day;

	if ((parts >= 19440) ||
		((2 == (day % 7)) && (parts >= 9924) && !(LEAP_YR_HEB (hYear))) ||
		((1 == (day % 7)) && (parts >= 16789) && LEAP_YR_HEB (hYear - 1)))
		alt_day = day + 1;
	else
		alt_day = day;
	
	if ((alt_day % 7) == 0 ||
		(alt_day % 7) == 3 ||
		(alt_day % 7) == 5)
		return alt_day + 1;
	else
		return alt_day;

}
exports.hebrew_elapsed_days = hebrew_elapsed_days;

/* Number of days in the hebrew YEAR */
function days_in_heb_year(year)
{
	return hebrew_elapsed_days(year + 1) - hebrew_elapsed_days(year);
}
exports.days_in_heb_year = days_in_heb_year;

/* true if Cheshvan is long in hebrew YEAR */
function long_cheshvan(year) {
	return (days_in_heb_year(year) % 10) == 5;
}
exports.long_cheshvan = long_cheshvan;

/* true if Kislev is short in hebrew YEAR */
function short_kislev(year) {
	return (days_in_heb_year(year) % 10) == 3;
}
exports.short_kislev = short_kislev;

function lookup_hebrew_month(c) {
  /* the Hebrew months are unique to their second letter
	 N         nisan  (november?)
	 I         iyyar
	 E        Elul
	 C        Cheshvan
	 K        Kislev
	 1        1Adar
	 2        2Adar   
	 Si Sh     sivan, Shvat
	 Ta Ti Te Tamuz, Tishrei, Tevet
	 Av Ad    Av, Adar

	 אב אד אי אל   אב אדר אייר אלול
	 ח            חשון
	 ט            טבת
	 כ            כסלב
	 נ            ניסן
	 ס            סיון
	 ש            שבט
	 תמ תש        תמוז תשרי
   */
	switch (c.toLowerCase()[0]) {
		case 'n':
		case 'נ':
			return (c.toLowerCase()[1] == 'o') ?	/* this catches "november" */
				0
				: months.NISAN;
		case 'i':
			return months.IYYAR;
		case 'e':
			return months.ELUL;
		case 'c':
		case 'ח':
			return months.CHESHVAN;
		case 'k':
		case 'כ':
			return months.KISLEV;
		case 's':
			switch (c.toLowerCase()[1]) {
				case 'i':
					return months.SIVAN;
				case 'h':
					return months.SHVAT;
				default:
					return 0;
			}
		case 't':
			switch (c.toLowerCase()[1]) {
				case 'a':
					return months.TAMUZ;
				case 'i':
					return months.TISHREI;
				case 'e':
					return months.TEVET;
				default:
					return 0;
			}
		case 'a':
			switch (c.toLowerCase()[1]) {
				case 'v':
					return months.AV;
				case 'd':
					if (c.indexOf('1') > -1) {
						return months.ADAR_I;
					}
					return months.ADAR_II;	/* else assume sheni */
				default:
					return 0;
			}
		case 'ס':
			return months.SIVAN;
		case 'ש':
			return months.SHVAT;
		case 'א':
			switch (c.toLowerCase()[1]) {
				case 'ב':
					return months.AV;
				case 'ד':
					if (c.indexOf('1') > -1 || c.indexOf('א', 1) > 1) {
						return months.ADAR_I;
					}
					return months.ADAR_II;	/* else assume sheni */
				case 'י':
					return months.IYYAR;
				case 'ל':
					return months.ELUL;
				default:
					return 0;
			}
		case 'ת':
			switch (c.toLowerCase()[1]) {
				case 'מ':
					return months.TAMUZ;
				case 'ש':
					return months.TISHREI;
				default:
					return 0;
			}
		default:
			return 0;
	}
}
exports.lookup_hebrew_month = lookup_hebrew_month;

/* Note: Applying this function to d+6 gives us the DAYNAME on or after an
 * absolute day d.  Similarly, applying it to d+3 gives the DAYNAME nearest to
 * absolute date d, applying it to d-1 gives the DAYNAME previous to absolute
 * date d, and applying it to d+7 gives the DAYNAME following absolute date d.

**/
function day_on_or_before(day_of_week, absdate) {
	return absdate - ((absdate - day_of_week) % 7);
}
exports.day_on_or_before = day_on_or_before;

function map(self, fun, thisp, sameprops) {
	// originally written for http://github.com/Scimonster/localbrowse
	if (self === null || typeof fun != 'function') {
		throw new TypeError();
	}
	var t = Object(self);
	var res = {};
	for (var i in t) {
		if (t.hasOwnProperty(i)) {
			var val = fun.call(thisp, t[i], i, t);
			if (sameprops) {
				// the new property should have the same enumerate/write/etc as the original
				var props = Object.getOwnPropertyDescriptor(t, i);
				props.value = val;
				Object.defineProperty(res, i, props);
			} else {
				res[i] = val;
			}
		}
	}
	if (Array.isArray(self) || typeof self == 'string') { // came as an array, return an array
		var arr = [];
		for (i in res) {
			arr[Number(i)] = res[i];
		}
		res = arr.filter(function (v) {
			return v;
		}); // for...in isn't guaranteed to give any meaningful order
		if (typeof self == 'string') {
			res = res.join('');
		}
	}
	return res;
}
exports.map = map;

function gematriya(num, limit) {
	if (typeof num !== 'number' && typeof num !== 'string') {
		throw new TypeError('non-number or string given to gematriya()');
	}
	var str = typeof num === 'string';
	if (str) {
		num = num.replace(/('|")/g,'');
	}
	if (!str && limit && limit - num.toString().length < 0) {
		num = num.toString().split('').reverse().slice(0, limit - num.toString().length).reverse().join('');
	}
	num = num.toString().split('').reverse();
	var letters = {
		0: '',
		1: 'א',
		2: 'ב',
		3: 'ג',
		4: 'ד',
		5: 'ה',
		6: 'ו',
		7: 'ז',
		8: 'ח',
		9: 'ט',
		10: 'י',
		20: 'כ',
		30: 'ל',
		40: 'מ',
		50: 'נ',
		60: 'ס',
		70: 'ע',
		80: 'פ',
		90: 'צ',
		100: 'ק',
		200: 'ר',
		300: 'ש',
		400: 'ת',
		500: 'תק',
		600: 'תר',
		700: 'תש',
		800: 'תת',
		900: 'תתק',
		1000: 'תתר'
	}, numbers = {
		'א': 1,
		'ב': 2,
		'ג': 3,
		'ד': 4,
		'ה': 5,
		'ו': 6,
		'ז': 7,
		'ח': 8,
		'ט': 9,
		'י': 10,
		'כ': 20,
		'ל': 30,
		'מ': 40,
		'נ': 50,
		'ס': 60,
		'ע': 70,
		'פ': 80,
		'צ': 90,
		'ק': 100,
		'ר': 200,
		'ש': 300,
		'ת': 400,
		'תק': 500,
		'תר': 600,
		'תש': 700,
		'תת': 800,
		'תתק': 900,
		'תתר': 1000
	};

	num = num.map(function g(n,i){
		if (str) {
			return numbers[n] < numbers[num[i-1]] && numbers[n] < 100 ? numbers[n] * 1000 : numbers[n];
		} else {
			if (parseInt(n) * Math.pow(10,i) > 1000) {
				return g(n,i-3);
			}
			return letters[parseInt(n) * Math.pow(10,i)];
		}
	});

	if (str) {
		return num.reduce(function(o,t){
			return o + t;
		}, 0);
	} else {
		num = num.reverse().join('').replace(/יה/g,'טו').replace(/יו/g,'טז').split('');

		if (num.length === 1) {
			num.push("'");
		} else if (num.length > 1) {
			num.splice(-1, 0, '"');
		}

		return num.join('');
	}
}
exports.gematriya = gematriya;

function range(start, end, step) {
	step = step || 1;
	if (step < 0) {
		step = 0 - step;
	}

	var arr = [];
	if (start < end) {
		for (var i = start; i <= end; i += step) {
			arr.push(i);
		}
	} else {
		for (var i = start; i >= end; i -= step) {
			arr.push(i);
		}
	}
	return arr;
}
exports.range = range;
/*
	Hebcal - A Jewish Calendar Generator
	Copyright (C) 1994-2004  Danny Sadinoff
	Portions Copyright (c) 2002 Michael J. Radwin. All Rights Reserved.

	https://github.com/hebcal/hebcal

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

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

	The JavaScript code was completely rewritten in 2014 by Scimonster
 */
var c = require('./common'), HDate = require('./hdate');

var masks = exports.masks = {};
masks.USER_EVENT          = 1;
masks.LIGHT_CANDLES       = 2;
masks.YOM_TOV_ENDS        = 4;
masks.CHUL_ONLY           = 8; // chutz l'aretz (Diaspora)
masks.IL_ONLY             = 16; // b'aretz (Israel)
masks.LIGHT_CANDLES_TZEIS = 32;

var IGNORE_YEAR = exports.IGNORE_YEAR = -1;

function Event(date, desc, mask) {
	this.date = new HDate(date);
	if (date instanceof Date) {
		this.date.setFullYear(IGNORE_YEAR);
	}
	this.desc = typeof desc === 'string' ? [desc] : desc;

	this.IGNORE_YEAR = date.getFullYear() === IGNORE_YEAR;
	this.USER_EVENT = !!(mask & masks.USER_EVENT);
	this.LIGHT_CANDLES = !!(mask & masks.LIGHT_CANDLES);
	this.YOM_TOV_ENDS = !!(mask & masks.YOM_TOV_ENDS);
	this.CHUL_ONLY = !!(mask & masks.CHUL_ONLY);
	this.IL_ONLY = !!(mask & masks.IL_ONLY);
	this.LIGHT_CANDLES_TZEIS = !!(mask & masks.LIGHT_CANDLES_TZEIS);
}

Event.prototype.is = function is(date, il) {
	if (!(arguments.length > 1)) {
		il = Event.isIL;
	}
	date = date || new HDate();
	if (date.getDate() !== this.date.getDate() || date.getMonth() !== this.date.getMonth()) {
		return false;
	}
	if (!this.IGNORE_YEAR && date.getFullYear() !== this.date.getFullYear()) {
		return false;
	}
	if (il && this.CHUL_ONLY || !il && this.IL_ONLY) {
		return false;
	}
	return true;
};

Event.prototype.masks = function mask() {
	return +(this.USER_EVENT          && masks.USER_EVENT) |
		   +(this.LIGHT_CANDLES       && masks.LIGHT_CANDLES) |
		   +(this.YOM_TOV_ENDS        && masks.YOM_TOV_ENDS) |
		   +(this.CHUL_ONLY           && masks.CHUL_ONLY) |
		   +(this.IL_ONLY             && masks.IL_ONLY) |
		   +(this.LIGHT_CANDLES_TZEIS && masks.LIGHT_CANDLES_TZEIS)
};

Event.prototype.getDesc = function getDesc(o) {
	return c.LANGUAGE(this.desc, o);
};

Event.prototype.candleLighting = function candleLighting() {
	if (this.LIGHT_CANDLES) {
		return new Date(this.date.sunset() - (Event.candleLighting * 60 * 1000));
	} else if (this.LIGHT_CANDLES_TZEIS) {
		return this.date.getZemanim().tzeit;
	}
	return null;
};

Event.prototype.havdalah = function havdalah() {
	if (this.YOM_TOV_ENDS) {
		return new Date(this.date.sunset() + (Event.havdalah * 60 * 1000));
	}
	return null;
};

Event.isIL = false;

Event.candleLighting = 18;

Event.havdalah = 50;

exports.Event = Event;

exports.standards = [ // standard holidays that don't shift based on year
	// RH 1 is defined later, based on year, because other holidays depend on it
	new Event(
		new HDate(1, c.months.TISHREI, IGNORE_YEAR),
		['Rosh Hashana 2', null, 'ראש השנה ב\''],
		masks.YOM_TOV_ENDS
	), new Event(
		new HDate(9, c.months.TISHREI, IGNORE_YEAR),
		['Erev Yom Kippur', null, 'ערב יום כיפור'],
		masks.LIGHT_CANDLES
	), new Event(
		new HDate(10, c.months.TISHREI, IGNORE_YEAR),
		['Yom Kippur', null, 'יום כיפור'],
		masks.YOM_TOV_ENDS
	), new Event(
		new HDate(14, c.months.TISHREI, IGNORE_YEAR),
		['Erev Sukkot', 'Erev Sukkos', 'ערב סוכות'],
		masks.LIGHT_CANDLES
	), new Event(
		new HDate(15, c.months.TISHREI, IGNORE_YEAR),
		['Sukkot 1', 'Sukkos 1', 'סוכות יום א\''],
		masks.LIGHT_CANDLES_TZEIS | masks.CHUL_ONLY
	), new Event(
		new HDate(15, c.months.TISHREI, IGNORE_YEAR),
		['Sukkot 1', 'Sukkos 1', 'סוכות יום א\''],
		masks.YOM_TOV_ENDS | masks.IL_ONLY
	), new Event(
		new HDate(16, c.months.TISHREI, IGNORE_YEAR),
		['Sukkot 2', 'Sukkos 2', 'סוכות יום ב\''],
		masks.YOM_TOV_ENDS | masks.CHUL_ONLY
	), new Event(
		new HDate(16, c.months.TISHREI, IGNORE_YEAR),
		['Sukkot 2 (CH"M)', 'Sukkos 2 (CH"M)', 'סוכות יום ב\' )חה"ם('],
		masks.IL_ONLY
	), new Event(
		new HDate(17, c.months.TISHREI, IGNORE_YEAR),
		['Sukkot 3 (CH"M)', 'Sukkos 3 (CH"M)', 'סוכות יום ג\' )חה"ם('],
		0
	), new Event(
		new HDate(18, c.months.TISHREI, IGNORE_YEAR),
		['Sukkot 4 (CH"M)', 'Sukkos 4 (CH"M)', 'סוכות יום ד\' )חה"ם('],
		0
	), new Event(
		new HDate(19, c.months.TISHREI, IGNORE_YEAR),
		['Sukkot 5 (CH"M)', 'Sukkos 5 (CH"M)', 'סוכות יום ה\' )חה"ם('],
		0
	), new Event(
		new HDate(20, c.months.TISHREI, IGNORE_YEAR),
		['Sukkot 6 (CH"M)', 'Sukkos 6 (CH"M)', 'סוכות יום ו\' )חה"ם('],
		0
	), new Event(
		new HDate(21, c.months.TISHREI, IGNORE_YEAR),
		['Sukkot 7 (Hoshana Raba)', 'Sukkos 7 (Hoshana Raba)', 'סוכות יום ז\' )הושנע רבה('],
		masks.LIGHT_CANDLES
	), new Event(
		new HDate(22, c.months.TISHREI, IGNORE_YEAR),
		['Shmini Atzeret', 'Shmini Atzeres', 'שמיני עצרת'],
		masks.LIGHT_CANDLES_TZEIS | masks.CHUL_ONLY
	), new Event(
		new HDate(22, c.months.TISHREI, IGNORE_YEAR),
		['Shmini Atzeret / Simchat Torah', 'Shmini Atzeres / Simchas Torah', 'שמיני עצרת / שמחת תורה'],
		masks.YOM_TOV_ENDS | masks.IL_ONLY
	), new Event(
		new HDate(23, c.months.TISHREI, IGNORE_YEAR),
		['Simchat Torah', 'Simchas Torah', 'שמחת תורה'],
		masks.YOM_TOV_ENDS | masks.CHUL_ONLY
	), new Event(
		new HDate(24, c.months.KISLEV, IGNORE_YEAR),
		['Erev Chanukah', null, 'ערב חנוכה'],
		0
	), new Event(
		new HDate(25, c.months.KISLEV, IGNORE_YEAR),
		['Chanukah: 1st Candle', null, 'חנוכה: נר א\''],
		0
	), new Event(
		new HDate(26, c.months.KISLEV, IGNORE_YEAR),
		['Chanukah: 2nd Candle', null, 'חנוכה: נר ב\''],
		0
	), new Event(
		new HDate(27, c.months.KISLEV, IGNORE_YEAR),
		['Chanukah: 3rd Candle', null, 'חנוכה: נר ג\''],
		0
	), new Event(
		new HDate(28, c.months.KISLEV, IGNORE_YEAR),
		['Chanukah: 4th Candle', null, 'חנוכה: נר ד\''],
		0
	), new Event(
		new HDate(29, c.months.KISLEV, IGNORE_YEAR),
		['Chanukah: 5th Candle', null, 'חנוכה: נר ה\''],
		0
	), new Event(
		new HDate(30, c.months.KISLEV, IGNORE_YEAR), // yes, i know these are wrong
		['Chanukah: 6th Candle', null, 'חנוכה: נר ו\''],
		0
	), new Event(
		new HDate(31, c.months.KISLEV, IGNORE_YEAR), // HDate() corrects the month automatically
		['Chanukah: 7th Candle', null, 'חנוכה: נר ז\''],
		0
	), new Event(
		new HDate(32, c.months.KISLEV, IGNORE_YEAR),
		['Chanukah: 8th Candle', null, 'חנוכה: נר ח\''],
		0
	), new Event(
		new HDate(15, c.months.SHVAT, IGNORE_YEAR),
		['Tu B\'Shvat', null, 'ט"ו בשבט'],
		0
	), new Event(
		new HDate(14, c.months.NISAN, IGNORE_YEAR),
		['Erev Pesach', null, 'ערב פסח'],
		masks.LIGHT_CANDLES
	), new Event(
		new HDate(15, c.months.NISAN, IGNORE_YEAR),
		['Pesach 1', null, 'פסח יום א\''],
		masks.LIGHT_CANDLES_TZEIS | masks.CHUL_ONLY
	), new Event(
		new HDate(15, c.months.NISAN, IGNORE_YEAR),
		['Pesach 1', null, 'פסח יום א\''],
		masks.YOM_TOV_ENDS | masks.IL_ONLY
	), new Event(
		new HDate(16, c.months.NISAN, IGNORE_YEAR),
		['Pesach 2', null, 'פסח יום ב\''],
		masks.YOM_TOV_ENDS | masks.CHUL_ONLY
	), new Event(
		new HDate(16, c.months.NISAN, IGNORE_YEAR),
		['Pesach 2 (CH"M)', null, 'פסח יום ב\' )חה"ם('],
		masks.IL_ONLY
	), new Event(
		new HDate(16, c.months.NISAN, IGNORE_YEAR),
		['Start counting Omer', null, 'התחלת ספירת העומר'],
		0
	), new Event(
		new HDate(17, c.months.NISAN, IGNORE_YEAR),
		['Pesach 3 (CH"M)', null, 'פסח יום ג\' )חה"ם('],
		0
	), new Event(
		new HDate(18, c.months.NISAN, IGNORE_YEAR),
		['Pesach 4 (CH"M)', null, 'פסח יום ד\' )חה"ם('],
		0
	), new Event(
		new HDate(19, c.months.NISAN, IGNORE_YEAR),
		['Pesach 5 (CH"M)', null, 'פסח יום ה\' )חה"ם('],
		0
	), new Event(
		new HDate(20, c.months.NISAN, IGNORE_YEAR),
		['Pesach 6 (CH"M)', null, 'פסח יום ו\' )חה"ם('],
		masks.LIGHT_CANDLES
	), new Event(
		new HDate(21, c.months.NISAN, IGNORE_YEAR),
		['Pesach 7', null, 'פסח יום ז\''],
		masks.LIGHT_CANDLES_TZEIS | masks.CHUL_ONLY
	), new Event(
		new HDate(21, c.months.NISAN, IGNORE_YEAR),
		['Pesach 7', null, 'פסח יום ז\''],
		masks.YOM_TOV_ENDS | masks.IL_ONLY
	), new Event(
		new HDate(22, c.months.NISAN, IGNORE_YEAR),
		['Pesach 8', null, 'פסח יום ח\''],
		masks.YOM_TOV_ENDS | masks.CHUL_ONLY
	), new Event(
		new HDate(14, c.months.IYYAR, IGNORE_YEAR),
		['Pesach Sheni', null, 'פסח שני'],
		0
	), new Event(
		new HDate(18, c.months.IYYAR, IGNORE_YEAR),
		['Lag B\'Omer', null, 'ל"ג בעומר'],
		0
	), new Event(
		new HDate(5, c.months.SIVAN, IGNORE_YEAR),
		['Erev Shavuot', 'Erev Shavuos', 'ערב שבועות'],
		masks.LIGHT_CANDLES
	), new Event(
		new HDate(6, c.months.SIVAN, IGNORE_YEAR),
		['Shavuot 1', 'Shavuos 1', 'שבועות א\''],
		masks.LIGHT_CANDLES_TZEIS | masks.CHUL_ONLY
	), new Event(
		new HDate(6, c.months.SIVAN, IGNORE_YEAR),
		['Shavuot', 'Shavuos', 'שבועות'],
		masks.YOM_TOV_ENDS | masks.IL_ONLY
	), new Event(
		new HDate(7, c.months.SIVAN, IGNORE_YEAR),
		['Shavuot 2', 'Shavuos 2', 'שבועות ב\''],
		masks.YOM_TOV_ENDS | masks.CHUL_ONLY
	), new Event(
		new HDate(29, c.months.ELUL, IGNORE_YEAR),
		['Erev Rosh Hashana', null, 'ערב ראש השנה'],
		masks.LIGHT_CANDLES
	)
];

exports.getHolidaysForYear = function getHolidaysForYear(year) {
	var h = exports.standards.slice(), // clone

		RH = new HDate(1, c.months.TISHREI, year),
		pesach = new HDate(15, c.months.NISAN, year),
		tmpDate;

	h.push(new Event(
		RH,
		['Rosh Hashana 1', null, 'ראש השנה א\''],
		masks.LIGHT_CANDLES_TZEIS
	));

	h.push(new Event(
		new HDate(3 + (RH.getDay() == c.days.THU), c.months.TISHREI, year), // push off to SUN if RH is THU
		['Tzom Gedaliah', null, 'צום גדליה'],
		0
	));

	h.push(new Event( // first SAT after RH
		new HDate(c.day_on_or_before(c.days.SAT, 7 + RH.abs())),
		['Shabbat Shuva', 'Shabbos Shuvah', 'שבת שובה'],
		0
	));

	tmpDate = new HDate(10, c.months.TEVET, year);
	if (tmpDate.getDay() === c.days.SAT) {
		tmpDate = tmpDate.next();
	}
	h.push(new Event(
		tmpDate,
		['Asara B\'Tevet', null, 'עשרה בטבת'],
		0
	));

	h.push(new Event(
		new HDate(c.day_on_or_before(c.days.SAT, pesach.abs() - 43)),
		['Shabbat Shekalim', 'Shabbos Shekalim', 'שבת שקלים'],
		0
	));

	h.push(new Event(
		new HDate(c.day_on_or_before(c.days.SAT, pesach.abs() - 30)),
		['Shabbat Zachor', 'Shabbos Zachor', 'שבת זכור'],
		0
	));

	h.push(new Event(
		new HDate(pesach.abs() - (pesach.getDay() == c.days.TUE ? 33 : 31)),
		['Ta\'anit Esther', 'Ta\'anis Esther', 'תענית אסתר'],
		0
	));

	if (c.LEAP_YR_HEB(year)) {
		h.push(new Event(
			new HDate(14, c.months.ADAR_I, year),
			['Purim Katan', null, 'פורים קטן'],
			0
		));

		h.push(new Event(
			new HDate(15, c.months.ADAR_I, year),
			['Shushan Purim Katan', null, 'שושן פורים קטן'],
			0
		));

		h.push(new Event(
			new HDate(13, c.months.ADAR_II, year),
			['Erev Purim', null, 'ערב פורים'],
			0
		));

		h.push(new Event(
			new HDate(14, c.months.ADAR_II, year),
			['Purim', null, 'פורים'],
			0
		));

		h.push(new Event(
			new HDate(15, c.months.ADAR_II, year),
			['Shushan Purim', null, 'שושן פורים'],
			0
		));
	} else {
		h.push(new Event(
			new HDate(13, c.months.ADAR_I, year),
			['Erev Purim', null, 'ערב פורים'],
			0
		));

		h.push(new Event(
			new HDate(14, c.months.ADAR_I, year),
			['Purim', null, 'פורים'],
			0
		));

		h.push(new Event(
			new HDate(15, c.months.ADAR_I, year),
			['Shushan Purim', null, 'שושן פורים'],
			0
		));
	}

	h.push(new Event(
		new HDate(c.day_on_or_before(c.days.SAT, pesach.abs() - 14) - 7),
		['Shabbat Parah', 'Shabbos Parah', 'שבת פרה'],
		0
	));

	h.push(new Event(
		new HDate(c.day_on_or_before(c.days.SAT, pesach.abs() - 14)),
		['Shabbat Hachodesh', 'Shabbos Hachodesh', 'שבת החודש'],
		0
	));

	if (pesach.prev().getDay() == c.days.SAT) {
		// if the fast falls on Shabbat, move to Thursday
		h.push(new Event(
			new HDate(c.day_on_or_before(c.days.THU, pesach.abs())),
			['Ta\'anit Bechorot', 'Ta\'anis Bechoros', 'תענית בכורות'],
			0
		));
	} else {
		h.push(new Event(
			new HDate(14, c.months.NISAN, year),
			['Ta\'anit Bechorot', 'Ta\'anis Bechoros', 'תענית בכורות'],
			0
		));
	}

	h.push(new Event(
		new HDate(c.day_on_or_before(c.days.SAT, pesach.abs() - 1)),
		['Shabbat HaGadol', 'Shabbos HaGadol', 'שבת הגדול'],
		0
	));

	if (year >= 5711) { // Yom HaShoah first observed in 1951
		tmpDate = new HDate(27, c.months.NISAN, year);
		/* When the actual date of Yom Hashoah falls on a Friday, the
		 * state of Israel observes Yom Hashoah on the preceding
		 * Thursday. When it falls on a Sunday, Yom Hashoah is observed
		 * on the following Monday.
		 * http://www.ushmm.org/remembrance/dor/calendar/
		 */

		if (tmpDate.getDay() === c.days.FRI) {
			tmpDate = tmpDate.prev();
		} else if (tmpDate.getDay() === c.days.SUN) {
			tmpDate = tmpDate.next();
		}

		h.push(new Event(
			tmpDate,
			['Yom HaShoah', null, 'יום השואה'],
			0
		));
	}

	if (year >= 5711) { // Yom HaAtzma'ut only celebrated after 1948
		tmpDate = new HDate(1, c.months.IYYAR, year);

		if (pesach.getDay() === c.days.SUN) {
			tmpDate.setDate(2);
		} else if (pesach.getDay() === c.days.SAT) {
			tmpDate.setDate(3);
		} else if (year < 5764) {
			tmpDate.setDate(4);
		} else if (pesach.getDay() === c.days.TUE) {
			tmpDate.setDate(5);
		} else {
			tmpDate.setDate(4);
		}

		h.push(new Event(
			tmpDate,
			['Yom HaZikaron', null, 'יום הזיכרון'],
			0
		));

		h.push(new Event(
			tmpDate.next(),
			['Yom HaAtzma\'ut', null, 'יום העצמאות'],
			0
		));
	}

	if (year >= 5727) { // Yom Yerushalayim only celebrated after 1967
		h.push(new Event(
			new HDate(29, c.months.IYYAR, year),
			['Yom Yerushalayim', null, 'יום ירושלים'],
			0
		));
	}

	tmpDate = new HDate(17, c.months.TAMUZ, year);
	if (tmpDate.getDay() === c.days.SAT) {
		tmpDate = tmpDate.next();
	}
	h.push(new Event(
		tmpDate,
		['Shiva-Asar B\'Tamuz', null, 'צום יז\' בתמוז'],
		0
	));

	tmpDate = new HDate(9, c.months.AV, year);
	if (tmpDate.getDay() === c.days.SAT) {
		tmpDate = tmpDate.next();
	}

	h.push(new Event(
		new HDate(c.day_on_or_before(c.days.SAT, tmpDate.abs())),
		['Shabbat Chazon', 'Shabbos Chazon', 'שבת חזון'],
		0
	));

	h.push(new Event(
		tmpDate.prev(),
		['Erev Tish\'a B\'Av', null, 'ערב תשעה באב'],
		0
	));

	h.push(new Event(
		tmpDate,
		['Tish\'a B\'Av', null, 'תשעה באב'],
		0
	));

	h.push(new Event(
		new HDate(c.day_on_or_before(c.days.SAT, tmpDate.abs() + 7)),
		['Shabbat Nachamu', 'Shabbos Nachamu', 'שבת נחמו'],
		0
	));

	h.push(new Event(
		new HDate(c.day_on_or_before(c.days.SAT, new HDate(1, c.months.TISHREI, year + 1).abs() - 4)),
		['Leil Selichot', 'Leil Selichos', 'ליל סליחות'],
		0
	));

	var day = 6;
	while (day < c.days_in_heb_year(year)) {
		h.push(new Event(
			new HDate(c.day_on_or_before(c.days.SAT, new HDate(1, c.months.TISHREI, year).abs() + day)),
			['Shabbat', 'Shabbos', 'שבת'],
			masks.YOM_TOV_ENDS
		));

		h.push(new Event(
			new HDate(c.day_on_or_before(c.days.FRI, new HDate(1, c.months.TISHREI, year).abs() + day)),
			['Erev Shabbat', 'Erev Shabbos', 'ערב שבת'],
			masks.LIGHT_CANDLES
		));

		day += 7;
	}

	return h;
};
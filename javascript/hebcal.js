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
var c = require('./common'),
	HDate = require('./hdate'),
	holidays = require('./holidays'),
	dafyomi = require('./dafyomi'),
	cities = require('./cities');

function Hebcal(year, month) {
	if (!year) {
		year = (new HDate()).getFullYear(); // this year;
	}
	if (typeof year !== 'number') {
		throw new TypeError('year to Hebcal() is not a number');
	}
	this.year = year;
	if (month) {
		if (typeof month == 'string') {
			month = c.lookup_hebrew_month(month);
		}
		if (typeof month == 'number') {
			return new Hebcal(year, [month]);
		}

		if (Array.isArray(month)) {
			this.months = month.map(function(i){
				var m = new Hebcal.Month(i, year);
				Object.defineProperty(m, '__year', {
					configurable: true,
					writable: true,
					value: this
				});
				return m;
			}, this);

			this.holidays = holidays.getHolidaysForYear(year).filter(function(h){
				return this.months.filter(function(m){ // don't keep ones that are out of bounds
					return m.month===h.date.getMonth();
				}).length;
			}, this);
		} else {
			throw new TypeError('month to Hebcal is not a valid type');
		}
	} else {
		return new Hebcal(year, c.range(1, c.MONTHS_IN_HEB(year)));
	}

	this.length = c.days_in_heb_year(year);

	Object.defineProperty(this, 'il', {
		enumerable: true,
		configurable: true,

		get: function() {
			return this.getMonth(1).il;
		},
		set: function(il) {
			this.months.forEach(function(m){
				m.il = il;
			});
		}
	});

	Object.defineProperty(this, 'lat', {
		enumerable: true,
		configurable: true,

		get: function() {
			return this.getMonth(1).lat;
		},
		set: function(lat) {
			this.months.forEach(function(m){
				m.lat = lat;
			});
		}
	});
	Object.defineProperty(this, 'long', {
		enumerable: true,
		configurable: true,

		get: function() {
			return this.getMonth(1).long;
		},
		set: function(lon) {
			this.months.forEach(function(m){
				m.long = lon;
			});
		}
	});
}

Hebcal.prototype.isLeapYear = function isLeapYear() {
	return c.LEAP_YR_HEB(this.year);
};

Hebcal.prototype.setCity = function setCity(city) {
	this.months.forEach(function(m){
		m.setCity(city);
	});
	return this;
};

Hebcal.prototype.setLocation = function setLocation(lat, lon) {
	this.months.forEach(function(m){
		m.setLocation(lat, lon);
	});
	return this;
};

Hebcal.prototype.next = function next() {
	return new Hebcal(this.year + 1);
};

Hebcal.prototype.prev = function prev() {
	return new Hebcal(this.year - 1);
};

Hebcal.prototype.getMonth = function getMonth(month) {
	month = typeof month === 'number' ? month :
		month.charCodeAt(0) >= 1488 && month.charCodeAt(0) <= 1514 && /('|")/.test(month) ? c.gematriya(month) :
			month.charCodeAt(0) >= 48 && month.charCodeAt(0) <= 57 /* number */ ? parseInt(month, 10) : c.lookup_hebrew_month(month);
	if (month > this.months.length) {
		return this.next().getMonth(month - this.months.length);
	}
	return this.months[month > 0 ? month - 1 : this.months.length + month];
};

Hebcal.prototype.map = function map() {
	return [].map.apply(this.months, arguments);
};

Hebcal.prototype.addHoliday = function addHoliday(holiday) {
	if (!(holiday instanceof holidays.Event)) {
		throw new TypeError('non-Event passed to addHoliday()');
	}
	this.holidays.push(holiday);
	return this;
};

Hebcal.prototype.find = function find(day, month) {
	if (arguments.length === 1) {
		if (typeof day === 'string') {
			return find.strings.call(this, day);
		} else if (Array.isArray(day)) {
			return [].concat.apply([], day.map(function(d){
				return this.find[Array.isArray(d) ? 'apply' : 'call'](this, d);
			}, this));
		} else if (day instanceof HDate) {
			return this.find(day.getDate(), day.getMonth());
		} else if (day instanceof Date) {
			return this.find(new HDate(day));
		}
	} else if (arguments.length === 2) {
		if (month instanceof Hebcal.Month) {
			return month.find(day);
		} else if (Array.isArray(month)) {
			return [].concat.apply([], month.map(function(m){
				return this.find(day, m);
			}, this));
		} else if (typeof month === 'string') {
			return this.find(day,
				month.charCodeAt(0) >= 1488 && month.charCodeAt(0) <= 1514 && /('|")/.test(month) ? c.gematriya(month) :
					month.charCodeAt(0) >= 48 && month.charCodeAt(0) <= 57 /* number */ ? parseInt(month, 10) : c.lookup_hebrew_month(month)
			);
		} else if (typeof month === 'number') {
			return this.find(day, this.getMonth(month));
		}
	}
	return [];
};
Hebcal.prototype.find.strings = function strings(str) {
	if (!str.split(/\s+/).join('')) {
		return [];
	} else if (strings[str.replace(/\s/g, '_').toLowerCase()]) {
		return strings[str.replace(/\s/g, '_').toLowerCase()].call(this);
	} else if (new HDate(str).getDate()) {
		return this.find(new HDate(str));
	}
	return this.find.apply(this, str.split(/\s+/));
};
Hebcal.prototype.find.strings.rosh_chodesh = function rosh_chodesh() {
	return this.find('Rosh Chodesh', c.range(1, this.months.length));
};
Hebcal.prototype.find.strings.holidays = function holidays() {
	return [].concat.apply([], this.holidays.map(function(h){
		return this.find(h.date);
	}, this));
};
Hebcal.prototype.find.strings.omer = function omer() {
	return this.find(c.range(15+1, 15+49), c.months.NISAN);
};
Hebcal.prototype.find.strings.today = function today() {
	return this.find(new HDate());
};
Hebcal.prototype.find.strings.yesterday = function yesterday() {
	return [this.find('today')[0].prev()];
};
Hebcal.prototype.find.strings.tomorrow = function tomorrow() {
	return [this.find('today')[0].next()];
};
Hebcal.prototype.find.strings.pesach = function pesach() {
	return this.find(c.range(15, 15+8-this.il), c.months.NISAN);
};
Hebcal.prototype.find.strings.sukkot = function sukkot() {
	return this.find(c.range(15, 15+9-this.il), c.months.TISHREI);
};
Hebcal.prototype.find.strings.succot = Hebcal.prototype.find.strings.succos = Hebcal.prototype.find.strings.sukkos = Hebcal.prototype.find.strings.sukkot;
Hebcal.prototype.find.strings.shavuot = function shavuot() {
	return this.find(c.range(6, 7-this.il), c.months.SIVAN);
};
Hebcal.prototype.find.strings.shavuos = Hebcal.prototype.find.strings.shavuot;
Hebcal.prototype.find.strings.rosh_hashana = function rosh_hashana() {
	return this.find(c.range(1, 2), c.months.TISHREI);
};
Hebcal.prototype.find.strings.rosh_hashanah = Hebcal.prototype.find.strings.rosh_hashana;

Hebcal.addZeman = HDate.addZeman;

Hebcal.cities = cities;

Hebcal.range = c.range;

Hebcal.gematriya = c.gematriya;

Hebcal.holidays = holidays;

Object.defineProperty(Hebcal, 'defaultLocation', {
	enumerable: true,
	configurable: true,

	get: function() {
		return HDate.defaultLocation;
	},
	set: function(loc) {
		HDate.defaultLocation = loc;
	}
});
Object.defineProperty(Hebcal, 'defaultCity', Object.getOwnPropertyDescriptor(HDate, 'defaultCity'));

Object.defineProperty(Hebcal, 'candleLighting', {
	enumerable: true,
	configurable: true,

	get: function() {
		return holidays.Event.candleLighting;
	},
	set: function(mins) {
		holidays.Event.candleLighting = mins;
	}
});

Object.defineProperty(Hebcal, 'havdalah', {
	enumerable: true,
	configurable: true,

	get: function() {
		return holidays.Event.havdalah;
	},
	set: function(mins) {
		holidays.Event.havdalah = mins;
	}
});

Hebcal.Month = function Month(month, year) {
	if (typeof month == 'string') {
		month = c.lookup_hebrew_month(month);
	}
	if (typeof month != 'number') {
		throw new TypeError('month to Hebcal.Month is not a valid type');
	}
	if (typeof year != 'number') {
		throw new TypeError('year to Hebcal.Month is not a number');
	}
	this.month = month;
	this.year = year;

	this.days = c.range(1, c.max_days_in_heb_month(month, year)).map(function(i){
		var d = new HDate(i, month, year);
		Object.defineProperty(d, '__month', {
			configurable: true,
			writable: true,
			value: this
		});
		return d;
	}, this);

	this.length = this.days.length;

	Object.defineProperty(this, 'il', {
		enumerable: true,
		configurable: true,

		get: function() {
			return this.getDay(1).il;
		},
		set: function(il) {
			this.days.forEach(function(d){
				d.il = il;
			});
		}
	});

	Object.defineProperty(this, 'lat', {
		enumerable: true,
		configurable: true,

		get: function() {
			return this.getDay(1).lat;
		},
		set: function(lat) {
			this.days.forEach(function(d){
				d.lat = lat;
			});
		}
	});
	Object.defineProperty(this, 'long', {
		enumerable: true,
		configurable: true,

		get: function() {
			return this.getDay(1).long;
		},
		set: function(lon) {
			this.days.forEach(function(d){
				d.long = lon;
			});
		}
	});
};

Hebcal.Month.prototype.isLeapYear = function isLeapYear() {
	return c.LEAP_YR_HEB(this.year);
};

Hebcal.Month.prototype.prev = function prev() {
	if (this.month === 1) { // Nisan
		return this.getYearObject().getMonth(-1);
	} else if (this.month === c.months.TISHREI) {
		return this.getYearObject().prev().getMonth(c.months.ELUL);
	} else {
		return this.getYearObject().getMonth(this.month - 1);
	}
};

Hebcal.Month.prototype.next = function next() {
	if (this.month === c.MONTHS_IN_HEB(this.year)) { // Adar
		return this.getYearObject().getMonth(1);
	} else if (this.month === c.months.ELUL) {
		return this.getYearObject().next().getMonth(c.months.TISHREI);
	} else {
		return this.getYearObject().getMonth(this.month + 1);
	}
};

Hebcal.Month.prototype.getDay = function getDay(day) {
	day = typeof day === 'number' ? day :
		day.charCodeAt(0) >= 1488 && day.charCodeAt(0) <= 1514 && /('|")/.test(day) ? c.gematriya(day) :
			day.charCodeAt(0) >= 48 && day.charCodeAt(0) <= 57 /* number */ ? parseInt(day, 10) : c.lookup_hebrew_day(day);
	if (day > this.days.length) {
		return this.next().getDay(day - this.days.length);
	}
	return this.days[day > 0 ? day - 1 : this.days.length + day];
};

Hebcal.Month.prototype.getYearObject = function getYearObject() {
	return this.__year;
};

Hebcal.Month.prototype.getName = function getName(o) {
	return c.LANGUAGE(c.monthNames[+this.isLeapYear()][this.month],o);
};

Hebcal.Month.prototype.rosh_chodesh = function rosh_chodesh() {
	var prev = this.prev();
	return prev.length === 30 ? [prev.getDay(-1), this.getDay(1)] : [this.getDay(1)];
};

Hebcal.Month.prototype.setCity = function setCity(city) {
	this.days.forEach(function(d){
		d.setCity(city);
	});
	return this;
};

Hebcal.Month.prototype.setLocation = function setLocation(lat, lon) {
	this.days.forEach(function(d){
		d.setLocation(lat, lon);
	});
	return this;
};

Hebcal.Month.prototype.map = function map() {
	return [].map.apply(this.days, arguments);
};

Hebcal.Month.prototype.find = function find(day) {
	if (typeof day === 'number') {
		return [this.getDay(day)];
	} else if (typeof day === 'string') {
		return find.strings.call(this, day);
	} else if (Array.isArray(day)) {
		return [].concat.apply([], day.map(function(d){
			return this.find(d);
		}, this));
	} else if (day instanceof HDate && day.getFullYear() === this.year && day.getMonth() === this.month) {
		return this.find(day.getDate());
	} else if (day instanceof Date) {
		return this.find(new HDate(day));
	}
	return [];
};
Hebcal.Month.prototype.find.strings = function strings(str) {
	if (strings[str.replace(/\s/g, '_').toLowerCase()]) {
		return strings[str.replace(/\s/g, '_').toLowerCase()].call(this);
	} else if (new HDate(str).getDate()) {
		return this.find(new HDate(str));
	}
	var num = str.charCodeAt(0) >= 1488 && str.charCodeAt(0) <= 1514 && /('|")/.test(str) ? c.gematriya(str) :
		str.charCodeAt(0) >= 48 && str.charCodeAt(0) <= 57 /* number */ ? parseInt(str, 10) : null;
	return num ? this.find(num) : [];
};
Hebcal.Month.prototype.find.strings.rosh_chodesh = function rosh_chodesh() {
	return this.rosh_chodesh();
};
Hebcal.Month.prototype.find.strings.shabbat_mevarchim = function shabbat_mevarchim() {
	return this.find(new HDate(c.day_on_or_before(c.days.SAT, this.getDay(29).abs())));
};
Hebcal.Month.prototype.find.strings.shabbos_mevarchim = Hebcal.Month.prototype.find.strings.shabbat_mevarchim;

Hebcal.HDate = HDate;

HDate.prototype.getMonthObject = function getMonthObject() {
	return this.__month;
};

HDate.prototype.getYearObject = function getYearObject() {
	return this.getMonthObject().getYearObject();
};

var HDatePrev = HDate.prototype.prev;
HDate.prototype.prev = function prev() {
	if (!this.getMonthObject()) {
		return HDatePrev.call(this);
	}
	if (this.getMonth() === c.months.TISHREI && this.getDate() === 1) { // 1st day RH
		return this.getMonthObject().prev().getDay(-1);
	} else {
		var g = this.greg();
		g.setDate(g.getDate() - 1);
		return this.getYearObject().find(g)[0];
	}
};

var HDateNext = HDate.prototype.next;
HDate.prototype.next = function next() {
	if (!this.getMonthObject()) {
		return HDateNext.call(this);
	}
	if (this.getMonth() === c.months.ELUL && this.getDate() === this.getMonthObject().length) { // last day
		return this.getMonthObject().next().getDay(1);
	} else {
		var g = this.greg();
		g.setDate(g.getDate() + 1);
		return this.getYearObject().find(g)[0];
	}
};

HDate.prototype.holidays = function holidays() {
	return this.getYearObject().holidays.filter(function(h){
		return this.isSameDate(h.date);
	}, this).map(function(h){
		h.date.setLocation(this);
		return h;
	}, this);
};

HDate.prototype.omer = function omer() {
	if (this.greg().getTime() > new HDate(15, c.months.NISAN, this.getFullYear()).greg().getTime() &&
		this.greg().getTime() < new HDate( 6, c.months.SIVAN, this.getFullYear()).greg().getTime()) {
		return HDate.hebrew2abs(this) - HDate.hebrew2abs(new HDate(16, c.months.NISAN, this.getFullYear())) + 1;
	}
	return 0;
};

HDate.prototype.dafyomi = function daf(o) {
	return dafyomi.dafname(dafyomi.dafyomi(this.greg()), o);
};

module.exports = Hebcal;
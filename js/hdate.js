var c = require('./common'),
	greg = require('./greg'),
	Sedra = require('./sedra')(HDate),
	suncalc = require('suncalc'),
	cities = require('./cities')(HDate);

function HDate(day, month, year) {
	if (day instanceof Date) {
		// we were passed a Gregorian date, so convert it
		return abs2hebrew(greg.greg2abs(day));
	} else if (day instanceof HDate) {
		return day;
	} else if (arguments.length === 3) {
		this.day = day;
		this.month = typeof month == 'number' ? month : c.lookup_hebrew_month(month);
		this.year = year;
		fixMonth(this);
		fixDate(this);
	} else if (arguments.length === 2) {
		return new HDate(day, month, (new HDate()).getFullYear());
	} else if (arguments.length === 0) {
		return new HDate(new Date());
	} else {
		throw new TypeError('HDate called with bad number of arguments');
	}

	this.il = false;
	this.setLocation(0, 0);
}

function fixDate(date) {
	if (date.day > c.max_days_in_heb_month(date.month,date.year)) {
		date.month += Math.floor(date.day / c.max_days_in_heb_month(date.month,date.year));
		date.day = date.day % c.max_days_in_heb_month(date.month,date.year);
	}
	fixMonth(date);
}

function fixMonth(date) {
	if (date.month > c.MONTHS_IN_HEB(date.year)) {
		date.year += Math.floor(date.month / c.MONTHS_IN_HEB(date.year));
		date.month = date.month % c.MONTHS_IN_HEB(date.year);
	}
}

HDate.prototype.getFullYear = function getFullYear() {
	return this.year;
};

HDate.prototype.isLeapYear = function isLeapYear() {
	return c.LEAP_YR_HEB(this.getFullYear());
};

HDate.prototype.getMonth = function getMonth() {
	return this.month;
};

HDate.prototype.getTishreiMonth = function getTishreiMonth() {
	return (this.getMonth() + c.MONTHS_IN_HEB(this.getFullYear()) - 6) % c.MONTHS_IN_HEB(this.getFullYear());
};

HDate.prototype.daysInMonth = function daysInMonth() {
	return c.max_days_in_heb_month(this.getMonth(), this.getFullYear());
};

HDate.prototype.getDate = function getDate() {
	return this.day;
};

HDate.prototype.setFullYear = function setFullYear(year) {
	this.year = year;
	fixMonth(this);
	fixDate(this);
	return this;
};

HDate.prototype.setMonth = function setMonth(month) {
	this.month = month;
	fixMonth(this);
	fixDate(this);
	return this;
};

HDate.prototype.setTishreiMonth = function setTishreiMonth(month) {
	return this.setMonth((month + 6) % c.MONTHS_IN_HEB(this.getFullYear()) || 13);
};

HDate.prototype.setDate = function setDate(date) {
	this.day = date;
	fixMonth(this);
	fixDate(this);
	return this;
};

/* convert hebrew date to absolute date */
/* Absolute date of Hebrew DATE.
   The absolute date is the number of days elapsed since the (imaginary)
   Gregorian date Sunday, December 31, 1 BC. */
function hebrew2abs(d) {
	var m, tempabs = d.getDate();
	
	if (d.getMonth() < c.months.TISHREI)
	{
		for (m = c.months.TISHREI; m <= c.MONTHS_IN_HEB(d.getFullYear()); m++)
			tempabs += c.max_days_in_heb_month(m, d.getFullYear());

		for (m = c.months.NISAN; m < d.getMonth(); m++)
			tempabs += c.max_days_in_heb_month(m, d.getFullYear());
	}
	else
	{
		for (m = c.months.TISHREI; m < d.getMonth(); m++)
			tempabs += c.max_days_in_heb_month(m, d.getFullYear());
	}
	
	
	ret = c.hebrew_elapsed_days(d.getFullYear()) - 1373429 + tempabs;
	return ret;
}
HDate.hebrew2abs = hebrew2abs;

function abs2hebrew(d) {
	var mmap = [
		c.months.KISLEV, c.months.TEVET, c.months.SHVAT, c.months.ADAR_I, c.months.NISAN,
		c.months.IYYAR, c.months.SIVAN, c.months.TAMUZ,
		c.months.TISHREI, c.months.TISHREI, c.months.TISHREI, c.months.CHESHVAN
	], hebdate, gregdate, day, month, year;

	if (d >= 10555144) {
		throw new RangeError("parameter to abs2hebrew " + d + " out of range");
	}
	
	gregdate = greg.abs2greg(d);
	hebdate = new HDate(1, c.months.TISHREI, 3760 + gregdate.getFullYear());
	year = 3760 + gregdate.getFullYear();
	
	while (hebdate.setFullYear(year + 1), d >= hebrew2abs(hebdate)) {
		year++;
	}

	if (year >= 4635 && year < 10666) {
		// optimize search
		month = mmap[gregdate.getMonth()];
	} else {
		// we're outside the usual range, so assume nothing about hebrew/gregorian calendar drift...
		month = c.months.TISHREI;
	}

	while (hebdate.setMonth(month),
		   hebdate.setDate(c.max_days_in_heb_month(month, year)),
		   hebdate.setFullYear(year),
		   d > hebrew2abs(hebdate)) {
		month = (month % c.MONTHS_IN_HEB(year)) + 1;
	}

	hebdate.setDate(1);
	
	day = parseInt(d - hebrew2abs(hebdate) + 1);
	if (day < 0) {
		throw new Error("assertion failure d < hebrew2abs(m,d,y) => " + d + " < " + hebrew2abs(hebdate) + "!");
	}

	hebdate.setDate(day);
	hebdate.setLocation(0, 0);
	
	return hebdate;
}
HDate.abs2hebrew = abs2hebrew;

HDate.prototype.greg = function() {
	return greg.abs2greg(hebrew2abs(this));
};

HDate.prototype.getMonthName = function getMonthName(o) {
	return c.LANGUAGE(c.monthNames[+this.isLeapYear()][this.getMonth()],o);
};

HDate.prototype.getSedra = function getSedra(o) {
	return (new Sedra(this.getFullYear(), this.il)).getSedraFromHebcalDate(this).map(function(p){
		return c.LANGUAGE(p, o || israel_sw);
	});
};

HDate.prototype.setCity = function setCity(city) {
	var c = cities.getCity(city);
	this.il = c[5] == cities.DST_SCHEMES.DST_ISRAEL;
	return this.setLocation(cities.getLocation(c));
};

HDate.prototype.setLocation = function setLocation(lat, long) {
	if (typeof lat == 'object' && !Array.isArray(lat)) {
		long = lat.long;
		lat = lat.lat;
	}
	if (Array.isArray(lat)) {
		lat = (lat[0] * 60 + lat[1]) / 60;
	}
	if (Array.isArray(long)) {
		long = (long[0] * 60 + long[1]) / 60;
	}
	if (typeof lat != 'number') {
		throw new TypeError('incorrect lat type passed to HDate.setLocation()');
	}
	if (typeof long != 'number') {
		throw new TypeError('incorrect long type passed to HDate.setLocation()');
	}

	this.lat = lat;
	this.long = long;

	getSunTimes(this);

	return this;
};

function getSunTimes(hdate) {
	if (!(hdate instanceof HDate) || typeof hdate.lat != 'number' || typeof hdate.long != 'number') {
		throw new TypeError('bad object given to getSunTimes()');
	}
	var g = hdate.greg(), d = new Date();
	g.setHours(d.getHours());
	g.setMinutes(d.getMinutes());

	var times = suncalc.getTimes(g, hdate.lat, hdate.long);

	hdate.sunrise = times.sunrise;
	hdate.sunset = times.sunset;
	hdate.dawn = times.dawn;
	hdate.night = times.night;

	// ms of daylight / (12 h per d * 1000 ms in s * 60 s in m) = mins in halachic hour
	hdate.hour = (hdate.sunset - hdate.sunrise) / (12 * 1000 * 60);
}

module.exports = HDate;
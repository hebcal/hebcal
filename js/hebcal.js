var c = require('./common'), HDate = require('./hdate');

function Hebcal(year, month) {
	if (!year) {
		year = (new HDate()).getFullYear(); // this year;
	}
	this.year = year;
	if (month) {
		if (typeof month == 'string') {
			month = c.lookup_hebrew_month(month);
		}
		if (typeof month == 'number') {
			this.months = [new Hebcal.Month(month, year)];
		} else if (Array.isArray(month)) {
			this.months = month.map(function(m){return new Hebcal.Month(m,year)});
		} else {
			throw new TypeError('month to Hebcal is not a valid type');
		}
	} else {
		this.months = [];
		for (var i = 1; i <= c.MONTHS_IN_HEB(year); i++) {
			this.months.push(new Hebcal.Month(i, year));
		}
	}

	this.length = c.days_in_heb_year(year);
}

Hebcal.prototype.isLeapYear = function isLeapYear() {
	return c.LEAP_YR_HEB(this.year);
};

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

	this.days = [];
	for (var i = 1; i <= c.max_days_in_heb_month(month, year); i++) {
		this.days.push(new HDate(i, month, year));
	}

	this.length = this.days.length;
};

Hebcal.Month.prototype.isLeapYear = function isLeapYear() {
	return c.LEAP_YR_HEB(this.year);
};

Hebcal.Month.prototype.prev = function prev() {
	return this.month === 1 ? new Hebcal.Month(c.MONTHS_IN_HEB(this.year - 1), this.year - 1) : new Hebcal.Month(this.month - 1, this.year);
};

Hebcal.Month.prototype.next = function next() {
	return this.month === c.MONTHS_IN_HEB(this.year) ? new Hebcal.Month(1, this.year + 1) : new Hebcal.Month(this.month + 1, this.year);
};

Hebcal.Month.prototype.getName = function getName(o) {
	return c.LANGUAGE(c.monthNames[+this.isLeapYear()][this.month],o);
};

Hebcal.Month.prototype.rosh_chodesh = function rosh_chodesh() {
	var prev = this.prev();
	return prev.length === 30 ? [prev.days[prev.days.length-1], this.days[0]] : [this.days[0]];
};

Hebcal.HDate = HDate;

HDate.prototype.getMonthObject = function getMonthObject() {
	return new Hebcal.Month(this.getMonth(), this.getFullYear());
};

module.exports = Hebcal;
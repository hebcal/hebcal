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
			return new Hebcal(year, [month]);
		} else if (Array.isArray(month)) {
			this.months = month.map(function(i){
				var m = new Hebcal.Month(i, year);
				Object.defineProperty(m, '__year', {
					configurable: true,
					writable: true,
					value: this
				});
				return m;
			}, this);
		} else {
			throw new TypeError('month to Hebcal is not a valid type');
		}
	} else {
		return new Hebcal(year, c.range(1, c.MONTHS_IN_HEB(year)));
	}

	this.length = c.days_in_heb_year(year);
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

Hebcal.prototype.setLocation = function setLocation(lat, long) {
	this.months.forEach(function(m){
		m.setLocation(lat, long);
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
			month.charCodeAt(0) >= 48 && month.charCodeAt(0) <= 57 /* number */ ? parseInt(month) : c.lookup_hebrew_month(month);
	return this.months[month > 0 ? month - 1 : this.months.length + month];
};

Hebcal.prototype.map = function map() {
	return [].map.apply(this.months, arguments);
};

Hebcal.prototype.find = function find(day, month) {
	if (arguments.length === 1) {
		if (typeof day === 'string') {
			return find.strings.call(this, day);
		} else if (Array.isArray(day)) {
			return [].concat.apply([], day.map(function(d){
				return this.find.apply(this, d);
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
					month.charCodeAt(0) >= 48 && month.charCodeAt(0) <= 57 /* number */ ? parseInt(month) : c.lookup_hebrew_month(month)
			);
		} else if (typeof month === 'number') {
			return this.find(day, this.months[month - 1]);
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
Hebcal.prototype.find.strings.today = function today() {
	return this.find(new HDate());
};

/*
Hebcal=require('./hebcal')
yr = new Hebcal()

*/

Hebcal.addZeman = HDate.addZeman;

Hebcal.cities = require('./cities')(HDate);

Hebcal.range = c.range;

Object.defineProperty(Hebcal, 'defaultLocation', {
	enumerable: true,
	configurable: true,

	get: function defaultLocationGet() {
		return HDate.defaultLocation;
	},
	set: function defaultLocationSet(loc) {
		HDate.defaultLocation = loc;
	}
});
Object.defineProperty(Hebcal, 'defaultCity', Object.getOwnPropertyDescriptor(HDate, 'defaultCity'));

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
			day.charCodeAt(0) >= 48 && day.charCodeAt(0) <= 57 /* number */ ? parseInt(day) : c.lookup_hebrew_day(day);
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
	return prev.length === 30 ? [prev.days[prev.days.length-1], this.days[0]] : [this.days[0]];
};

Hebcal.Month.prototype.setCity = function setCity(city) {
	this.days.forEach(function(d){
		d.setCity(city);
	});
	return this;
};

Hebcal.Month.prototype.setLocation = function setLocation(lat, long) {
	this.days.forEach(function(d){
		d.setLocation(lat, long);
	});
	return this;
};

Hebcal.Month.prototype.map = function map() {
	return [].map.apply(this.days, arguments);
};

Hebcal.Month.prototype.find = function find(day) {
	if (typeof day === 'number') {
		return [this.days[day - 1]];
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
		str.charCodeAt(0) >= 48 && str.charCodeAt(0) <= 57 /* number */ ? parseInt(str) : null;
	return num ? this.find(num) : [];
};
Hebcal.Month.prototype.find.strings.rosh_chodesh = function rosh_chodesh() {
	return this.rosh_chodesh();
};

Hebcal.HDate = HDate;

HDate.prototype.getMonthObject = function getMonthObject() {
	return this.__month;
};

HDate.prototype.getYearObject = function getYearObject() {
	return this.getMonthObject().getYearObject();
};

HDate.prototype.prev = function prev() {
	if (this.getMonth() === c.months.TISHREI && this.getDate() === 1) { // 1st day RH
		return this.getMonthObject().prev().getDay(-1);
	} else {
		var g = this.greg();
		g.setDate(g.getDate() - 1);
		return this.getYearObject().find(g)[0];
	}
};

HDate.prototype.next = function next() {
	if (this.getMonth() === c.months.ELUL && this.getDate() === this.getMonthObject().length) { // last day
		return this.getMonthObject().next().getDay(1);
	} else {
		var g = this.greg();
		g.setDate(g.getDate() + 1);
		return this.getYearObject().find(g)[0];
	}
};

module.exports = Hebcal;
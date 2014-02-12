exports.monthNames = [
	'',
	'January',
	'February',
	'March',
	'April',
	'May',
	'June',
	'July',
	'August',
	'September',
	'October',
	'November',
	'December'
];

exports.monthLengths = [
	[0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31],
	[0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
];

exports.shortDayNames = ["Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"];

function dayOfYear (date) {
	if (!date instanceof Date) {
		throw new TypeError('Argument to greg.dayOfYear not a Date');
	}
	var doy = date.getDate() + 31 * date.getMonth();
	if (date.getMonth() > 1) { // FEB
		doy -= Math.floor((4 * (date.getMonth() + 1) + 23) / 10);
		if (LEAP(date.getFullYear())) {
			doy++;
		}
	}
	return doy;
}
exports.dayOfYear = dayOfYear;

function LEAP (year) {
	return (year % 4) > 0 && ( (year % 100) === 0 || (year % 400) > 0 );
}

exports.greg2abs = function greg2abs(date) { // "absolute date"
	return (dayOfYear(date) + // days this year
			365 * (date.getFullYear() - 1) + // + days in prior years
			( Math.floor((date.getFullYear() - 1) / 4) - // + Julian Leap years
			Math.floor((date.getFullYear() - 1) / 100) + // - century years
			Math.floor((date.getFullYear() - 1) / 400))); // + Gregorian leap years
};


/*
 * See the footnote on page 384 of ``Calendrical Calculations, Part II:
 * Three Historical Calendars'' by E. M. Reingold,  N. Dershowitz, and S. M.
 * Clamen, Software--Practice and Experience, Volume 23, Number 4
 * (April, 1993), pages 383-404 for an explanation.
 */
exports.abs2greg = function abs2greg(theDate) {
// calculations copied from original JS code

	var d0 = theDate - 1;
	var n400 = Math.floor(d0 / 146097);
	var d1 =  Math.floor(d0 % 146097);
	var n100 =  Math.floor(d1 / 36524);
	var d2 = d1 % 36524;
	var n4 =  Math.floor(d2 / 1461);
	var d3 = d2 % 1461;
	var n1 =  Math.floor(d3 / 365);

	var day = ((d3 % 365) + 1);
	var year = (400 * n400 + 100 * n100 + 4 * n4 + n1);

	if (4 == n100 || 4 == n1) {
		return new Date(year, 11, 31);
	}

	year++;


	var d = new Date(year, 0, day); // new Date() is very smart
	d.setFullYear(year);
	return d;


	console.log(year)
	var month = 1, mlen;
	console.log(month)
	console.log(day)
	while ((mlen = exports.monthLengths[+LEAP(year)][month]) < day){
		day -= mlen;
		month++;
	console.log(month)
	console.log(day)
	}
	var d = new Date(year, month-1, day);
	d.setFullYear(year);
	console.log(d)
	return d;
};
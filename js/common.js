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
        ["Adar I",null,"אדר א׳"],
        ["Adar II",null,"אדר ב׳"],
		["Nisan",null,"ניסן"]
	]
];

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
   */
	switch (c.toLowerCase()[0]) {
		case 'n':
			return (c.toLowerCase()[1] == 'o') ?	/* this catches "november" */
				0
				: months.NISAN;
		case 'i':
			return months.IYYAR;
		case 'e':
			return months.ELUL;
		case 'c':
			return months.CHESHVAN;
		case 'k':
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
		default:
			return 0;
	}
}
exports.lookup_hebrew_month = lookup_hebrew_month;
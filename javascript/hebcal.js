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

function Hebcal(){
}

Hebcal.WKDAY_NAMES = [
                      "Sun",
                      "Mon",
                      "Tue",
                      "Wed",
                      "Thu",
                      "Fri",
                      "Sat",
];

Hebcal.MONTH_NAMES = [
                      "",
                      "ניסן",
                      "אייר",
                      "סיון",
                      "תמוז",
                      "אב",
                      "אלול",
                      "תשרי",
                      "חשון",
                      "כסלו",
                      "טבת",
                      "שבט",
                      "אדר",
                      "אדר ב׳",
                      ];                      

/* 1-based months */
function HebcalHebrewDate( hYear, hMonth, hDay ){
    this.hYear = hYear;
    this.hMonth = hMonth;
    this.hDay = hDay;
}
HebcalHebrewDate.prototype.toString = function(){ 
    return this.hDay+" " + this.getMonthName() +", "+this.hYear;
}

HebcalHebrewDate.prototype.getMonthName= function(){
    var mName = Hebcal.MONTH_NAMES[this.hMonth];
    if( LEAP_YR_HEB(this.hYear)
        && this.hMonth == HebcalHebrewDate.ADAR_I )
        mName = mName + " א׳";
    return mName;f
}

HebcalHebrewDate.NISAN = 1;
HebcalHebrewDate.IYYAR = 2;
HebcalHebrewDate.SIVAN = 3;
HebcalHebrewDate.TAMMUZ = 4;
HebcalHebrewDate.AV = 5;
HebcalHebrewDate.ELUL = 6;
HebcalHebrewDate.TISHREI = 7;
HebcalHebrewDate.CHESHVAN = 8;
HebcalHebrewDate.KISLEV = 9;
HebcalHebrewDate.TEVET = 10;
HebcalHebrewDate.SHVAT = 11;
HebcalHebrewDate.ADAR_I = 12;  //also ADAR I
HebcalHebrewDate.ADAR_II = 13;




/* due to  http://www.math.technion.ac.il/~rl/docs/gauss.pdf
*/
function Gauss(year){
    year = Math.floor(year);
    /*Fundamental constants*/ 
    var T =  (33. + 14. / 24.) ;
    var L =  ((1. +485. / 1080.) / 24. / 19.) ;
    var K=  ((29. + (12. + 793. / 1080.) / 24. )/ 19.) ;
    /*Derived constants*/ 
    var m0 =  (T - 10. * K + L + 14.);
    var m1  = ((21. + 589. / 1080.) / 24.); /* 13*19*K mod 1 */ 
    var em2 = ((15. + 204. / 1080.) / 24.); /* 1 - (12*19*K mod 1) */ 
    

	var a,b,c;
	var m;
	var	Mar;	// "day in March" on which Pesach falls (return value)

	var a = (12 * year + 17) % 19;
	var b = year % 4;
	var m  = m0 + K*a + b/4. - L*year; 
    if(m < 0) 
        m--; 
    var M = Math.floor(m); 
    if(m < 0) 
        m++; 
    m -= M; 
    var c = (M + 3 * year + 5 * b + 5) % 7;
    switch( Math.floor(c)){
    case 0: 
        if(a >= 12 && m >= m1) { 
            c=1;M++; 
        } 
        break; 
    case 1: 
        if(a >= 7 && m >= m2) { 
            c=3;
            M+=2; 
        } 
        break; 
    case2: 
        c=3;
        M++; 
        break; 
    case4: 
        c=5;
        M++; 
        break; 
    case6: 
        c=0;
        M++; 
        break; 
    default:
        error();
    } 
    
    /* convert to Gregorian Calendar*/ 
    M += (year - 3760) / 100 - (year - 3760) / 400 - 2; 
    return M; 
}


/**
   convert an absolute day number to a HebcalHebrewDate
 */
function abs2hebrew( d )
{
    var mmap = [
                HebcalHebrewDate.KISLEV, 
                HebcalHebrewDate.TEVET, 
                HebcalHebrewDate.SHVAT, 
                HebcalHebrewDate.ADAR_I,
                HebcalHebrewDate.NISAN,
                HebcalHebrewDate.IYYAR, 
                HebcalHebrewDate.SIVAN, 
                HebcalHebrewDate.TAMMUZ, 
                HebcalHebrewDate.TISHREI, 
                HebcalHebrewDate.TISHREI, 
                HebcalHebrewDate.TISHREI, 
                HebcalHebrewDate.CHESHVAN,
                ];
    //    date_t hebdate, gregdate;
    //    int day, month, year;

    if( d >= 10555144 )  {
        assert("parameter to abs2hebrew "+d+"out of range",false);
    }

    var hebdate = new HebcalHebrewDate(0,0,0);
    
    var gregdate = abs2greg( d );
    hebdate.hDay = 1;
    hebdate.hMonth = HebcalHebrewDate.TISHREI;
    var year = 3760 + gregdate.gYear;
    
    while (hebdate.hYear = year + 1,
           d >= hebrew2abs( hebdate ))
        year++;

    var month;
    if( year >= 4635 && year < 10666  )
    {
        /* optimize search */
        month = mmap[gregdate.gMonth - 1];
    }
    else
    {
        /* we're outside the usual range, so assume nothing about hebrew/gregorian calendar drift... */
        month = HebcalHebrewDate.TISHREI;
    }

    while (hebdate.hMonth = month,
           hebdate.hDay = max_days_in_heb_month (month, year),
           hebdate.hYear = year,
           d > hebrew2abs (hebdate))
        month = (month % MONTHS_IN_HEB(year)) + 1;

    hebdate.hDay = 1;
    
    var day = d - hebrew2abs (hebdate) + 1;
    if( day <= 0)
    {
        assert( "assertion failure d < hebrew2abs(m,d,y) => %ld < %ld!\n"+ d +  hebrew2abs(hebdate), false);

    }

    hebdate.hDay = day;
    
    return hebdate;
}


function DOW(day,month,year) {
	var a = Math.floor((14 - month)/12);
	var y = year - a;
	var m = month + 12*a - 2;
	var d = (day + y + Math.floor(y/4) - Math.floor(y/100) +
			Math.floor(y/400) + Math.floor((31*m)/12)) % 7;
	return d + 1;
}

/* convert hebrew date to absolute date */
/*Absolute date of Hebrew DATE.
   The absolute date is the number of days elapsed since the (imaginary)
   Gregorian date Sunday, December 31, 1 BC. */
hebrew2abs = function ( hDate ){
    assert("hebrew2abs",hDate instanceof HebcalHebrewDate);

    var m;
    var  tempabs = hDate.hDay;

    
    if (hDate.hMonth < HebcalHebrewDate.TISHREI)
    {
        for (m = HebcalHebrewDate.TISHREI; m <= MONTHS_IN_HEB (hDate.hYear); m++)
            tempabs +=  max_days_in_heb_month(m, hDate.hYear);

        for (m =  HebcalHebrewDate.NISAN; m < hDate.hMonth; m++)
            tempabs +=  max_days_in_heb_month(m, hDate.hYear);
    }
    else
    {
        for (m =  HebcalHebrewDate.TISHREI; m < hDate.hMonth; m++)
            tempabs +=  max_days_in_heb_month(m, hDate.hYear);
    }
    
    
    var el = hebrew_elapsed_days( hDate.hYear );
    var ret = el - 1373429 + tempabs;
    return ret;


};



function hebrew_elapsed_days(hYear){
    
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

function days_in_heb_year( hYear )
{
    return  hebrew_elapsed_days(hYear + 1) - hebrew_elapsed_days(hYear);
}

function long_cheshvan( hYear ){
    return (days_in_heb_year(hYear) % 10) == 5;
}

function short_kislev( hYear ){
    return (days_in_heb_year(hYear) % 10) == 3;
}

/**
   return true if hYear is a Hebrew leap year
 */
function LEAP_YR_HEB( hYear ) {
    return  (1 + hYear*7 ) % 19 < 7;
}

/**
   return the number of months in hebrew year hYear
 */
function MONTHS_IN_HEB( hYear ) {
    return LEAP_YR_HEB( hYear ) ? 13 :12;
}

function max_days_in_heb_month(  month, year ){
    if ( month == HebcalHebrewDate.IYYAR ||
         month == HebcalHebrewDate.TAMMUZ || 
         month == HebcalHebrewDate.ELUL ||
         month == HebcalHebrewDate.TEVET || 
         month == HebcalHebrewDate.ADAR_II ||
         (month == HebcalHebrewDate.ADAR_I && !LEAP_YR_HEB( year )) ||
         (month == HebcalHebrewDate.CHESHVAN && !long_cheshvan( year )) ||
         (month == HebcalHebrewDate.KISLEV && short_kislev( year )))
        return 29;
    else
        return 30;
}


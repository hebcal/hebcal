// $Header$
/*
   Hebcal - A Jewish Calendar Generator
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


// #define SUN 0
// #define MON 1
// #define TUE 2
// #define WED 3
// #define THU 4
// #define FRI 5
// #define SAT 6

// #define JAN 1
HebcalGregorianDate.FEB = 2;
// #define MAR 3
// #define APR 4
// #define MAY 5
// #define JUN 6
// #define JUL 7
// #define AUG 8
// #define SEP 9
// #define OCT 10
// #define NOV 11
// #define DEC 12

/* 1-based months */
function HebcalGregorianDate( gYear, gMonth, gDay ){
    this.gYear = gYear;
    this.gMonth = gMonth;
    this.gDay = gDay;
}

HebcalGregorianDate.prototype.toString = function(){
    return "["+this.gYear  +"-"
    + this.gMonth  +"-"
    + this.gDay + "]";
}



/**
   return true if  year is  a gregorian leap year.
 */
function leap(y) {
    y = Math.floor(y);
	return (y % 400 == 0) 
            || (y % 100 != 0 
                && y % 4 == 0);
}

/*
 *Return the day number within the year of the date DATE.
 *For example, dayOfYear({1,1,1987}) returns the value 1
 *while dayOfYear({12,31,1980}) returns 366.
 */
function dayOfYear( gregDt )
{
    var dOY = gregDt.gDay + 31 * (gregDt.gMonth - 1);
    if (gregDt.gMonth > HebcalGregorianDate.FEB)
    {
        dOY -= Math.floor((4 * gregDt.gMonth + 23) / 10);
        if (leap(gregDt.gYear))
            dOY++;
    }
    return dOY;
}



/**
   takes a HebcalGregorianDate as input and returns a 
   Julian day number as output.
 */
function greg2abs(  gregDt )			/* "absolute date" */ 
{
    return (dayOfYear (gregDt)	/* days this year */
            + 365 *  (gregDt.gYear - 1)	/* + days in prior years */
            +  ( Math.floor((gregDt.gYear - 1) / 4)	/* + Julian Leap years */
                - Math.floor((gregDt.gYear - 1) / 100)	/* - century years */
                + Math.floor((gregDt.gYear - 1) / 400)));	/* + Gregorian leap years */
}


HebcalGregorianDate.GregMonthLengths = 
[
    [0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31],
    [0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
];


/*
 * See the footnote on page 384 of ``Calendrical Calculations, Part II:
 * Three Historical Calendars'' by E. M. Reingold,  N. Dershowitz, and S. M.
 * Clamen, Software--Practice and Experience, Volume 23, Number 4
 * (April, 1993), pages 383-404 for an explanation.
 */
function abs2greg( theDate ){
//   int day, year, month, mlen;
//   date_t d;
//   long int d0, n400, d1, n100, d2, n4, d3, n1;

  var d0 = theDate - 1;
  var n400 = Math.floor(d0 / 146097);
  var d1 =  Math.floor(d0 % 146097);
  var n100 =  Math.floor(d1 / 36524);
  var d2 = d1 % 36524;
  var n4 =  Math.floor(d2 / 1461);
  var d3 = d2 % 1461;
  var n1 =  Math.floor(d3 / 365);

  var day =  ((d3 % 365) + 1);
  var year =  (400 * n400 + 100 * n100 + 4 * n4 + n1);

  if (4 == n100 || 4 == n1)
      return new HebcalGregorianDate( year, 12, 31 );

  year++;
  month = 1;
  while ((mlen = HebcalGregorianDate.GregMonthLengths[+leap(year)][month]) < day){
      day -= mlen;
      month++;
  }
  return new HebcalGregorianDate( year, month, day );
}


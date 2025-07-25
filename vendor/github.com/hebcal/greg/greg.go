// Hebcal's greg package converts between Gregorian/Julian dates
// and R.D. (Rata Die) day numbers.
// An R.D. number consists of an absolute number equal the number of days
// that elapsed from a fixed point in time. We use negative numbers to represent
// days before that fixed point. For this library, we use the Gregorian date 12/31/1 BCE
// (or the equivalent Julian date 1/2/1 CE) as our fixed point.
// We use the Julian calendar before Sep 1752, and the Gregorian one thereafter.

// Note: Comments contain dates in American format (mm/dd/yy)

package greg

// Hebcal - A Jewish Calendar Generator
// Copyright (c) 2022 Michael J. Radwin
// Derived from original C version, Copyright (C) 1994-2004 Danny Sadinoff
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

import (
	"math"
	"time"
)

// the dates on which we switch from Julian to Gregorian
// all days between these dates (exclusive) are invalid
var GREG_ADJUSTMENT_BEGIN = time.Date(1752, time.September, 2, 0, 0, 0, 0, time.Local)
var GREG_ADJUSTMENT_END = time.Date(1752, time.September, 14, 0, 0, 0, 0, time.Local)

// 1-based month lengths
var monthLen = [13]int{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}

// DaysIn returns the number of days in the Gregorian/Julian month.
func DaysIn(m time.Month, year int) int {
	if m == time.February && IsLeapYear(year) {
		return 29
	}
	return monthLen[m]
}

// Returns true if the year is a leap year
func IsLeapYear(year int) bool {

	y := int64(year)

	// adjust and pretend 0 was a valid year to simplify the math
	// this change is internal and localized to this function
	if y < 0 {
		y++
	}

	if mod(y, 400) == 0 {
		return true
	}

	if y > int64(GREG_ADJUSTMENT_BEGIN.Year()) && mod(y, 100) == 0 {
		return false
	}

	if mod(y, 4) == 0 {
		return true
	}

	return false
}

// Converts given date to absolute R.D. (Rata Die) days.
// Hours, minutes and seconds are ignored
func DateToRD(t time.Time) int64 {
	year, month, day := t.Date()
	abs := ToRD(year, month, day)
	return abs
}

// This function helps with a trick we later use to find the absolute number
// of the current day this year. The idea is that if we are in Jan or Feb,
// our approximation is exactly accurate. But for later months we need to subtract 1
// if it's a leap year, and 2 if it isn't
func monthOffset(year int, month time.Month) int {
	if month <= time.February {
		return 0
	} else if IsLeapYear(year) {
		return -1
	} else {
		return -2
	}
}

// Converts given date to absolute R.D. (Rata Die) days.
// Panics if the year is 0
func ToRD(year int, month time.Month, day int) int64 {

	// ensure year is valid
	if year == 0 {
		panic("Error: There was no year 0")
	}

	// ensure date is valid
	if year >= GREG_ADJUSTMENT_BEGIN.Year() && year <= GREG_ADJUSTMENT_END.Year() &&
		month >= GREG_ADJUSTMENT_BEGIN.Month() && month <= GREG_ADJUSTMENT_END.Month() &&
		day > GREG_ADJUSTMENT_BEGIN.Day() && day < GREG_ADJUSTMENT_END.Day() {
		panic("Error: Date is between GREG_ADJUSTMENT_BEGIN and GREG_ADJUSTMENT_END")
	}

	var py int64

	if year > 0 {
		py = int64(year - 1)
	} else {
		py = int64(year)
	}

	abs := 365*py + // days up to preceding year
		quotient(py, 4) + // add in Julian leap years
		quotient((367*int64(month)-362), 12) + // add in the days so far this year
		int64(monthOffset(year, month)) + int64(day)

	if year > GREG_ADJUSTMENT_BEGIN.Year() ||
		(year == GREG_ADJUSTMENT_BEGIN.Year() && (month > GREG_ADJUSTMENT_BEGIN.Month() ||
			(month == GREG_ADJUSTMENT_BEGIN.Month() &&
				day > GREG_ADJUSTMENT_BEGIN.Day()))) {
		abs -= quotient(py, 100) // subtract out century leap years
		abs += quotient(py, 400) // add in Gregorian leap years
	} else {
		// absolute dates obtained from Julian dates need to be adjusted because the
		// Julian date 1/2/1 is the equivlent of Gregorian 12/31/1 BC, so the number
		// of days since Julian 12/31/1 BC is 2 greater than since Gregorian 12/31/1 BC
		abs -= 2
	}

	return abs
}

// the next two functions allow us to deal with negative R.D. numbers
func mod(x, y int64) int64 {
	X := float64(x)
	Y := float64(y)
	return int64(X - Y*math.Floor(X/Y))
}

func quotient(x, y int64) int64 {
	return int64(math.Floor(float64(x) / float64(y)))
}

/*
Finds the year in which a given R.D. occurs
Returns the year and remaining days
See the footnote on page 384 of “Calendrical Calculations, Part II:
Three Historical Calendars” by E. M. Reingold,  N. Dershowitz, and S. M.
Clamen, Software--Practice and Experience, Volume 23, Number 4
(April, 1993), pages 383-404 for an explanation.
*/
func yearFromFixed(rataDie int64) (int, int64) {

	l0 := int64(rataDie) - 1 // subtract 1 because we are counting from Gregorian date 12/31/1 BCE (Julian 1/2/1)

	// get the absolute date for Sep, 2 1752
	adj_date := DateToRD(GREG_ADJUSTMENT_BEGIN)

	// Gregorian 12/31/1 BCE = Julian 1/2/1 CE
	// So if we are in the Julian calendar, add 2
	// so that we are counting from Julian 12/31/1 BCE
	if rataDie <= adj_date {
		l0 += 2
	}

	// number of 100 and 400 year periods
	// and days into current period
	var n400, n100, d1, d2 int64

	if rataDie > adj_date {
		n400 = quotient(l0, 146097)
		d1 = mod(l0, 146097)
		n100 = quotient(d1, 36524)
		d2 = mod(d1, 36524)
	} else {
		n400 = quotient(l0, 146100)
		d1 = mod(l0, 146100)
		n100 = quotient(d1, 36525)
		d2 = mod(d1, 36525)
	}

	n4 := quotient(d2, 1461) // number of 4 year periods
	d3 := mod(d2, 1461)
	n1 := quotient(d3, 365) // number of years into current period
	day := mod(d3, 365)
	year := 400*n400 + 100*n100 + 4*n4 + n1 // total years
	yy := int(year)

	// at this point, year 1 is represented as 0
	if yy < 0 {
		yy -= 1
	}

	// if we didn't get a 4-year block (with a leap day in it), but we did get 4 separate years
	// it must be December 31 on the previous year (because there was a leap day). So don't add 1
	if n100 == 4 || n1 == 4 {

		// but first, if yy is 0, then we are actually in 1 BCE
		if yy != 0 {
			return yy, 365
		} else {
			// year 1 BCE was a leap year
			return -1, 365
		}
	}

	// otherwise it is the next year (because there is no year 0, so generally add 1)
	return yy + 1, day
}

// Converts from Rata Die (R.D. number) to Gregorian/Julian date.
func FromRD(rataDie int64) (int, time.Month, int) {
	year, day := yearFromFixed(rataDie) // get the year we are in and the day within the year

	day++

	month := 1
	for ; month <= 12; month++ {
		mlen := int64(DaysIn(time.Month(month), year))
		if mlen >= day {
			break
		} else {
			day -= mlen
		}
	}

	// var correction int64
	// if rataDie < ToRD(year, time.March, 1) {
	// 	correction = 0
	// } else if IsLeapYear(year) {
	// 	correction = 1
	// } else {
	// 	correction = 2
	// }
	// month := quotient(12*(priorDays+correction)+373, 367) // trick to find month
	// day := rataDie - ToRD(year, time.Month(month), 1) + 1 // find remaining days
	return year, time.Month(month), int(day)
}

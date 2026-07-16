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
	"time"
)

// The dates on which we switch from the Julian to the Gregorian calendar.
// All days between these dates (exclusive) are invalid.
//
// These are the canonical values; the equivalent GREG_ADJUSTMENT_BEGIN and
// GREG_ADJUSTMENT_END variables are derived from them.
const (
	gregAdjustmentBeginYear  = 1752
	gregAdjustmentBeginMonth = time.September
	gregAdjustmentBeginDay   = 2

	gregAdjustmentEndYear  = 1752
	gregAdjustmentEndMonth = time.September
	gregAdjustmentEndDay   = 14

	// gregAdjustmentBeginRD is the R.D. number of GREG_ADJUSTMENT_BEGIN, the
	// final day of the Julian period. Any R.D. at or below it is Julian.
	// TestGregAdjustmentBeginRD asserts this stays in sync with ToRD.
	gregAdjustmentBeginRD = 639796
)

// The dates on which we switch from Julian to Gregorian.
// All days between these dates (exclusive) are invalid.
var (
	GREG_ADJUSTMENT_BEGIN = time.Date(gregAdjustmentBeginYear, gregAdjustmentBeginMonth, gregAdjustmentBeginDay, 0, 0, 0, 0, time.Local)
	GREG_ADJUSTMENT_END   = time.Date(gregAdjustmentEndYear, gregAdjustmentEndMonth, gregAdjustmentEndDay, 0, 0, 0, 0, time.Local)
)

// 1-based month lengths in a non-leap year.
var monthLen = [13]int{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}

// DaysIn returns the number of days in the Gregorian/Julian month.
func DaysIn(m time.Month, year int) int {
	if m == time.February && IsLeapYear(year) {
		return 29
	}
	return monthLen[m]
}

// IsLeapYear returns true if the year is a leap year.
//
// Years before the switch to the Gregorian calendar follow the Julian rule
// (every 4th year), so the century exception does not apply to them.
func IsLeapYear(year int) bool {
	// Adjust and pretend 0 was a valid year to simplify the math.
	// This change is internal and localized to this function.
	if year < 0 {
		year++
	}

	// A remainder is zero under floor division exactly when it is zero under
	// Go's truncating %, regardless of sign, so % is safe here.
	if year%400 == 0 {
		return true
	}
	if year > gregAdjustmentBeginYear && year%100 == 0 {
		return false
	}
	return year%4 == 0
}

// DateToRD converts the given date to absolute R.D. (Rata Die) days.
// Hours, minutes and seconds are ignored.
func DateToRD(t time.Time) int64 {
	year, month, day := t.Date()
	return ToRD(year, month, day)
}

// monthOffset helps with a trick we later use to find the absolute number
// of the current day this year. The idea is that if we are in Jan or Feb,
// our approximation is exactly accurate. But for later months we need to
// subtract 1 if it's a leap year, and 2 if it isn't.
func monthOffset(year int, month time.Month) int64 {
	if month <= time.February {
		return 0
	}
	if IsLeapYear(year) {
		return -1
	}
	return -2
}

// ToRD converts the given date to absolute R.D. (Rata Die) days.
//
// It panics if the year is 0 (there was no year 0), or if the date falls in
// the gap between GREG_ADJUSTMENT_BEGIN and GREG_ADJUSTMENT_END, which never
// occurred.
func ToRD(year int, month time.Month, day int) int64 {
	if year == 0 {
		panic("Error: There was no year 0")
	}

	if year >= gregAdjustmentBeginYear && year <= gregAdjustmentEndYear &&
		month >= gregAdjustmentBeginMonth && month <= gregAdjustmentEndMonth &&
		day > gregAdjustmentBeginDay && day < gregAdjustmentEndDay {
		panic("Error: Date is between GREG_ADJUSTMENT_BEGIN and GREG_ADJUSTMENT_END")
	}

	// Days up to the preceding year. There is no year 0, so positive years
	// step down by one and negative years are already "off by one".
	py := int64(year)
	if year > 0 {
		py--
	}

	abs := 365*py + // days up to preceding year
		quotient(py, 4) + // add in Julian leap years
		quotient(367*int64(month)-362, 12) + // add in the days so far this year
		monthOffset(year, month) +
		int64(day)

	if isGregorian(year, month, day) {
		abs -= quotient(py, 100) // subtract out century leap years
		abs += quotient(py, 400) // add in Gregorian leap years
	} else {
		// Absolute dates obtained from Julian dates need to be adjusted because
		// the Julian date 1/2/1 is the equivalent of Gregorian 12/31/1 BCE, so
		// the number of days since Julian 12/31/1 BCE is 2 greater than since
		// Gregorian 12/31/1 BCE.
		abs -= 2
	}

	return abs
}

// isGregorian reports whether the date falls after the switch from the Julian
// to the Gregorian calendar.
func isGregorian(year int, month time.Month, day int) bool {
	if year != gregAdjustmentBeginYear {
		return year > gregAdjustmentBeginYear
	}
	if month != gregAdjustmentBeginMonth {
		return month > gregAdjustmentBeginMonth
	}
	return day > gregAdjustmentBeginDay
}

// quotient returns the quotient of x/y rounded toward negative infinity.
// Go's / operator truncates toward zero, which differs from floor division
// when exactly one operand is negative, so the R.D. math (which relies on
// floor semantics for dates BCE) corrects for that case here.
func quotient(x, y int64) int64 {
	q := x / y
	if x%y != 0 && (x < 0) != (y < 0) {
		q--
	}
	return q
}

// mod returns x modulo y, taking the sign of y, to match quotient's floor
// division. It is the companion of quotient: x == y*quotient(x,y) + mod(x,y).
func mod(x, y int64) int64 {
	m := x % y
	if m != 0 && (m < 0) != (y < 0) {
		m += y
	}
	return m
}

// divmod returns quotient(x, y) and mod(x, y) together. The calendar cycle
// math needs both for the same operands, and computing them in one step lets
// the compiler emit a single division rather than two.
func divmod(x, y int64) (q, m int64) {
	q, m = x/y, x%y
	if m != 0 && (m < 0) != (y < 0) {
		q--
		m += y
	}
	return q, m
}

/*
yearFromFixed finds the year in which a given R.D. occurs.
Returns the year and remaining days.
See the footnote on page 384 of “Calendrical Calculations, Part II:
Three Historical Calendars” by E. M. Reingold,  N. Dershowitz, and S. M.
Clamen, Software--Practice and Experience, Volume 23, Number 4
(April, 1993), pages 383-404 for an explanation.
*/
func yearFromFixed(rataDie int64) (int, int64) {
	// Subtract 1 because we are counting from Gregorian 12/31/1 BCE (Julian 1/2/1).
	l0 := rataDie - 1

	// Number of 100 and 400 year periods, and days into the current period.
	// The cycle lengths differ between the calendars: the Julian calendar has a
	// leap day every 4 years with no century exception (146100 days per 400
	// years), while the Gregorian calendar drops 3 of them (146097).
	var n400, n100, d1, d2 int64
	if rataDie > gregAdjustmentBeginRD {
		n400, d1 = divmod(l0, 146097)
		n100, d2 = divmod(d1, 36524)
	} else {
		// Gregorian 12/31/1 BCE = Julian 1/2/1 CE, so in the Julian calendar add
		// 2 to count from Julian 12/31/1 BCE.
		l0 += 2
		n400, d1 = divmod(l0, 146100)
		n100, d2 = divmod(d1, 36525)
	}

	n4, d3 := divmod(d2, 1461) // number of 4 year periods
	n1, day := divmod(d3, 365) // number of years into current period, days into that year

	year := int(400*n400 + 100*n100 + 4*n4 + n1) // total years

	// At this point, year 1 is represented as 0.
	if year < 0 {
		year--
	}

	// If we didn't get a 4-year block (with a leap day in it), but we did get 4
	// separate years, it must be December 31 of the previous year (because there
	// was a leap day). So don't add 1.
	if n100 == 4 || n1 == 4 {
		if year == 0 {
			// Year 1 BCE was a leap year.
			return -1, 365
		}
		return year, 365
	}

	// Otherwise it is the next year (because there is no year 0, so generally add 1).
	return year + 1, day
}

// FromRD converts from R.D. (Rata Die) number to a Gregorian/Julian date.
func FromRD(rataDie int64) (int, time.Month, int) {
	year, day := yearFromFixed(rataDie) // the year, and the day within that year
	day++

	// Hoist the leap year test out of the loop; it is the same for every month.
	leap := IsLeapYear(year)

	month := time.January
	for ; month <= time.December; month++ {
		mlen := int64(monthLen[month])
		if leap && month == time.February {
			mlen++
		}
		if mlen >= day {
			break
		}
		day -= mlen
	}

	return year, month, int(day)
}

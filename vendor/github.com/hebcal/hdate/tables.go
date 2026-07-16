package hdate

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

// The length of a Hebrew year determines everything else about its shape:
// whether it is a leap year, whether Cheshvan is long, and whether Kislev is
// short. There are only six possible lengths, so the month lengths and the
// day offset of each month within the year can be tabulated once at startup
// and then looked up in constant time, instead of being rederived with a loop
// over the months on every conversion.
//
// yearType maps a year length to an index in [0, numYearTypes):
//
//	0: 353   3: 383   (deficient, Kislev short)
//	1: 354   4: 384   (regular)
//	2: 355   5: 385   (complete, Cheshvan long)
//
// with 0-2 common years and 3-5 leap years.
const (
	numYearTypes  = 6
	maxDaysInYear = 385
)

// yearType returns the index into the tables below for a year of daysInYear
// days. daysInYear must be one of the six values DaysInYear can return.
func yearType(daysInYear int) int {
	t := daysInYear%10 - 3 // 353 -> 0, 354 -> 1, 355 -> 2
	if daysInYear > 355 {
		t += 3 // leap years shift by 3
	}
	return t
}

// yearLenOfType is the inverse of yearType.
func yearLenOfType(t int) int {
	return 353 + t%3 + 30*(t/3)
}

var (
	// daysInMonthTab[t][m] is the number of days in month m of a year of
	// type t. Indexed by HMonth, so index 0 is unused.
	daysInMonthTab [numYearTypes][Adar2 + 1]int8

	// monthOffsetTab[t][m] is the number of days from Tishrei 1 to the first
	// of month m in a year of type t. Months are indexed in calendar order
	// (Nisan = 1), but a Hebrew year begins in Tishrei, so Nisan through Elul
	// have larger offsets than Tishrei through Adar II.
	//
	// Sized to hold the out-of-range indices 0 and Adar2+1 as well, so that
	// ToRD reproduces the old month-summing loop for every input it accepted.
	monthOffsetTab [numYearTypes][Adar2 + 2]int16

	// monthByDayOfYear[t][d] is the month containing the d'th day (0-based)
	// after Tishrei 1 in a year of type t. This turns the month search in
	// FromRD into a single lookup.
	monthByDayOfYear [numYearTypes][maxDaysInYear]int8
)

func init() {
	for t := 0; t < numYearTypes; t++ {
		yearLen := yearLenOfType(t)
		leap := yearLen > 355
		longCheshvan := yearLen%10 == 5
		shortKislev := yearLen%10 == 3

		for m := Nisan; m <= Adar2; m++ {
			days := 30
			switch m {
			case Iyyar, Tamuz, Elul, Tevet, Adar2:
				days = 29
			case Adar1:
				if !leap {
					days = 29 // plain Adar
				}
			case Cheshvan:
				if !longCheshvan {
					days = 29
				}
			case Kislev:
				if shortKislev {
					days = 29
				}
			}
			daysInMonthTab[t][m] = int8(days)
		}

		// The last month of the year, i.e. HMonth(MonthsInYear(year)).
		lastMonth := Adar1
		if leap {
			lastMonth = Adar2
		}

		for m := HMonth(0); m <= Adar2+1; m++ {
			offset := 0
			if m < Tishrei {
				for x := Tishrei; x <= lastMonth; x++ {
					offset += daysInMonth(t, x)
				}
				for x := Nisan; x < m; x++ {
					offset += daysInMonth(t, x)
				}
			} else {
				for x := Tishrei; x < m; x++ {
					offset += daysInMonth(t, x)
				}
			}
			monthOffsetTab[t][m] = int16(offset)
		}

		fill := func(m HMonth) {
			offset := int(monthOffsetTab[t][m])
			for i := 0; i < daysInMonth(t, m); i++ {
				monthByDayOfYear[t][offset+i] = int8(m)
			}
		}
		for m := Tishrei; m <= lastMonth; m++ {
			fill(m)
		}
		for m := Nisan; m <= Elul; m++ {
			fill(m)
		}
	}
}

// daysInMonth returns the length of month m in a year of type t. Months
// outside [Nisan, Adar2] report 30 days, matching the exported DaysInMonth.
func daysInMonth(t int, m HMonth) int {
	if m < Nisan || m > Adar2 {
		return 30
	}
	return int(daysInMonthTab[t][m])
}

// monthOffset returns the number of days from Tishrei 1 to the first of month
// m in a year of type t.
func monthOffset(t int, m HMonth) int64 {
	if m < 0 || m > Adar2+1 {
		// Out of the tabulated range. Nothing in this package produces such a
		// month; fall back to summing so that ToRD stays total for any input.
		offset := 0
		if m < Tishrei {
			lastMonth := Adar1
			if yearLenOfType(t) > 355 {
				lastMonth = Adar2
			}
			for x := Tishrei; x <= lastMonth; x++ {
				offset += daysInMonth(t, x)
			}
			for x := Nisan; x < m; x++ {
				offset += daysInMonth(t, x)
			}
		} else {
			for x := Tishrei; x < m; x++ {
				offset += daysInMonth(t, x)
			}
		}
		return int64(offset)
	}
	return int64(monthOffsetTab[t][m])
}

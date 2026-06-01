// Hebcal's tanakhyomi package calculates Tanakh Yomi, a schedule for
// completing the Tanakh annually according to the ancient Masoretic
// division into sedarim. The cycle begins on 23 Tishrei (the day after
// Shmini Atzeret in Israel) and skips Shabbat and the major festivals.
package tanakhyomi

// Hebcal - A Jewish Calendar Generator
// Copyright (c) 2026 Michael J. Radwin
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
	"strconv"
	"sync"
	"time"

	"github.com/hebcal/greg"
	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/event"
	"github.com/hebcal/hebcal-go/hebcal"
)

const (
	joshua   = "Joshua"
	jeremiah = "Jeremiah"
	ruth     = "Ruth"
	shir     = "Song of Songs"
)

// TanakhYomiStart is the R.D. number of the start of the Tanakh Yomi
// cycle, Tuesday 26 October 1948 (23 Tishrei 5709).
var TanakhYomiStart = greg.ToRD(1948, time.October, 26)

type book struct {
	name  string
	blatt int
}

var books = []book{
	{joshua, 14},
	{"Judges", 14},
	{"Samuel", 34},
	{"Kings", 35},
	{"Isaiah", 26},
	{jeremiah, 31},
	{"Ezekiel", 29},
	{"Minor Prophets", 21},
	{"Psalms", 19},
	{"Proverbs", 8},
	{"Job", 8},
	{shir, 1},
	{ruth, 1},
	{"Lamentations", 1},
	{"Ecclesiastes", 4},
	{"Esther", 5},
	{"Daniel", 7},
	{"Ezra and Nehemiah", 10},
	{"Chronicles", 25},
	{"Chronicles", 25},
}

// Holidays (besides Shabbat and any CHAG) on which Tanakh Yomi is skipped.
var toSkip = map[string]bool{
	"Purim":                  true,
	"Yom HaAtzma'ut":         true,
	"Tish'a B'Av":            true,
	"Tish'a B'Av (observed)": true,
}

// Reading is one day's Tanakh Yomi reading: a single Masoretic seder
// (verse range) within one of the books of Tanakh.
type Reading struct {
	Name   string // Book name, e.g. "Isaiah", "Minor Prophets", "Chronicles"
	Blatt  string // Seder number, usually numeric but may be e.g. "4.1"
	Verses string // Masoretic verse range, e.g. "55:13-58:13" or "Zephaniah 3:20"
}

// String returns the untranslated description, e.g. "Isaiah Seder 23".
func (r Reading) String() string {
	return r.Name + " Seder " + r.Blatt
}

func makeReading(name, blatt string) Reading {
	return Reading{Name: name, Blatt: blatt, Verses: masoreticVerses(name, blatt)}
}

func masoreticVerses(name, blatt string) string {
	if n, err := strconv.Atoi(blatt); err == nil {
		arr := masoreticRegular[name]
		if n >= 1 && n <= len(arr) {
			return arr[n-1]
		}
		return ""
	}
	return masoreticSplit[name][blatt]
}

// New calculates the Tanakh Yomi reading for the given date.
//
// The second return value is false when there is no reading: before the
// cycle began (26 October 1948), on Shabbat, and on the major festivals
// the schedule skips (Pesach I/VII, Shavuot, Rosh Hashana, Yom Kippur,
// Sukkot I, Shmini Atzeret, Purim, Yom HaAtzma'ut, Tish'a B'Av).
func New(hd hdate.HDate) (Reading, bool) {
	cday := hd.Abs()
	if cday < TanakhYomiStart {
		return Reading{}, false
	}
	if skipDay(hd) {
		return Reading{}, false
	}
	hyear := hd.Year()
	rh := hdate.ToRD(hyear, hdate.Tishrei, 1)
	startAbs := rh + 22 // 23 Tishrei
	if cday < startAbs {
		// Tail of the previous year's cycle (Chronicles), read 1-22 Tishrei.
		rhDow := rh % 7
		blatt := 12
		if rhDow == 4 {
			blatt = 11
		} else if rhDow == 6 {
			blatt = 10
		}
		for i := rh + 2; i < cday; i++ {
			if !skipDayAbs(i) {
				blatt++
			}
		}
		return makeReading("Chronicles", strconv.Itoa(blatt)), true
	}

	total := 0
	for i := startAbs; i < cday; i++ {
		if !skipDayAbs(i) {
			total++
		}
	}
	rt := makeReadingTable(hyear)
	for _, el := range rt.table {
		if total < el.blatt {
			blatt := total + 1
			name := el.name
			if (rt.longShir && name == shir) || (rt.longRuth && name == ruth) {
				return makeReading(name, "1."+strconv.Itoa(blatt)), true
			}
			if rt.longJoshua && name == joshua && blatt >= 4 {
				switch blatt {
				case 4:
					return makeReading(name, "4.1"), true
				case 5:
					return makeReading(name, "4.2"), true
				default:
					return makeReading(name, strconv.Itoa(blatt-1)), true
				}
			}
			if rt.longJeremiah && name == jeremiah && blatt >= 9 {
				switch blatt {
				case 9:
					return makeReading(name, "9.1"), true
				case 10:
					return makeReading(name, "9.2"), true
				default:
					return makeReading(name, strconv.Itoa(blatt-1)), true
				}
			}
			return makeReading(name, strconv.Itoa(blatt)), true
		}
		total -= el.blatt
	}
	panic("internal error: tanakh yomi reading not found")
}

type readingTable struct {
	table        []book
	longRuth     bool
	longShir     bool
	longJeremiah bool
	longJoshua   bool
}

// makeReadingTable adjusts the seder counts for the given Hebrew year so
// that the cycle exactly fills the available reading days, splitting
// chapters at the end of the book list when there are extra days.
func makeReadingTable(year int) readingTable {
	numDays := calculateNumDaysToRead(year)
	count := numDays
	if hdate.IsLeapYear(year) {
		count = numDays - 25
	}
	extra := count - 293
	table := append([]book(nil), books...)
	rt := readingTable{table: table}
	switch extra {
	case 0:
		// no adjustments
	case 4:
		table[0] = book{joshua, 15} // Joshua 4 splits across two days
		rt.longJoshua = true
		fallthrough
	case 3:
		table[5] = book{jeremiah, 32} // Jeremiah 9 splits across two days
		rt.longJeremiah = true
		fallthrough
	case 2:
		table[11] = book{shir, 2} // Shir HaShirim gets 2 days
		rt.longShir = true
		fallthrough
	case 1:
		table[12] = book{ruth, 2} // Ruth gets 2 days
		rt.longRuth = true
	default:
		panic("tanakh yomi: unexpected extra count for year " + strconv.Itoa(year))
	}
	return rt
}

// calculateNumDaysToRead counts the reading days (excluding Shabbat and
// skipped festivals) from 23 Tishrei of year to 22 Tishrei of year+1.
func calculateNumDaysToRead(year int) int {
	startAbs := hdate.ToRD(year, hdate.Tishrei, 23)
	endAbs := hdate.ToRD(year+1, hdate.Tishrei, 22)
	included := 0
	for abs := startAbs; abs <= endAbs; abs++ {
		if !skipDayAbs(abs) {
			included++
		}
	}
	return included
}

func skipDayAbs(abs int64) bool {
	return skipDay(hdate.FromRD(abs))
}

// skipDay reports whether Tanakh Yomi is not studied on the given date:
// Shabbat, any CHAG, or one of the toSkip holidays (Israel schedule).
func skipDay(hd hdate.HDate) bool {
	if hd.Weekday() == time.Saturday {
		return true
	}
	_, ok := holidaySkips(hd.Year())[hd.Abs()]
	return ok
}

var (
	skipCacheMu sync.Mutex
	skipCache   = map[int]map[int64]bool{}
)

// holidaySkips returns the set of R.D. day numbers in the given Hebrew
// year on which a CHAG or toSkip holiday occurs (Israel schedule).
func holidaySkips(year int) map[int64]bool {
	skipCacheMu.Lock()
	defer skipCacheMu.Unlock()
	if set, ok := skipCache[year]; ok {
		return set
	}
	set := map[int64]bool{}
	for _, ev := range hebcal.GetHolidaysForYear(year, true) {
		if ev.Flags&event.CHAG != 0 || toSkip[ev.Desc] {
			set[ev.Date.Abs()] = true
		}
	}
	skipCache[year] = set
	return set
}

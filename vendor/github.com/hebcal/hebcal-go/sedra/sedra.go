// Hebcal's sedra package calculates the weekly Torah reading
// (Parashat HaShavua).
package sedra

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
	"fmt"
	"strconv"
	"strings"
	"time"

	"github.com/hebcal/hdate"
	"github.com/hebcal/locales"
)

type yearType int

const (
	incomplete yearType = 1 + iota
	regular
	complete
)

// The 54 parshiyot of the Torah as transilterated strings
// parshiot[0] == 'Bereshit', parshiot[1] == 'Noach', parshiot[53] == "Ha'azinu".
var parshiot = []string{
	"Bereshit",
	"Noach",
	"Lech-Lecha",
	"Vayera",
	"Chayei Sara",
	"Toldot",
	"Vayetzei",
	"Vayishlach",
	"Vayeshev",
	"Miketz",
	"Vayigash",
	"Vayechi",
	"Shemot",
	"Vaera",
	"Bo",
	"Beshalach",
	"Yitro",
	"Mishpatim",
	"Terumah",
	"Tetzaveh",
	"Ki Tisa",
	"Vayakhel",
	"Pekudei",
	"Vayikra",
	"Tzav",
	"Shmini",
	"Tazria",
	"Metzora",
	"Achrei Mot",
	"Kedoshim",
	"Emor",
	"Behar",
	"Bechukotai",
	"Bamidbar",
	"Nasso",
	"Beha'alotcha",
	"Sh'lach",
	"Korach",
	"Chukat",
	"Balak",
	"Pinchas",
	"Matot",
	"Masei",
	"Devarim",
	"Vaetchanan",
	"Eikev",
	"Re'eh",
	"Shoftim",
	"Ki Teitzei",
	"Ki Tavo",
	"Nitzavim",
	"Vayeilech",
	"Ha'azinu",
}

// doubled encodes parsha index n as a doubled (combined) reading.
// Doubled readings are stored as negative indices in the sedra arrays.
func doubled(n int) int {
	return -n
}

// undoubled recovers the index of the first parsha of a doubled reading.
func undoubled(n int) int {
	return -n
}

func isValidDouble(n int) bool {
	switch n {
	case -21, -26, -28, -31, -38, -41, -50:
		return true
	default:
		return false
	}
}

/*
 * These indices were originally included in the emacs 19 distribution.
 * These arrays determine the correct indices into the parsha names
 * -1 means no parsha that week.
 */
var satShort = []int{
	-1, 52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
	17, 18, 19, 20, doubled(21), 23, 24, -1, 25, doubled(26), doubled(28), 30, doubled(31), 33, 34, 35, 36, 37, 38, 39, 40, doubled(41), 43, 44, 45, 46, 47,
	48, 49, 50}

var satLong = []int{
	-1, 52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
	17, 18, 19, 20, doubled(21), 23, 24, -1, 25, doubled(26), doubled(28), 30, doubled(31), 33, 34, 35, 36, 37, 38, 39, 40, doubled(41), 43, 44, 45, 46, 47,
	48, 49, doubled(50)}

var monShort = []int{
	51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
	18, 19, 20, doubled(21), 23, 24, -1, 25, doubled(26), doubled(28), 30, doubled(31), 33, 34, 35, 36, 37, 38, 39, 40, doubled(41), 43, 44, 45, 46, 47, 48,
	49, doubled(50)}

var monLong = []int{
	51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, doubled(21), 23, 24, -1, 25, doubled(26), doubled(28),
	30, doubled(31), 33, -1, 34, 35, 36, 37, doubled(38), 40, doubled(41), 43, 44, 45, 46, 47, 48, 49, doubled(50)}

var thuNormal = []int{
	52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
	18, 19, 20, doubled(21), 23, 24, -1, -1, 25, doubled(26), doubled(28), 30, doubled(31), 33, 34, 35, 36, 37, 38, 39, 40, doubled(41), 43, 44, 45, 46, 47,
	48, 49, 50}
var thuNormalIsrael = []int{
	52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	16, 17, 18, 19, 20, doubled(21), 23, 24, -1, 25, doubled(26), doubled(28), 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, doubled(41), 43, 44, 45,
	46, 47, 48, 49, 50}

var thuLong = []int{
	52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
	18, 19, 20, 21, 22, 23, 24, -1, 25, doubled(26), doubled(28), 30, doubled(31), 33, 34, 35, 36, 37, 38, 39, 40, doubled(41), 43, 44, 45, 46, 47,
	48, 49, 50}

var satShortLeap = []int{
	-1, 52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, doubled(41),
	43, 44, 45, 46, 47, 48, 49, doubled(50)}

var satLongLeap = []int{
	-1, 52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32, 33, -1, 34, 35, 36, 37, doubled(38), 40, doubled(41),
	43, 44, 45, 46, 47, 48, 49, doubled(50)}

var monShortLeap = []int{
	51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
	17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32, 33, -1, 34, 35, 36, 37, doubled(38), 40, doubled(41), 43,
	44, 45, 46, 47, 48, 49, doubled(50)}

var monShortLeapIsrael = []int{
	51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	doubled(41), 43, 44, 45, 46, 47, 48, 49, doubled(50)}

var monLongLeap = []int{
	51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
	17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, -1, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, doubled(41),
	43, 44, 45, 46, 47, 48, 49, 50}

var monLongLeapIsrael = []int{
	51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50}

var thuShortLeap = []int{
	52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
	17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, -1, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
	43, 44, 45, 46, 47, 48, 49, 50}

var thuLongLeap = []int{
	52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
	17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, -1, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
	43, 44, 45, 46, 47, 48, 49, doubled(50)}

// schedule picks between the Israel and Diaspora reading schedules.
func schedule(il bool, ilArray, chulArray []int) []int {
	if il {
		return ilArray
	}
	return chulArray
}

func getSedraArray(leap bool, rhDay time.Weekday, ytype yearType, il bool) []int {
	if leap {
		return leapSedraArray(rhDay, ytype, il)
	}
	return commonSedraArray(rhDay, ytype, il)
}

func commonSedraArray(rhDay time.Weekday, ytype yearType, il bool) []int {
	switch rhDay {
	case time.Saturday:
		switch ytype {
		case incomplete:
			return satShort
		case complete:
			return satLong
		}
	case time.Monday:
		switch ytype {
		case incomplete:
			return monShort
		case complete:
			return schedule(il, monShort, monLong)
		}
	case time.Tuesday:
		if ytype == regular {
			return schedule(il, monShort, monLong)
		}
	case time.Thursday:
		switch ytype {
		case regular:
			return schedule(il, thuNormalIsrael, thuNormal)
		case complete:
			return thuLong
		}
	}
	panic("improper sedra year type calculated")
}

func leapSedraArray(rhDay time.Weekday, ytype yearType, il bool) []int {
	switch rhDay {
	case time.Saturday:
		switch ytype {
		case incomplete:
			return satShortLeap
		case complete:
			return schedule(il, satShortLeap, satLongLeap)
		}
	case time.Monday:
		switch ytype {
		case incomplete:
			return schedule(il, monShortLeapIsrael, monShortLeap)
		case complete:
			return schedule(il, monLongLeapIsrael, monLongLeap)
		}
	case time.Tuesday:
		if ytype == regular {
			return schedule(il, monLongLeapIsrael, monLongLeap)
		}
	case time.Thursday:
		switch ytype {
		case incomplete:
			return thuShortLeap
		case complete:
			return thuLongLeap
		}
	}
	panic("improper sedra year type calculated")
}

// Sedra is the schedule of weekly Torah readings for an entire Hebrew year.
// Construct one with New.
type Sedra struct {
	Year          int   // Hebrew year
	IL            bool  // Israel flag (true for Israel schedule, false for Diaspora)
	firstSaturday int64 // R.D. date of the first Shabbat of the year
	theSedraArray []int // read-only array of parshiot read this year
}

// Parsha represents a Weekly Torah Reading (Parashat haShavua).
type Parsha struct {
	// Name represents the name of the parsha (or parshiyot) read on
	// Hebrew date, e.g. {"Noach"} or {"Matot", "Masei"}
	Name []string

	// Num is the parsha number (or numbers) using 1-indexing. The slice contains
	// one number for a regular (single) parsha, and two numbers for a doubled parsha.
	// For Parashat Bereshit, Num would be equal to {1}, and for
	// Matot-Masei it would be {42, 43}
	Num []int

	// Chag is true if this is a regular parasha HaShavua Torah reading,
	// false if it's a special holiday reading
	Chag bool
}

// New constructs a Sedra for the entire Hebrew year.
func New(year int, il bool) Sedra {
	longC := hdate.LongCheshvan(year)
	shortK := hdate.ShortKislev(year)
	var ytype yearType
	if longC && !shortK {
		ytype = complete
	} else if !longC && shortK {
		ytype = incomplete
	} else {
		ytype = regular
	}
	rh := hdate.New(year, hdate.Tishrei, 1)
	rhDay := rh.Weekday()
	leap := hdate.IsLeapYear(year)
	firstSaturday := hdate.DayOnOrBefore(time.Saturday, rh.Abs()+6)
	theSedraArray := getSedraArray(leap, rhDay, ytype, il)
	return Sedra{Year: year, IL: il, firstSaturday: firstSaturday, theSedraArray: theSedraArray}
}

// LookupByRD returns the Parsha that's read on the Saturday on or after the
// R.D. date.
func (s *Sedra) LookupByRD(rataDie int64) Parsha {
	sedraNumWeeks := len(s.theSedraArray)
	if sedraNumWeeks == 0 {
		panic("invalid Sedra state, did you forget sedra.New()?")
	}
	abs := hdate.DayOnOrBefore(time.Saturday, rataDie+6)
	if abs < s.firstSaturday {
		panic("lookup date " + strconv.FormatInt(abs, 10) +
			" is earlier than start of year " +
			strconv.FormatInt(s.firstSaturday, 10))
	}
	var idx int
	weekNum := int((abs - s.firstSaturday) / 7)
	if weekNum >= sedraNumWeeks {
		indexLast := s.theSedraArray[sedraNumWeeks-1]
		if indexLast < 0 {
			/* advance 2 parashiyot ahead after a doubled week */
			idx = undoubled(indexLast) + 2
		} else {
			idx = indexLast + 1
		}
	} else {
		idx = s.theSedraArray[weekNum]
	}
	switch {
	case idx >= 0:
		return Parsha{Name: []string{parshiot[idx]}, Num: []int{idx + 1}, Chag: false}
	case idx == -1:
		return Parsha{Chag: true}
	default:
		p1 := undoubled(idx)
		p2 := p1 + 1
		return Parsha{
			Name: []string{parshiot[p1], parshiot[p2]},
			Num:  []int{p1 + 1, p2 + 1},
			Chag: false,
		}
	}
}

// Lookup returns the Parsha that is read on the Saturday on or after hd.
func (s *Sedra) Lookup(hd hdate.HDate) Parsha {
	return s.LookupByRD(hd.Abs())
}

// FindParshaNum finds the date on which the parsha number (Bereshit=1) is read.
//
// It returns an error if num is not a valid parsha number, or if that parsha is
// not read on its own in this year. The latter happens whenever the parsha is
// part of a doubled reading: in a year that reads Matot-Masei together, neither
// Matot nor Masei can be found on its own.
func (s *Sedra) FindParshaNum(num int) (hdate.HDate, error) {
	parshaNum := num - 1
	if parshaNum > 53 || (parshaNum < 0 && !isValidDouble(parshaNum)) {
		return hdate.HDate{}, fmt.Errorf("invalid parsha number %d", num)
	}
	for idx, candidate := range s.theSedraArray {
		if candidate == parshaNum {
			return hdate.FromRD(s.firstSaturday + int64(idx*7)), nil
		}
	}
	return hdate.HDate{}, fmt.Errorf("parsha number %d is not read in year %d", num, s.Year)
}

// String returns a string representation of the parsha.
func (p Parsha) String() string {
	return "Parashat " + strings.Join(p.Name, "-")
}

// Render returns the localized string name of the parsha.
func (p Parsha) Render(locale string) string {
	locale = strings.ToLower(locale)
	var sb strings.Builder
	sb.Grow(32)
	for i, enName := range p.Name {
		if i != 0 {
			if locale == "he" || locale == "he-x-nonikud" {
				sb.WriteRune('־')
			} else {
				sb.WriteRune('-')
			}
		}
		name, _ := locales.LookupTranslation(enName, locale)
		sb.WriteString(name)
	}
	return sb.String()
}

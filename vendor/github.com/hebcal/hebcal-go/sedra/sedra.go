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
	"errors"
	"strconv"
	"strings"
	"time"

	"github.com/hebcal/hdate"
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

/* parsha doubler */
func _d(n int) int {
	return -n
}

/* parsha undoubler */
func _u(n int) int {
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
var sat_short = []int{
	-1, 52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
	17, 18, 19, 20, _d(21), 23, 24, -1, 25, _d(26), _d(28), 30, _d(31), 33, 34, 35, 36, 37, 38, 39, 40, _d(41), 43, 44, 45, 46, 47,
	48, 49, 50}

var sat_long = []int{
	-1, 52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
	17, 18, 19, 20, _d(21), 23, 24, -1, 25, _d(26), _d(28), 30, _d(31), 33, 34, 35, 36, 37, 38, 39, 40, _d(41), 43, 44, 45, 46, 47,
	48, 49, _d(50)}

var mon_short = []int{
	51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
	18, 19, 20, _d(21), 23, 24, -1, 25, _d(26), _d(28), 30, _d(31), 33, 34, 35, 36, 37, 38, 39, 40, _d(41), 43, 44, 45, 46, 47, 48,
	49, _d(50)}

var mon_long = []int{
	51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, _d(21), 23, 24, -1, 25, _d(26), _d(28),
	30, _d(31), 33, -1, 34, 35, 36, 37, _d(38), 40, _d(41), 43, 44, 45, 46, 47, 48, 49, _d(50)}

var thu_normal = []int{
	52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
	18, 19, 20, _d(21), 23, 24, -1, -1, 25, _d(26), _d(28), 30, _d(31), 33, 34, 35, 36, 37, 38, 39, 40, _d(41), 43, 44, 45, 46, 47,
	48, 49, 50}
var thu_normal_Israel = []int{
	52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	16, 17, 18, 19, 20, _d(21), 23, 24, -1, 25, _d(26), _d(28), 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, _d(41), 43, 44, 45,
	46, 47, 48, 49, 50}

var thu_long = []int{
	52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
	18, 19, 20, 21, 22, 23, 24, -1, 25, _d(26), _d(28), 30, _d(31), 33, 34, 35, 36, 37, 38, 39, 40, _d(41), 43, 44, 45, 46, 47,
	48, 49, 50}

var sat_short_leap = []int{
	-1, 52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, _d(41),
	43, 44, 45, 46, 47, 48, 49, _d(50)}

var sat_long_leap = []int{
	-1, 52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32, 33, -1, 34, 35, 36, 37, _d(38), 40, _d(41),
	43, 44, 45, 46, 47, 48, 49, _d(50)}

var mon_short_leap = []int{
	51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
	17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32, 33, -1, 34, 35, 36, 37, _d(38), 40, _d(41), 43,
	44, 45, 46, 47, 48, 49, _d(50)}

var mon_short_leap_Israel = []int{
	51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	_d(41), 43, 44, 45, 46, 47, 48, 49, _d(50)}

var mon_long_leap = []int{
	51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
	17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, -1, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, _d(41),
	43, 44, 45, 46, 47, 48, 49, 50}

var mon_long_leap_Israel = []int{
	51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50}

var thu_short_leap = []int{
	52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
	17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, -1, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
	43, 44, 45, 46, 47, 48, 49, 50}

var thu_long_leap = []int{
	52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
	17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, -1, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
	43, 44, 45, 46, 47, 48, 49, _d(50)}

func getSedraArray(leap bool, rhDay time.Weekday, ytype yearType, il bool) []int {
	if !leap {
		switch rhDay {
		case time.Saturday:
			if ytype == incomplete {
				return sat_short
			} else if ytype == complete {
				return sat_long
			}
		case time.Monday:
			if ytype == incomplete {
				return mon_short
			} else if ytype == complete {
				if il {
					return mon_short
				} else {
					return mon_long
				}
			}
		case time.Tuesday:
			if ytype == regular {
				if il {
					return mon_short
				} else {
					return mon_long
				}
			}
		case time.Thursday:
			if ytype == regular {
				if il {
					return thu_normal_Israel
				} else {
					return thu_normal
				}
			} else if ytype == complete {
				return thu_long
			}
		}
	} else {
		/* leap year */
		switch rhDay {
		case time.Saturday:
			if ytype == incomplete {
				return sat_short_leap
			} else if ytype == complete {
				if il {
					return sat_short_leap
				} else {
					return sat_long_leap
				}
			}
		case time.Monday:
			if ytype == incomplete {
				if il {
					return mon_short_leap_Israel
				} else {
					return mon_short_leap
				}
			} else if ytype == complete {
				if il {
					return mon_long_leap_Israel
				} else {
					return mon_long_leap
				}
			}
		case time.Tuesday:
			if ytype == regular {
				if il {
					return mon_long_leap_Israel
				} else {
					return mon_long_leap
				}
			}
		case time.Thursday:
			if ytype == incomplete {
				return thu_short_leap
			} else if ytype == complete {
				return thu_long_leap
			}
		}
	}
	panic("improper sedra year type calculated")
}

type Sedra struct {
	Year          int   // Hebrew year
	IL            bool  // Israel flag (true for Israel schedule, false for Diaspora)
	firstSaturday int64 // R.D. date of the first Shabbat of the year
	theSedraArray []int // read-only array of parshiot read this year
}

// A Parsha object represents a Weekly Torah Reading (Parashat haShavua)
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

// Constructs a new Sedra for the entire Hebrew year.
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

// Returns the Parsha that's read on the Saturday on or after R.D. date.
func (sedra *Sedra) LookupByRD(rataDie int64) Parsha {
	sedraNumWeeks := len(sedra.theSedraArray)
	if sedraNumWeeks == 0 {
		panic("invalid Sedra state, did you forget sedra.New()?")
	}
	abs := hdate.DayOnOrBefore(time.Saturday, rataDie+6)
	if abs < sedra.firstSaturday {
		panic("lookup date " + strconv.FormatInt(abs, 10) +
			" is earlier than start of year " +
			strconv.FormatInt(sedra.firstSaturday, 10))
	}
	var idx int
	weekNum := int((abs - sedra.firstSaturday) / 7)
	if weekNum >= sedraNumWeeks {
		indexLast := sedra.theSedraArray[sedraNumWeeks-1]
		if indexLast < 0 {
			/* advance 2 parashiyot ahead after a doubled week */
			idx = _u(indexLast) + 2
		} else {
			idx = indexLast + 1
		}
	} else {
		idx = sedra.theSedraArray[weekNum]
	}
	if idx >= 0 {
		name := parshiot[idx]
		return Parsha{Name: []string{name}, Num: []int{idx + 1}, Chag: false}
	} else if idx == -1 {
		return Parsha{Chag: true}
	} else {
		// undouble
		p1 := _u(idx)
		p2 := p1 + 1
		n1 := parshiot[p1]
		n2 := parshiot[p2]
		return Parsha{Name: []string{n1, n2}, Num: []int{p1 + 1, p2 + 1}, Chag: false}
	}
}

// Returns the Parsha that is read on the Saturday on or after hd.
func (sedra *Sedra) Lookup(hd hdate.HDate) Parsha {
	return sedra.LookupByRD(hd.Abs())
}

// Finds the date on which the parsha number (Bereshit=1) is read.
func (sedra *Sedra) FindParshaNum(num int) (hdate.HDate, error) {
	parshaNum := num - 1
	if parshaNum > 53 || (parshaNum < 0 && !isValidDouble(parshaNum)) {
		return hdate.HDate{}, errors.New("invalid parsha number " + strconv.Itoa(num))
	}
	for idx, candidate := range sedra.theSedraArray {
		if candidate == parshaNum {
			return hdate.FromRD(sedra.firstSaturday + int64(idx*7)), nil
		}
	}
	panic("not found parsha num " + strconv.Itoa(num))
}

// Returns a string representation of the parsha
func (parsha Parsha) String() string {
	return "Parashat " + strings.Join(parsha.Name, "-")
}

// Hebcal's pirkeiavot package calculates the Pirkei Avot ("Ethics of the
// Fathers") summer schedule: one chapter studied on each Shabbat between
// Pesach and Rosh Hashana, the six-chapter cycle repeating with doubled
// chapters at the end to fill the season.
//
// https://en.wikipedia.org/wiki/Pirkei_Avot#Study_of_the_work
package pirkeiavot

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
	"time"

	"github.com/hebcal/hdate"
)

// New returns the Pirkei Avot chapter(s) studied on the given Shabbat.
//
// The il argument selects the Israel schedule (no 8th day Pesach, no 2nd
// day Shavuot) when true, or the Diaspora schedule when false.
//
// The second return value is false when the date is not a Pirkei Avot
// Shabbat: not a Saturday, before Pesach, after Rosh Hashana, or a
// Shabbat coinciding with a holiday on which Pirkei Avot is skipped (8th
// day Pesach / 2nd day Shavuot in the Diaspora; Erev Tish'a B'Av or
// Tish'a B'Av).
func New(hd hdate.HDate, il bool) ([]int, bool) {
	if hd.Weekday() != time.Saturday {
		return nil, false
	}
	hyear := hd.Year()
	pesach7 := hdate.New(hyear, hdate.Nisan, 21)
	if hd.Abs() <= pesach7.Abs() {
		return nil, false // before Pesach
	}
	first := pesach7.After(time.Saturday)
	weekDiff := ceilDiv(hd.Abs()-first.Abs(), 7)

	// When a holiday falls on Shabbat, Pirkei Avot is not studied; the same
	// applies to Erev Tish'a B'Av.
	var holidays []hdate.HDate
	if !il {
		// 8th day Pesach and 2nd day Shavuot are observed only in the Diaspora.
		holidays = append(holidays, pesach7.Next(), hdate.New(hyear, hdate.Sivan, 7))
	}
	av8 := hdate.New(hyear, hdate.Av, 8)
	holidays = append(holidays, av8, av8.Next()) // Erev 9 Av and Tish'a B'Av

	for _, day := range holidays {
		if day.Abs() == hd.Abs() {
			return nil, false
		}
		if day.Abs()-hd.Abs() <= 0 && day.Weekday() == time.Saturday {
			weekDiff-- // a holiday Shabbat already passed; don't count that week
		}
	}

	if weekDiff < 0 {
		return nil, false
	} else if weekDiff < 18 {
		return []int{int(weekDiff%6) + 1}, true
	}

	// The final round doubles up chapters to reach Rosh Hashana.
	rh := hdate.New(hyear+1, hdate.Tishrei, 1)
	last := rh.Before(time.Saturday)
	weeksRemain := ceilDiv(last.Abs()-hd.Abs(), 7)
	switch weeksRemain {
	case 0:
		return []int{5, 6}, true
	case 1:
		return []int{3, 4}, true
	case 2:
		if weekDiff%6 == 1 {
			return []int{2}, true
		}
		return []int{1, 2}, true
	case 3:
		return []int{1}, true
	}
	return nil, false
}

// ceilDiv returns ceil(a/b) for b > 0, matching JavaScript Math.ceil.
func ceilDiv(a, b int64) int64 {
	if a >= 0 {
		return (a + b - 1) / b
	}
	return -((-a) / b)
}

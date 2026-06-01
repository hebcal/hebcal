// Hebcal's rambam package calculates the Daily Rambam (Mishneh Torah)
// learning schedule in both the 1-chapter-a-day and 3-chapters-a-day
// cycles instituted by the Lubavitcher Rebbe in 1984.
//
// https://en.wikipedia.org/wiki/Daily_Rambam_Study
package rambam

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
	"strings"
	"time"

	"github.com/hebcal/greg"
	"github.com/hebcal/hdate"
)

// rambam1CycleLen is the number of days in the 1-chapter-a-day cycle.
const rambam1CycleLen = 1017

// rambam3CycleLen is the number of days in the 3-chapters-a-day cycle.
const rambam3CycleLen = rambam1CycleLen / 3

// Rambam1Start is the R.D. number of the start of the Daily Rambam cycle,
// Sunday 27 Nisan 5744 (29 April 1984). Both cycles share this start date.
var Rambam1Start = greg.ToRD(1984, time.April, 29)

// first4verses holds the mitzvot ranges for the first four (introductory)
// sections of the Mishneh Torah, which cover spans of mitzvot rather than
// numbered chapters.
var first4verses = [4][3]string{
	{"1-21", "22-33", "34-45"},           // Transmission of the Oral Law
	{"1-83", "84-166", "167-248"},        // Positive Mitzvot
	{"1-122", "123-245", "246-365"},      // Negative Mitzvot
	{"1:1-4:8", "5:1-9:9", "10:1-14:10"}, // Overview of Mishneh Torah Contents
}

// mishnehTorah3 is the section table for the 3-chapters-a-day cycle. It
// differs from mishnehTorah1 in two places to keep the three daily
// chapters logically grouped.
var mishnehTorah3 = makeMishnehTorah3()

func makeMishnehTorah3() []section {
	s := make([]section, len(mishnehTorah1))
	copy(s, mishnehTorah1)
	// "The Order of Prayer" gets an extra day in the 3-day cycle (4 => 5).
	s[15].ch = 5
	// The final 2 chapters of "Leavened and Unleavened Bread" are combined
	// on a single day in the 3-day cycle (9 => 8).
	s[20].ch = 8
	return s
}

// Reading is one section/chapter of the Mishneh Torah as scheduled by the
// Daily Rambam cycle.
type Reading struct {
	// Name is the English transliteration of the section (Halacha), e.g.
	// "Foundations of the Torah", "Sabbath", "Kings and Wars".
	Name string
	// Perek is the chapter (perek) reference: usually a number, but may be
	// a range like "4-5" when chapters are combined, or a mitzvot range
	// like "1-21" / "1:1-4:8" for the introductory sections.
	Perek string
}

// String returns the untranslated description, e.g. "Kings and Wars 4".
func (r Reading) String() string {
	return r.Name + " " + r.Perek
}

// getChap returns the reading at index idx (0-based) within the given
// section table.
func getChap(idx int, mishnehTorah []section) Reading {
	total := idx
	for j := 0; j < len(mishnehTorah); j++ {
		if total < mishnehTorah[j].ch {
			chapNum := total + 1
			var perek string
			if j < 4 {
				perek = first4verses[j][chapNum-1]
			} else {
				perek = strconv.Itoa(chapNum)
			}
			return Reading{Name: mishnehTorah[j].name, Perek: perek}
		}
		total -= mishnehTorah[j].ch
	}
	panic("internal error: rambam index out of range")
}

// New1 calculates the Daily Rambam reading for the 1-chapter-a-day cycle.
//
// The second return value is false when the date is before the cycle
// began (29 April 1984).
func New1(hd hdate.HDate) (Reading, bool) {
	cday := hd.Abs()
	if cday < Rambam1Start {
		return Reading{}, false
	}
	dno := int((cday - Rambam1Start) % rambam1CycleLen)
	reading := getChap(dno, mishnehTorah1)
	if reading.Name == "The Order of Prayer" && reading.Perek == "4" {
		reading.Perek = "4-5"
	}
	return reading, true
}

// New3 calculates the Daily Rambam readings for the 3-chapters-a-day
// cycle, returning the three chapters studied that day in order.
//
// The second return value is false when the date is before the cycle
// began (29 April 1984).
func New3(hd hdate.HDate) ([]Reading, bool) {
	cday := hd.Abs()
	if cday < Rambam1Start {
		return nil, false
	}
	dno := int((cday - Rambam1Start) % rambam3CycleLen)
	idx := dno * 3
	r1 := getChap(idx, mishnehTorah3)
	if r1.Name == "Leavened and Unleavened Bread" && r1.Perek == "8" {
		r1.Perek = "8-9"
	}
	r2 := getChap(idx+1, mishnehTorah3)
	r3 := getChap(idx+2, mishnehTorah3)
	return []Reading{r1, r2, r3}, true
}

// combinePair combines two readings from the same section into a single
// reading spanning from the first chapter/range to the last.
func combinePair(r1, r2 Reading) Reading {
	start := r1.Perek
	if i := strings.IndexByte(start, '-'); i >= 0 {
		start = start[:i]
	}
	end := r2.Perek
	if i := strings.LastIndexByte(end, '-'); i >= 0 {
		end = end[i+1:]
	}
	return Reading{Name: r1.Name, Perek: start + "-" + end}
}

// collapseAdjacent merges adjacent readings that fall in the same section
// of the Mishneh Torah, for display in the 3-chapter cycle.
func collapseAdjacent(r []Reading) []Reading {
	if len(r) != 3 {
		return r
	}
	if r[0].Name == r[1].Name && r[1].Name == r[2].Name {
		return []Reading{combinePair(r[0], r[2])}
	}
	if r[0].Name == r[1].Name {
		return []Reading{combinePair(r[0], r[1]), r[2]}
	}
	if r[1].Name == r[2].Name {
		return []Reading{r[0], combinePair(r[1], r[2])}
	}
	return r
}

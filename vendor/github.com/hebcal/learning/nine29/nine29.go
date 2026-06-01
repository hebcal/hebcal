// Hebcal's nine29 package calculates the 929 (Tanakh B'Yachad) daily
// learning schedule: one chapter of the Hebrew Bible per day, Sunday
// through Thursday (skipping Friday and Saturday), covering all 929
// chapters in about 3.5 years.
//
// https://www.929.org.il
package nine29

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
	"time"

	"github.com/hebcal/greg"
	"github.com/hebcal/hdate"
)

// Total929Chapters is the number of chapters in the Hebrew Bible (Tanakh).
const Total929Chapters = 929

// Nine29Start is the R.D. number of the start of the first 929 cycle,
// Sunday 21 December 2014.
var Nine29Start = greg.ToRD(2014, time.December, 21)

// Cycle 1 ended Wed 18 April 2018 (Israel's 70th Independence Day). The
// historical schedule had additional skip days so chapter 929 landed
// earlier than the simple Fri/Sat-only formula would predict. Cycle 2
// did not begin until Sunday 15 July 2018 — a ~3-month gap unique to this
// transition; subsequent cycles restart the following Sunday.
var nine29EndCycle1 = greg.ToRD(2018, time.April, 18)
var nine29StartCycle2 = greg.ToRD(2018, time.July, 15)

// Reading is the 929 reading for a single day.
type Reading struct {
	CycleChap int    // Chapter number within the cycle (1-929)
	CycleNum  int    // 1-based cycle number
	Book      string // Book name, e.g. "Genesis" or "Ruth"
	BookChap  int    // 1-based chapter number within the book
}

// skipDay reports whether the given R.D. day is a Friday or Saturday,
// when no 929 chapter is read.
func skipDay(abs int64) bool {
	dow := abs % 7 // 0=Sun .. 5=Fri, 6=Sat
	return dow == 5 || dow == 6
}

// findCycleEnd returns the R.D. day on which chapter 929 is read, given
// the R.D. day (always a Sunday) on which a cycle begins.
func findCycleEnd(cycleStart int64) int64 {
	chaptersRead := 0
	abs := cycleStart
	for {
		if !skipDay(abs) {
			chaptersRead++
			if chaptersRead == Total929Chapters {
				return abs
			}
		}
		abs++
	}
}

// New calculates the 929 reading for the given date.
//
// The second return value is false when there is no reading that day:
// before the program began (21 December 2014), on Fridays and Saturdays,
// and on the wind-down days between the end of one cycle and the start of
// the next.
func New(hd hdate.HDate) (Reading, bool) {
	abs := hd.Abs()
	if abs < Nine29Start {
		return Reading{}, false
	}

	cycleStart := Nine29Start
	cycleNumber := 1
	for {
		cycleEnd := findCycleEnd(cycleStart)
		// Cycle 1->2 has a unique 3-month gap; later transitions are +4 days
		// (Wed last chapter, skip Thu-Sat, next Sunday).
		var nextStart int64
		if cycleNumber == 1 {
			nextStart = nine29StartCycle2
		} else {
			nextStart = cycleEnd + 4
		}

		if abs < nextStart {
			if skipDay(abs) {
				return Reading{}, false
			}
			// Cycle 1 used a modified (holiday-skipping) schedule; cap it at
			// the historical end date rather than the formula-computed end.
			effectiveCycleEnd := cycleEnd
			if cycleNumber == 1 {
				effectiveCycleEnd = nine29EndCycle1
			}
			if abs > effectiveCycleEnd {
				return Reading{}, false
			}
			chaptersBeforeToday := 0
			for i := cycleStart; i < abs; i++ {
				if !skipDay(i) {
					chaptersBeforeToday++
				}
			}
			chapterNum := chaptersBeforeToday + 1
			book, bookChap := chapterToBook(chapterNum)
			return Reading{
				CycleChap: chapterNum,
				CycleNum:  cycleNumber,
				Book:      book,
				BookChap:  bookChap,
			}, true
		}

		cycleStart = nextStart
		cycleNumber++
	}
}

// chapterToBook maps a 1-based cycle chapter number (1-929) to its book
// name and 1-based chapter within that book.
func chapterToBook(chapter int) (string, int) {
	remaining := chapter
	for _, book := range tanakhBooks {
		if remaining <= book.numChap {
			return book.name, remaining
		}
		remaining -= book.numChap
	}
	panic("929 chapter " + strconv.Itoa(chapter) + " out of range")
}

// String returns the untranslated description, e.g. "Genesis 1 (1)".
func (r Reading) String() string {
	return r.Book + " " + strconv.Itoa(r.BookChap) + " (" + strconv.Itoa(r.CycleChap) + ")"
}

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

// lastChapterOffset is the day offset from the start of a cycle (always a
// Sunday) to the day chapter 929 is read.
//
// Reading 5 chapters a week, 929 chapters is 185 full weeks (925 chapters)
// plus 4 more: the 185 weeks span days 0..1294, so chapters 926-929 fall on
// the Sun/Mon/Tue/Wed of the following week, i.e. offsets 1295..1298. That
// makes the last chapter land on offset 1298, always a Wednesday.
const lastChapterOffset = 1298

// cycleDays is the number of days from the start of one cycle to the start
// of the next. The last chapter is a Wednesday, and the next cycle begins the
// following Sunday (+4 days), so cycles repeat on a fixed 1302-day period.
const cycleDays = lastChapterOffset + 4

// chaptersInDays returns the number of chapters read in the days days
// starting at a cycle start (always a Sunday), i.e. ending just before
// cycleStart+days.
//
// Whole weeks contribute 5 chapters each; the partial week contributes one
// per day from Sunday through Thursday, capped at 5 since Fri/Sat are skipped.
func chaptersInDays(days int64) int {
	partial := days % 7
	if partial > 5 {
		partial = 5
	}
	return int(days/7*5 + partial)
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

	// Locate the cycle containing this date directly. Cycle 1->2 has a unique
	// ~3-month gap, so cycle 1 is special-cased; from cycle 2 onward the
	// cycles repeat on a fixed cycleDays period.
	var cycleNumber int
	var cycleStart int64
	if abs < nine29StartCycle2 {
		cycleNumber = 1
		cycleStart = Nine29Start
	} else {
		elapsed := (abs - nine29StartCycle2) / cycleDays
		cycleNumber = 2 + int(elapsed)
		cycleStart = nine29StartCycle2 + elapsed*cycleDays
	}

	if skipDay(abs) {
		return Reading{}, false
	}
	// Cycle 1 used a modified (holiday-skipping) schedule; cap it at the
	// historical end date rather than the formula-computed end.
	effectiveCycleEnd := cycleStart + lastChapterOffset
	if cycleNumber == 1 {
		effectiveCycleEnd = nine29EndCycle1
	}
	if abs > effectiveCycleEnd {
		// Wind-down / historical gap: no reading.
		return Reading{}, false
	}
	chapterNum := chaptersInDays(abs-cycleStart) + 1
	book, bookChap := chapterToBook(chapterNum)
	return Reading{
		CycleChap: chapterNum,
		CycleNum:  cycleNumber,
		Book:      book,
		BookChap:  bookChap,
	}, true
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

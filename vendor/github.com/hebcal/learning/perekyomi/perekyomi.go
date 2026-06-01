// Hebcal's perekyomi package calculates the Perek Yomi, a daily regimen
// of learning one chapter (perek) of the Mishnah each day, completing the
// entire Mishnah in 525 days (~17 months).
package perekyomi

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
	"github.com/hebcal/learning/mishnayomi"
)

// cycleLen is the number of days in the Perek Yomi cycle (total Mishnah
// chapters).
const cycleLen = 525

// PerekYomiStart is the R.D. number of the start of the Perek Yomi cycle,
// Shabbat 9 February 2002 (27 Sh'vat 5762).
var PerekYomiStart = greg.ToRD(2002, time.February, 9)

// tractates is the Mishnah's tractate ordering, shared with the
// mishnayomi package.
var tractates = mishnayomi.Tractates()

// Reading is one chapter of the Mishnah as scheduled by Perek Yomi.
type Reading struct {
	Tractate string // Tractate name, e.g. "Berakhot", "Sotah", "Avot"
	Chap     int    // 1-based chapter number within the tractate
}

// String returns the untranslated description, e.g. "Sotah 8".
func (r Reading) String() string {
	return r.Tractate + " " + strconv.Itoa(r.Chap)
}

// New calculates the Perek Yomi reading for the given date. The cycle has
// no skip days.
//
// The second return value is false when the date is before the cycle
// began (9 February 2002).
func New(hd hdate.HDate) (Reading, bool) {
	abs := hd.Abs()
	if abs < PerekYomiStart {
		return Reading{}, false
	}
	total := int((abs - PerekYomiStart) % cycleLen)
	for _, t := range tractates {
		if total < t.NumChaps {
			return Reading{Tractate: t.Name, Chap: total + 1}, true
		}
		total -= t.NumChaps
	}
	panic("internal error: perek yomi index out of range")
}

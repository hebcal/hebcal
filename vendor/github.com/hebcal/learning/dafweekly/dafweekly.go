// Hebcal's dafweekly package calculates the Daf-a-Week schedule, a
// slower regimen of learning the Babylonian Talmud in which the same
// page (daf) is studied for an entire week. One full cycle takes about
// 52 years.
package dafweekly

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

	"github.com/hebcal/greg"
	"github.com/hebcal/hdate"
	"github.com/hebcal/learning/dafyomi"
)

// 2711 dapim in the Babylonian Talmud cycle, each studied for 7 days.
const numDays = 2711 * 7

// DafWeeklyStart is the R.D. number of the start of the Daf-a-Week cycle,
// Sunday 6 March 2005 (25 Adar I 5765).
var DafWeeklyStart = greg.ToRD(2005, time.March, 6)

// New calculates the Daf-a-Week page for the given date. The same daf is
// returned for every day of a given week.
//
// The second return value is false when the date is before the cycle
// began (6 March 2005).
func New(hd hdate.HDate) (dafyomi.Daf, bool) {
	abs := hd.Abs()
	if abs < DafWeeklyStart {
		return dafyomi.Daf{}, false
	}
	dayNum := int((abs - DafWeeklyStart) % numDays)
	weekNum := dayNum / 7
	return dafyomi.FindDaf(weekNum), true
}

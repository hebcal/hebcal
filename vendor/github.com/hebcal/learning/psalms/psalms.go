// Hebcal's psalms package calculates the Daily Psalms (Tehillim) reading
// in the traditional 30-day cycle, indexed by Hebrew day of the month so
// that the entire book of Psalms is completed each Hebrew month.
package psalms

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
	"github.com/hebcal/hdate"
)

// Reading is one day's slice of the 30-day Psalms cycle. Begin and End
// are normally chapter numbers (e.g. "10", "17"), but Psalm 119 — the
// longest chapter, studied over two days — uses verse-prefixed strings
// ("119:1"/"119:96" and "119:97"/"119:176").
type Reading struct {
	Begin string
	End   string
}

// String returns the reading as "Begin-End", e.g. "140-150".
func (r Reading) String() string {
	return r.Begin + "-" + r.End
}

// schedule is indexed by Hebrew day of the month (1-30); index 0 is unused.
var schedule = [31]Reading{
	{"0", "0"},
	{"1", "9"},
	{"10", "17"},
	{"18", "22"},
	{"23", "28"},
	{"29", "34"},
	{"35", "38"},
	{"39", "43"},
	{"44", "48"},
	{"49", "54"},
	{"55", "59"},
	{"60", "65"},
	{"66", "68"},
	{"69", "71"},
	{"72", "76"},
	{"77", "78"},
	{"79", "82"},
	{"83", "87"},
	{"88", "89"},
	{"90", "96"},
	{"97", "103"},
	{"104", "105"},
	{"106", "107"},
	{"108", "112"},
	{"113", "118"},
	{"119:1", "119:96"},
	{"119:97", "119:176"},
	{"120", "134"},
	{"135", "139"},
	{"140", "144"},
	{"145", "150"},
}

// New returns the Daily Psalms portion for the given date. In months with
// only 29 days, the 30th portion is combined with the 29th (returning
// 140-150). Every Hebrew date has a reading; there is no cycle start.
func New(hd hdate.HDate) Reading {
	dd := hd.Day()
	if dd == 29 && hdate.DaysInMonth(hd.Month(), hd.Year()) == 29 {
		return Reading{Begin: "140", End: "150"} // read both 29th and 30th
	}
	return schedule[dd]
}

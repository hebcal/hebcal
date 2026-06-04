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
	"strconv"
	"strings"

	"github.com/hebcal/gematriya"
	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/event"
	"github.com/hebcal/locales"
	"github.com/hebcal/learning/dafyomi"
)

type dafWeeklyEvent struct {
	Date hdate.HDate
	Daf  dafyomi.Daf
}

// NewDafWeeklyEvent returns a calendar event for the Daf-a-Week page on
// the given date.
func NewDafWeeklyEvent(hd hdate.HDate, daf dafyomi.Daf) event.CalEvent {
	return dafWeeklyEvent{Date: hd, Daf: daf}
}

func (ev dafWeeklyEvent) GetDate() hdate.HDate {
	return ev.Date
}

func (ev dafWeeklyEvent) Render(locale string) string {
	name, _ := locales.LookupTranslation(ev.Daf.Name, locale)
	if l := strings.ToLower(locale); l == "he" || l == "he-x-nonikud" {
		return name + " דף " + gematriya.Gematriya(ev.Daf.Blatt)
	}
	return name + " " + strconv.Itoa(ev.Daf.Blatt)
}

func (ev dafWeeklyEvent) GetFlags() event.HolidayFlags {
	return event.DAILY_LEARNING
}

func (ev dafWeeklyEvent) GetEmoji() string {
	return ""
}

func (ev dafWeeklyEvent) Basename() string {
	return ev.Daf.String()
}

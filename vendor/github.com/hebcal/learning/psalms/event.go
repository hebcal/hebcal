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
	"strconv"
	"strings"

	"github.com/hebcal/gematriya"
	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/event"
	"github.com/hebcal/locales"
)

type psalmsEvent struct {
	Date    hdate.HDate
	Reading Reading
}

// NewPsalmsEvent returns a calendar event for the Daily Psalms reading on
// the given date.
func NewPsalmsEvent(hd hdate.HDate, reading Reading) event.CalEvent {
	return psalmsEvent{Date: hd, Reading: reading}
}

func (ev psalmsEvent) GetDate() hdate.HDate {
	return ev.Date
}

func (ev psalmsEvent) Render(locale string) string {
	book, _ := locales.LookupTranslation("Psalms", locale)
	r := ev.Reading
	if l := strings.ToLower(locale); l == "he" || l == "he-x-nonikud" {
		if b, err := strconv.Atoi(r.Begin); err == nil {
			if e, err := strconv.Atoi(r.End); err == nil {
				return book + " " + gematriya.Gematriya(b) + "-" + gematriya.Gematriya(e)
			}
		}
	}
	return book + " " + r.Begin + "-" + r.End
}

func (ev psalmsEvent) GetFlags() event.HolidayFlags {
	return event.DAILY_LEARNING
}

func (ev psalmsEvent) GetEmoji() string {
	return ""
}

func (ev psalmsEvent) Basename() string {
	return "Psalms " + ev.Reading.String()
}

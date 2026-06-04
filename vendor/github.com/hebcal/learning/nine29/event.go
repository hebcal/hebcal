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
	"strings"

	"github.com/hebcal/gematriya"
	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/event"
	"github.com/hebcal/locales"
)

type nine29Event struct {
	Date    hdate.HDate
	Reading Reading
}

// NewNine29Event returns a calendar event for the 929 reading on the
// given date.
func NewNine29Event(hd hdate.HDate, reading Reading) event.CalEvent {
	return nine29Event{Date: hd, Reading: reading}
}

func (ev nine29Event) GetDate() hdate.HDate {
	return ev.Date
}

func (ev nine29Event) Render(locale string) string {
	r := ev.Reading
	cycleChap := " (" + strconv.Itoa(r.CycleChap) + ")"
	if isHebrewLocale(locale) {
		name, _ := locales.LookupTranslation(r.Book, locale)
		return name + " " + gematriya.Gematriya(r.BookChap) + cycleChap
	}
	return r.Book + " " + strconv.Itoa(r.BookChap) + cycleChap
}

func (ev nine29Event) GetFlags() event.HolidayFlags {
	return event.DAILY_LEARNING
}

func (ev nine29Event) GetEmoji() string {
	return ""
}

func (ev nine29Event) Basename() string {
	return ev.Reading.String()
}

func isHebrewLocale(locale string) bool {
	locale = strings.ToLower(locale)
	return locale == "he" || locale == "he-x-nonikud"
}

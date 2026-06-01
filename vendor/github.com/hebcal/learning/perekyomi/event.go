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
	"strings"

	"github.com/hebcal/gematriya"
	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/event"
	"github.com/hebcal/hebcal-go/locales"
)

type perekYomiEvent struct {
	Date    hdate.HDate
	Reading Reading
}

// NewPerekYomiEvent returns a calendar event for the Perek Yomi reading
// on the given date.
func NewPerekYomiEvent(hd hdate.HDate, reading Reading) event.CalEvent {
	return perekYomiEvent{Date: hd, Reading: reading}
}

func (ev perekYomiEvent) GetDate() hdate.HDate {
	return ev.Date
}

func (ev perekYomiEvent) Render(locale string) string {
	r := ev.Reading
	name, _ := locales.LookupTranslation(r.Tractate, locale)
	if l := strings.ToLower(locale); l == "he" || l == "he-x-nonikud" {
		return name + " " + gematriya.Gematriya(r.Chap)
	}
	return name + " " + strconv.Itoa(r.Chap)
}

func (ev perekYomiEvent) GetFlags() event.HolidayFlags {
	return event.DAILY_LEARNING
}

func (ev perekYomiEvent) GetEmoji() string {
	return ""
}

func (ev perekYomiEvent) Basename() string {
	return ev.Reading.String()
}

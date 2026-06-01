package dafyomi

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

type dafYomiEvent struct {
	Date hdate.HDate
	Daf  Daf
}

// NewDafYomiEvent returns a calendar event for the Daf Yomi on the given date.
func NewDafYomiEvent(hd hdate.HDate, daf Daf) event.CalEvent {
	return dafYomiEvent{Date: hd, Daf: daf}
}

func (ev dafYomiEvent) GetDate() hdate.HDate {
	return ev.Date
}

func (ev dafYomiEvent) Render(locale string) string {
	name, _ := locales.LookupTranslation(ev.Daf.Name, locale)
	locale = strings.ToLower(locale)
	if locale == "he" || locale == "he-x-nonikud" {
		return name + " דף " + gematriya.Gematriya(ev.Daf.Blatt)
	}
	return name + " " + strconv.Itoa(ev.Daf.Blatt)
}

func (ev dafYomiEvent) GetFlags() event.HolidayFlags {
	return event.DAF_YOMI
}

func (ev dafYomiEvent) GetEmoji() string {
	return ""
}

func (ev dafYomiEvent) Basename() string {
	return ev.Daf.String()
}

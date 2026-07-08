package nachyomi

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
	"github.com/hebcal/learning/dafyomi"
	"github.com/hebcal/locales"
)

type nachYomiEvent struct {
	Date    hdate.HDate
	Chapter dafyomi.Daf
}

// NewNachYomiEvent returns a calendar event for the Nach Yomi (a chapter
// of Nevi'im or Ketuvim) on the given date.
func NewNachYomiEvent(hd hdate.HDate, chapter dafyomi.Daf) event.CalEvent {
	return nachYomiEvent{Date: hd, Chapter: chapter}
}

func (ev nachYomiEvent) GetDate() hdate.HDate {
	return ev.Date
}

func (ev nachYomiEvent) Render(locale string) string {
	name, _ := locales.LookupTranslation(ev.Chapter.Name, locale)
	locale = strings.ToLower(locale)
	if locale == "he" || locale == "he-x-nonikud" {
		return name + " " + gematriya.Gematriya(ev.Chapter.Blatt)
	}
	return name + " " + strconv.Itoa(ev.Chapter.Blatt)
}

func (ev nachYomiEvent) GetFlags() event.HolidayFlags {
	return event.NACH_YOMI
}

func (ev nachYomiEvent) GetEmoji() string {
	return ""
}

func (ev nachYomiEvent) Basename() string {
	return ev.Chapter.String()
}

func (ev nachYomiEvent) GetCategories() []string {
	return []string{"nachyomi"}
}

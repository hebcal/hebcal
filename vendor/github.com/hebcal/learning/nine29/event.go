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
	"github.com/hebcal/learning/internal/sefaria"
	"github.com/hebcal/locales"
)

type nine29Event struct {
	date    hdate.HDate
	Reading Reading
}

// NewNine29Event returns a calendar event for the 929 reading on the
// given date.
func NewNine29Event(hd hdate.HDate, reading Reading) event.CalEvent {
	return nine29Event{date: hd, Reading: reading}
}

func (ev nine29Event) Date() hdate.HDate {
	return ev.date
}

// Render returns the chapter followed by its day in the cycle, e.g.
// "Deuteronomy 34 (187)".
func (ev nine29Event) Render(locale string) string {
	return ev.RenderBrief(locale) + " (" + strconv.Itoa(ev.Reading.CycleChap) + ")"
}

// RenderBrief returns the chapter without its day in the cycle, e.g.
// "Deuteronomy 34" or "דְּבָרִים ל״ד".
func (ev nine29Event) RenderBrief(locale string) string {
	r := ev.Reading
	if isHebrewLocale(locale) {
		name, _ := locales.LookupTranslation(r.Book, locale)
		return name + " " + gematriya.Gematriya(r.BookChap)
	}
	return r.Book + " " + strconv.Itoa(r.BookChap)
}

func (ev nine29Event) Flags() event.Flags {
	return event.DailyLearning
}

func (ev nine29Event) Emoji() string {
	return ""
}

// Desc returns the untranslated description, the same as Basename.
func (ev nine29Event) Desc() string { return ev.Basename() }

func (ev nine29Event) Basename() string {
	return ev.Reading.String()
}

func (ev nine29Event) Categories() []string {
	return []string{"929"}
}

// URL returns a link to sefaria.org for the current chapter, e.g.
// https://www.sefaria.org/Deuteronomy.34?lang=bi .
func (ev nine29Event) URL() string {
	return sefaria.URL(ev.Reading.Book, strconv.Itoa(ev.Reading.BookChap))
}

func isHebrewLocale(locale string) bool {
	locale = strings.ToLower(locale)
	return locale == "he" || locale == "he-x-nonikud"
}

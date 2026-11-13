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
	"github.com/hebcal/learning/internal/sefaria"
	"github.com/hebcal/locales"
)

type perekYomiEvent struct {
	date    hdate.HDate
	Reading Reading
}

// NewPerekYomiEvent returns a calendar event for the Perek Yomi reading
// on the given date.
func NewPerekYomiEvent(hd hdate.HDate, reading Reading) event.CalEvent {
	return perekYomiEvent{date: hd, Reading: reading}
}

func (ev perekYomiEvent) Date() hdate.HDate {
	return ev.date
}

func (ev perekYomiEvent) Render(locale string) string {
	r := ev.Reading
	name, _ := locales.LookupTranslation(r.Tractate, locale)
	if l := strings.ToLower(locale); l == "he" || l == "he-x-nonikud" {
		return name + " " + gematriya.Gematriya(r.Chap)
	}
	return name + " " + strconv.Itoa(r.Chap)
}

func (ev perekYomiEvent) Flags() event.Flags {
	return event.DailyLearning
}

func (ev perekYomiEvent) Emoji() string {
	return ""
}

// Desc returns the untranslated description, the same as Basename.
func (ev perekYomiEvent) Desc() string { return ev.Basename() }

func (ev perekYomiEvent) Basename() string {
	return ev.Reading.String()
}

func (ev perekYomiEvent) Categories() []string {
	return []string{"perekYomi"}
}

// URL returns a link to sefaria.org for the Mishna chapter.
func (ev perekYomiEvent) URL() string {
	prefix := "Mishnah"
	if ev.Reading.Tractate == "Avot" {
		prefix = "Pirkei"
	}
	return sefaria.URL(prefix+" "+ev.Reading.Tractate, strconv.Itoa(ev.Reading.Chap))
}

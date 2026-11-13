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
	"github.com/hebcal/learning/internal/sefaria"
	"github.com/hebcal/locales"
)

type psalmsEvent struct {
	date    hdate.HDate
	Reading Reading
}

// NewPsalmsEvent returns a calendar event for the Daily Psalms reading on
// the given date.
func NewPsalmsEvent(hd hdate.HDate, reading Reading) event.CalEvent {
	return psalmsEvent{date: hd, Reading: reading}
}

func (ev psalmsEvent) Date() hdate.HDate {
	return ev.date
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

func (ev psalmsEvent) Flags() event.Flags {
	return event.DailyLearning
}

func (ev psalmsEvent) Emoji() string {
	return ""
}

// Desc returns the untranslated description, the same as Basename.
func (ev psalmsEvent) Desc() string { return ev.Basename() }

func (ev psalmsEvent) Basename() string {
	return "Psalms " + ev.Reading.String()
}

func (ev psalmsEvent) Categories() []string {
	return []string{"dailyPsalms"}
}

// URL returns a link to sefaria.org for the Psalms portion, e.g.
// https://www.sefaria.org/Psalms.10-17?lang=bi .
func (ev psalmsEvent) URL() string {
	chapter := strings.ReplaceAll(ev.Reading.Begin+"-"+ev.Reading.End, ":", ".")
	return sefaria.URL("Psalms", chapter)
}

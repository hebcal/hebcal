package rambam

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

	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/event"
	"github.com/hebcal/learning/internal/hebrew"
	"github.com/hebcal/learning/internal/sefaria"
	"github.com/hebcal/locales"
)

// readingURL returns a link to sefaria.org for a single Mishneh Torah
// reading, e.g. https://www.sefaria.org/Mishneh_Torah%2C_Kings_and_Wars.4?lang=bi .
func readingURL(r Reading) string {
	name := "Mishneh Torah, " + r.Name + "." + r.Perek
	name = strings.ReplaceAll(name, " ", "_")
	name = strings.ReplaceAll(name, ":", ".")
	return "https://www.sefaria.org/" + sefaria.EncodeURIComponent(name) + "?lang=bi"
}

// renderReading renders a single Mishneh Torah reading in the given locale.
func renderReading(r Reading, locale string) string {
	if isHebrewLocale(locale) {
		name, _ := locales.LookupTranslation(r.Name, locale)
		return name + " פרק " + perekHebrew(r.Perek)
	}
	name, ok := locales.LookupTranslation(r.Name, locale)
	if !ok || name == "" {
		name = r.Name
	}
	return name + " " + r.Perek
}

// perekHebrew renders a perek reference in Hebrew: a plain chapter number
// becomes gematriya; a range is left as-is.
func perekHebrew(perek string) string {
	if n, err := strconv.Atoi(perek); err == nil {
		return hebrew.GematriyaNN(n)
	}
	return perek
}

func isHebrewLocale(locale string) bool {
	locale = strings.ToLower(locale)
	return locale == "he" || locale == "he-x-nonikud"
}

// --- 1 chapter a day ---

type rambam1Event struct {
	date    hdate.HDate
	Reading Reading
}

// NewDailyRambam1Event returns a calendar event for the 1-chapter-a-day
// Daily Rambam reading on the given date.
func NewDailyRambam1Event(hd hdate.HDate, reading Reading) event.CalEvent {
	return rambam1Event{date: hd, Reading: reading}
}

func (ev rambam1Event) Date() hdate.HDate           { return ev.date }
func (ev rambam1Event) Render(locale string) string { return renderReading(ev.Reading, locale) }
func (ev rambam1Event) Flags() event.Flags          { return event.DailyLearning }
func (ev rambam1Event) Emoji() string               { return "" }
func (ev rambam1Event) Basename() string            { return ev.Reading.String() }

// Desc returns the untranslated description, the same as Basename.
func (ev rambam1Event) Desc() string { return ev.Basename() }

func (ev rambam1Event) Categories() []string {
	return []string{"rambam1"}
}

// URL returns a link to sefaria.org for the chapter.
func (ev rambam1Event) URL() string {
	return readingURL(ev.Reading)
}

// --- 3 chapters a day ---

type rambam3Event struct {
	date     hdate.HDate
	Readings []Reading // collapsed for display
}

// NewDailyRambam3Event returns a calendar event for the 3-chapters-a-day
// Daily Rambam reading on the given date. Adjacent chapters in the same
// section are collapsed into a single range for display.
func NewDailyRambam3Event(hd hdate.HDate, readings []Reading) event.CalEvent {
	return rambam3Event{date: hd, Readings: collapseAdjacent(readings)}
}

func (ev rambam3Event) Date() hdate.HDate  { return ev.date }
func (ev rambam3Event) Flags() event.Flags { return event.DailyLearning }
func (ev rambam3Event) Emoji() string      { return "" }

func (ev rambam3Event) Render(locale string) string {
	parts := make([]string, len(ev.Readings))
	for i, r := range ev.Readings {
		parts[i] = renderReading(r, locale)
	}
	return strings.Join(parts, ", ")
}

// Desc returns the untranslated description, the same as Basename.
func (ev rambam3Event) Desc() string { return ev.Basename() }

func (ev rambam3Event) Basename() string {
	parts := make([]string, len(ev.Readings))
	for i, r := range ev.Readings {
		parts[i] = r.String()
	}
	return strings.Join(parts, ", ")
}

func (ev rambam3Event) Categories() []string {
	return []string{"rambam3"}
}

// Memo lists each reading with its sefaria.org link, a blank line between
// them, when the day's three chapters do not collapse to a single reading:
//
//	Neighbors 13-14
//	https://www.sefaria.org/Mishneh_Torah%2C_Neighbors.13-14?lang=bi
//
//	Agents and Partners 1
//	https://www.sefaria.org/Mishneh_Torah%2C_Agents_and_Partners.1?lang=bi
//
// A single reading has no memo; its link is URL's. The memo is English in
// every locale.
func (ev rambam3Event) Memo(locale string) string {
	if len(ev.Readings) < 2 {
		return ""
	}
	parts := make([]string, len(ev.Readings))
	for i, r := range ev.Readings {
		parts[i] = r.String() + "\n" + readingURL(r)
	}
	return strings.Join(parts, "\n\n")
}

// URL returns a link to sefaria.org when the day's three chapters collapse
// to a single reading; otherwise it returns "", and the links are in Memo.
func (ev rambam3Event) URL() string {
	if len(ev.Readings) == 1 {
		return readingURL(ev.Readings[0])
	}
	return ""
}

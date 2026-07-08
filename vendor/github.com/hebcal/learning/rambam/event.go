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

	"github.com/hebcal/gematriya"
	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/event"
	"github.com/hebcal/locales"
)

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
		return gematriya.Gematriya(n)
	}
	return perek
}

func isHebrewLocale(locale string) bool {
	locale = strings.ToLower(locale)
	return locale == "he" || locale == "he-x-nonikud"
}

// --- 1 chapter a day ---

type rambam1Event struct {
	Date    hdate.HDate
	Reading Reading
}

// NewDailyRambam1Event returns a calendar event for the 1-chapter-a-day
// Daily Rambam reading on the given date.
func NewDailyRambam1Event(hd hdate.HDate, reading Reading) event.CalEvent {
	return rambam1Event{Date: hd, Reading: reading}
}

func (ev rambam1Event) GetDate() hdate.HDate         { return ev.Date }
func (ev rambam1Event) Render(locale string) string  { return renderReading(ev.Reading, locale) }
func (ev rambam1Event) GetFlags() event.HolidayFlags { return event.DAILY_LEARNING }
func (ev rambam1Event) GetEmoji() string             { return "" }
func (ev rambam1Event) Basename() string             { return ev.Reading.String() }
func (ev rambam1Event) GetCategories() []string {
	return []string{"rambam1"}
}

// --- 3 chapters a day ---

type rambam3Event struct {
	Date     hdate.HDate
	Readings []Reading // collapsed for display
}

// NewDailyRambam3Event returns a calendar event for the 3-chapters-a-day
// Daily Rambam reading on the given date. Adjacent chapters in the same
// section are collapsed into a single range for display.
func NewDailyRambam3Event(hd hdate.HDate, readings []Reading) event.CalEvent {
	return rambam3Event{Date: hd, Readings: collapseAdjacent(readings)}
}

func (ev rambam3Event) GetDate() hdate.HDate         { return ev.Date }
func (ev rambam3Event) GetFlags() event.HolidayFlags { return event.DAILY_LEARNING }
func (ev rambam3Event) GetEmoji() string             { return "" }

func (ev rambam3Event) Render(locale string) string {
	parts := make([]string, len(ev.Readings))
	for i, r := range ev.Readings {
		parts[i] = renderReading(r, locale)
	}
	return strings.Join(parts, ", ")
}

func (ev rambam3Event) Basename() string {
	parts := make([]string, len(ev.Readings))
	for i, r := range ev.Readings {
		parts[i] = r.String()
	}
	return strings.Join(parts, ", ")
}

func (ev rambam3Event) GetCategories() []string {
	return []string{"rambam3"}
}

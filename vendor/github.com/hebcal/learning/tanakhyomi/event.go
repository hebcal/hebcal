package tanakhyomi

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

type tanakhYomiEvent struct {
	Date    hdate.HDate
	Reading Reading
}

// NewTanakhYomiEvent returns a calendar event for the Tanakh Yomi reading
// on the given date.
func NewTanakhYomiEvent(hd hdate.HDate, reading Reading) event.CalEvent {
	return tanakhYomiEvent{Date: hd, Reading: reading}
}

func (ev tanakhYomiEvent) GetDate() hdate.HDate {
	return ev.Date
}

func (ev tanakhYomiEvent) Render(locale string) string {
	r := ev.Reading
	var s string
	if l := strings.ToLower(locale); l == "he" || l == "he-x-nonikud" {
		name, _ := locales.LookupTranslation(r.Name, locale)
		s = name + " ס׳ " + sederHebrew(r.Blatt)
	} else {
		name, ok := locales.LookupTranslation(r.Name, locale)
		if !ok || name == "" {
			name = r.Name
		}
		s = name + " Seder " + r.Blatt
	}
	if r.Verses != "" {
		s += " (" + r.Verses + ")"
	}
	return s
}

// sederHebrew renders a seder number in Hebrew. A plain number becomes
// gematriya; a split seder like "4.1" becomes gematriya(major) + minor.
func sederHebrew(blatt string) string {
	if n, err := strconv.Atoi(blatt); err == nil {
		return gematriyaNN(n)
	}
	parts := strings.SplitN(blatt, ".", 2)
	if maj, err := strconv.Atoi(parts[0]); err == nil && len(parts) == 2 {
		return gematriyaNN(maj) + parts[1]
	}
	return blatt
}

// gematriyaNN formats a number as Hebrew letters without the geresh /
// gershayim punctuation marks.
func gematriyaNN(n int) string {
	return strings.NewReplacer("׳", "", "״", "").Replace(gematriya.Gematriya(n))
}

func (ev tanakhYomiEvent) GetFlags() event.HolidayFlags {
	return event.DAILY_LEARNING
}

func (ev tanakhYomiEvent) GetEmoji() string {
	return ""
}

func (ev tanakhYomiEvent) Basename() string {
	s := ev.Reading.String()
	if ev.Reading.Verses != "" {
		s += " (" + ev.Reading.Verses + ")"
	}
	return s
}

func (ev tanakhYomiEvent) GetCategories() []string {
	return []string{"tanakhYomi"}
}

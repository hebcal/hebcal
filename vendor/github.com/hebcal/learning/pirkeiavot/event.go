package pirkeiavot

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

const pirkeiAvot = "Pirkei Avot"

type pirkeiAvotEvent struct {
	Date     hdate.HDate
	Chapters []int
}

// NewPirkeiAvotSummerEvent returns a calendar event for the Pirkei Avot
// chapter(s) studied on the given Shabbat.
func NewPirkeiAvotSummerEvent(hd hdate.HDate, chapters []int) event.CalEvent {
	return pirkeiAvotEvent{Date: hd, Chapters: chapters}
}

func (ev pirkeiAvotEvent) GetDate() hdate.HDate {
	return ev.Date
}

func (ev pirkeiAvotEvent) Render(locale string) string {
	book, _ := locales.LookupTranslation(pirkeiAvot, locale)
	hebrew := false
	if l := strings.ToLower(locale); l == "he" || l == "he-x-nonikud" {
		hebrew = true
	}
	parts := make([]string, len(ev.Chapters))
	for i, c := range ev.Chapters {
		if hebrew {
			parts[i] = gematriya.Gematriya(c)
		} else {
			parts[i] = strconv.Itoa(c)
		}
	}
	return book + " " + strings.Join(parts, "-")
}

func (ev pirkeiAvotEvent) GetFlags() event.HolidayFlags {
	return event.DAILY_LEARNING
}

func (ev pirkeiAvotEvent) GetEmoji() string {
	return ""
}

func (ev pirkeiAvotEvent) Basename() string {
	parts := make([]string, len(ev.Chapters))
	for i, c := range ev.Chapters {
		parts[i] = strconv.Itoa(c)
	}
	return pirkeiAvot + " " + strings.Join(parts, "-")
}

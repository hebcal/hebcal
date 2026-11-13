// Package event defines CalEvent, the interface every calendar event
// satisfies, and the event types shared across the module: holidays, the
// weekly Torah portion, Hebrew dates, the molad and personal events.
package event

import (
	"time"

	"github.com/hebcal/hdate"
)

// Hebcal - A Jewish Calendar Generator
// Copyright (c) 2022 Michael J. Radwin
// Derived from original C version, Copyright (C) 1994-2004 Danny Sadinoff
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

// CalEvent is an event on the Hebrew calendar: a holiday, a Torah portion, a
// candle-lighting time, a day of the Omer, a daily-learning reading. It is
// implemented in this module and by schedule providers such as
// github.com/hebcal/learning.
//
// Some events can do more, through the optional interfaces BriefRenderer,
// Memoer, Timed and Linked.
type CalEvent interface {
	// Date returns the Hebrew date on which the event occurs.
	Date() hdate.HDate
	// Desc returns the untranslated description, the event's stable
	// identifier: "Erev Pesach", "Sukkot III (CH''M)", "Parashat Bereshit",
	// "Omer 7", "29 Elul 5786", "Molad Cheshvan 5787", "Candle lighting".
	// Unlike Basename, it tells apart the days of one holiday.
	Desc() string
	// Basename returns a simplified, untranslated description that groups
	// related events: "Pesach" for "Erev Pesach", "Sukkot" for "Sukkot III
	// (CH''M)". For many events it is the same as Desc.
	Basename() string
	// Render returns the description translated to locale, e.g. "Pesach
	// III (CH’’M)".
	Render(locale string) string
	// Flags returns the event's flag bitmask.
	Flags() Flags
	// Emoji returns the event's emoji, or "" if it has none.
	Emoji() string
	// Categories returns the category and optional sub-categories for this
	// event, e.g. ["holiday", "major", "fast"] or ["roshchodesh"]. The first
	// element is the primary category.
	Categories() []string
	// URL returns the absolute URL of the event's page (on hebcal.com for
	// the events of this module), or "" if it has none: a Molad, for
	// example, or a date outside the range the website covers.
	URL() string
}

// Timed is implemented by events that occur at a clock time:
// candle-lighting, Havdalah, the start or end of a fast, the latest time
// for Kiddush Levana. Assert it rather than a concrete type, so that every
// timed event is found, whatever package defines it.
type Timed interface {
	CalEvent
	// Time returns the time at which the event occurs, in the time zone of
	// the calendar's location.
	Time() time.Time
}

// Linked is implemented by events that belong to another event: the
// candle-lighting of a holiday, the end of a fast, Yizkor on Yom Kippur.
type Linked interface {
	// LinkedEvent returns the event this one belongs to, or nil if it has
	// none.
	LinkedEvent() CalEvent
}

// BriefRenderer is implemented by events that have a shorter description than
// Render's, for places with little room: a timed event without its time
// ("Candle lighting"), a Hebrew date without its year ("15th of Cheshvan"),
// "Omer day 22", or a daily-learning reading without its series name.
type BriefRenderer interface {
	RenderBrief(locale string) string
}

// RenderBrief returns ev's brief description if it has one, and Render's
// otherwise.
func RenderBrief(ev CalEvent, locale string) string {
	if b, ok := ev.(BriefRenderer); ok {
		return b.RenderBrief(locale)
	}
	return ev.Render(locale)
}

// flagToCategory maps an event flag to its category and sub-categories, in
// priority order (the first matching flag wins). It mirrors the flagToCategory
// table in @hebcal/core.
var flagToCategory = []struct {
	flag Flags
	cats []string
}{
	{MajorFast, []string{"holiday", "major", "fast"}},
	{ChanukahCandles, []string{"holiday", "minor"}},
	{HebrewDate, []string{"hebdate"}},
	{MinorFast, []string{"holiday", "fast"}},
	{MinorHoliday, []string{"holiday", "minor"}},
	{ModernHoliday, []string{"holiday", "modern"}},
	{Molad, []string{"molad"}},
	{OmerCount, []string{"omer"}},
	{ParshaHashavua, []string{"parashat"}},
	{RoshChodesh, []string{"roshchodesh"}},
	{ShabbatMevarchim, []string{"mevarchim"}},
	{SpecialShabbat, []string{"holiday", "shabbat"}},
	{User, []string{"user"}},
	{Yizkor, []string{"yizkor"}},
}

// CategoriesFromFlags returns the category and sub-categories implied by an
// event's flag bitmask, matching the base Event.getCategories() in
// @hebcal/core. It returns ["unknown"] when no flag matches. The returned
// slice is a fresh copy that the caller may modify.
func CategoriesFromFlags(mask Flags) []string {
	for _, fc := range flagToCategory {
		if mask.HasAny(fc.flag) {
			return append([]string(nil), fc.cats...)
		}
	}
	return []string{"unknown"}
}
